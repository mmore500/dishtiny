#pragma once
#ifndef UITSL_MPI_GROUP_UTILS_HPP_INCLUDE
#define UITSL_MPI_GROUP_UTILS_HPP_INCLUDE

#include <numeric>
#include <set>
#include <sstream>

#include <mpi.h>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/math/math.hpp"

#include "../debug/safe_compare.hpp"
#include "../math/math_utils.hpp"
#include "../utility/print_utils.hpp"

#include "audited_routines.hpp"
#include "proc_id_t.hpp"

namespace uitsl {

// predeclaration
inline MPI_Group comm_to_group(const MPI_Comm &);

inline MPI_Group intersect_groups(emp::vector<MPI_Group> groups) {

  MPI_Group res{
    groups.size() ? groups.back() : MPI_GROUP_EMPTY
  };

  // TODO more MPI_Group_free ?
  for ( ; groups.size(); groups.pop_back()) {

    MPI_Group temp;
    UITSL_Group_intersection(
      res, // MPI_Group group1,
      groups.back(), // MPI_Group group2,
      &temp // MPI_Group* newgroup
    );
    UITSL_Group_free(
      &res // MPI_Group* group
    );
    res = temp;
  }

  return res;

}

inline MPI_Group combine_groups(emp::vector<MPI_Group> groups) {

  MPI_Group res{
    groups.size() ? groups.back() : MPI_GROUP_EMPTY
  };

  // TODO more MPI_Group_free ?
  for ( ; groups.size(); groups.pop_back()) {

    MPI_Group temp;
    UITSL_Group_union(
      res, // MPI_Group group1
      groups.back(), // MPI_Group group2
      &temp // MPI_Group* newgroup
    );
    UITSL_Group_free(
      &res // MPI_Group* group
    );
    res = temp;
  }

  return res;

}

inline MPI_Group subtract_groups(
  const MPI_Group & minuend,
  const MPI_Group & subtrahend
) {

  MPI_Group res;

  UITSL_Group_difference(
    minuend, // MPI_Group group1
    subtrahend, // MPI_Group group2
    &res // MPI_Group * newgroup
  );

  return res;

}

inline size_t group_size(const MPI_Group & group) {
  int res;
  UITSL_Group_size(
    group, // MPI_Group group
    &res // int *size
  );
  emp_assert(res >= 0);
  return res;
}

inline MPI_Comm group_to_comm(
  const MPI_Group & group,
  const MPI_Comm & comm=MPI_COMM_WORLD
) {
  MPI_Comm res;
  UITSL_Comm_create_group(
    comm, // MPI_Comm comm
    group, // MPI_Group group
    0, // int tag TODO is this important?
    &res // MPI_Comm * newcomm
  );
  return res;
}

inline MPI_Group make_group(
  emp::vector<proc_id_t> ranks,
  const MPI_Group source=uitsl::comm_to_group(MPI_COMM_WORLD)
) {

  std::sort(std::begin(ranks), std::end(ranks));
  const auto last{ std::unique(std::begin(ranks), std::end(ranks)) };
  ranks.erase(last, std::end(ranks));

  emp_assert(std::set<proc_id_t>(
    std::begin(ranks),
    std::end(ranks)
  ).size() == ranks.size(), uitsl::to_string(ranks));
  emp_assert(std::all_of(
    std::begin(ranks),
    std::end(ranks),
    [&](const auto & rank){
      return uitsl::safe_less(rank, uitsl::group_size(source)) && rank >= 0;
    }
  ), uitsl::to_string(ranks));

  MPI_Group res;
  UITSL_Group_incl(
    source, // MPI_Group group
    ranks.size(), // int n
    ranks.data(), // const int ranks[]
    &res // MPI_Group * newgroup
  );
  return res;
}

inline proc_id_t translate_group_rank(
  const proc_id_t rank,
  const MPI_Group& from,
  const MPI_Group& to=uitsl::comm_to_group(MPI_COMM_WORLD)
) {
  proc_id_t res;
  UITSL_Group_translate_ranks(
    from, // MPI_Group group1
    1, // int n
    &rank, // const int ranks1[]
    to, // MPI_Group group2
    &res // int ranks2[]
  );
  return res;
}

inline emp::vector<proc_id_t> get_group_ranks(const MPI_Group& group) {

  emp::vector<proc_id_t> within_group_ranks(group_size(group));
  std::iota(
    std::begin(within_group_ranks),
    std::end(within_group_ranks),
    0
  );

  emp::vector<proc_id_t> within_world_ranks(within_group_ranks.size());
  UITSL_Group_translate_ranks(
    group, // MPI_Group group1
    within_group_ranks.size(), // int n
    within_group_ranks.data(), // const int ranks1[]
    comm_to_group(MPI_COMM_WORLD), // MPI_Group group2
    within_world_ranks.data() // int ranks2[]
  );

  return within_world_ranks;

}

inline std::string group_to_string(const MPI_Group& group) {
  std::stringstream ss;
  ss << uitsl::format_member(
    "uitsl::group_size(group)", uitsl::group_size(group)
  ) << '\n';
  ss << uitsl::format_member(
    "uitsl::get_group_ranks(group)", uitsl::to_string(uitsl::get_group_ranks(group))
  ) << '\n';
  return ss.str();
}

} // namespace uitsl

#endif // #ifndef UITSL_MPI_GROUP_UTILS_HPP_INCLUDE
