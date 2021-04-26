#include <chrono>
#include <iostream>
#include <map>
#include <sstream>

#include "../../../third-party/Empirical/include/emp/config/ArgManager.hpp"

#include "netuit/arrange/ToroidalTopologyFactory.hpp"
#include "netuit/assign/GenerateMetisAssignments.hpp"
#include "uitsl/containers/safe/unordered_map.hpp"
#include "uitsl/debug/safe_cast.hpp"
#include "uitsl/distributed/do_successively.hpp"
#include "uitsl/mpi/comm_utils.hpp"
#include "uitsl/mpi/mpi_flex_guard.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"
#include "uitsl/utility/assign_utils.hpp"

#include "config/num_nodes.hpp"

#include "Job.hpp"

int main(int argc, char* argv[]) {

  emp::ArgManager am{ argc, argv, emp::ArgManager::make_builtin_specs(&cfg) };
  am.UseCallbacks();
  if ( am.HasUnused() ) std::exit( EXIT_FAILURE );

  if ( cfg.N_THREADS() == 1 ) uitsl::mpi_flex_guard.InitSingleThread();
  else uitsl::mpi_flex_guard.InitMultithread();

  if ( uitsl::is_root() ) {
    std::cout << ">>> begin <<<" << '\n' << '\n';
  }

  const size_t dim = static_cast<size_t>(std::pow(num_nodes(), 0.5));
  const auto topology = netuit::make_toroidal_topology(
    { dim, dim }
  );

  const std::pair<
      std::function<uitsl::proc_id_t(size_t)>,
      std::function<uitsl::thread_id_t(size_t)>
  > assignments = netuit::GenerateMetisAssignmentFunctors(
    uitsl::safe_cast<size_t>( uitsl::get_nprocs() ),
    cfg.N_THREADS(),
    topology
  );


  // todo switch this out for assign metis
  netuit::Mesh<ImplSpec> mesh{
    topology,
    assignments.second,
    assignments.first
  };

  uitsl::safe::unordered_map<size_t, std::string> res;

  uitsl::ThreadTeam team;
  for (size_t thread = 0; thread < cfg.N_THREADS(); ++thread) team.Add(
    [&mesh, &res, thread](){

      // run the job
      Job job{ thread, mesh.GetSubmesh(thread) };

      std::stringstream ss;

      ss << "process " << uitsl::get_proc_id() << '\n';
      ss << "thread " << thread << '\n';
      ss << job.ToString() << '\n';

      res[thread] = ss.str();

    }
  );
  team.Join();

  uitsl::do_successively(
    [&](){
      for (const auto& [k, v] : std::map{ std::begin(res), std::end(res) }) {
        // prevent excessive logs for many-proc jobs
        if (uitsl::get_proc_id() < 4) std::cout << v;
      }
    },
    uitsl::print_separator
  );

  return 0;
}
