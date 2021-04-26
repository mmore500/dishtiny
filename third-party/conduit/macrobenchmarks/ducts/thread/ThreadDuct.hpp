#include <thread>

#include <mpi.h>

#include "uitsl/chrono/TimeGuard.hpp"
#include "uitsl/concurrent/Gatherer.hpp"
#include "uitsl/debug/benchmark_utils.hpp"
#include "uitsl/debug/safe_cast.hpp"
#include "uitsl/mpi/mpi_utils.hpp"
#include "uitsl/nonce/CircularIndex.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"
#include "uitsl/parallel/thread_utils.hpp"
#include "uitsl/polyfill/latch.hpp"

#include "uit/fixtures/Conduit.hpp"
#include "uit/setup/ImplSpec.hpp"

#include "netuit/arrange/RingTopologyFactory.hpp"
#include "netuit/mesh/Mesh.hpp"

#define MESSAGE_T int

using Spec = uit::ImplSpec<MESSAGE_T, ImplSel>;

void do_work(
  netuit::Mesh<Spec>::submesh_t submesh,
  std::latch& latch,
  uitsl::Gatherer<MESSAGE_T>& gatherer
) {

  std::chrono::milliseconds duration; { const uitsl::TimeGuard guard{duration};

  auto optional_input = submesh.front().GetInputOrNullopt(0);
  auto optional_output = submesh.front().GetOutputOrNullopt(0);

  if (optional_output) optional_output->TryPut(uitsl::get_thread_id());

  latch.arrive_and_wait();

  for (size_t rep = 0; rep < 1e7; ++rep) {
    if (optional_output) optional_output->TryPut(uitsl::get_thread_id());
    if (optional_input) uitsl::do_not_optimize(
      optional_input->JumpGet()
    );
  }

  } // close TimeGuard

  gatherer.Put(duration.count());

}

void profile_thread_count(const size_t num_threads) {

  uitsl::ThreadTeam team;

  netuit::Mesh<Spec> mesh{
    netuit::RingTopologyFactory{}(num_threads),
    uitsl::AssignSegregated<uitsl::thread_id_t>{}
  };

  uitsl::Gatherer<MESSAGE_T> gatherer(MPI_INT);

  std::chrono::milliseconds duration; { const uitsl::TimeGuard guard{duration};

  std::latch latch{uitsl::safe_cast<std::ptrdiff_t>(num_threads)};
  for (uitsl::thread_id_t i = 0; i < num_threads; ++i) {
    team.Add(
      [i, &latch, &gatherer, &mesh](){
        do_work(mesh.GetSubmesh(i), latch, gatherer);
      }
    );
  }

  team.Join();

  } // close TimeGuard

  auto res = gatherer.Gather();

  if (res) {

    std::cout << "threads: " << num_threads << '\n';

    std::cout << "mean milliseconds:" << std::accumulate(
        std::begin(*res),
        std::end(*res),
        0.0
      ) / std::size(*res) << '\n';

    std::cout << "net milliseconds:" << duration.count() << '\n';

  }


}

int main(int argc, char* argv[]) {

  int provided;
  UITSL_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
  emp_assert(provided >= MPI_THREAD_FUNNELED);

  for (size_t threads = 1; threads <= uitsl::get_nproc(); threads*=2) {
    profile_thread_count(threads);
  }

  MPI_Finalize();

  return 0;
}
