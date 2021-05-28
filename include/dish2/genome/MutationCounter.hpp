#pragma once
#ifndef DISH2_GENOME_MUTATIONCOUNTER_HPP_INCLUDE
#define DISH2_GENOME_MUTATIONCOUNTER_HPP_INCLUDE

#include <cstddef>
#include <tuple>

#include "../../../third-party/cereal/include/cereal/cereal.hpp"

namespace dish2 {

struct MutationCounter {

  uint64_t point_mutation_counter{};
  uint64_t insertion_deletion_counter{};
  uint64_t mutation_occurrence_counter{};

  void RecordPointMutation(const size_t num) { point_mutation_counter += num; }

  void RecordInsertionDeletion(const size_t num) {
    insertion_deletion_counter += num;
  }

  void RecordMutationOccurrence() { ++mutation_occurrence_counter; }

  bool operator==(const MutationCounter& other) const {
    return std::tuple{
      point_mutation_counter,
      insertion_deletion_counter
    } == std::tuple{
      other.point_mutation_counter,
      other.insertion_deletion_counter
    };
  }

  bool operator<(const MutationCounter& other) const {
    return std::tuple{
      point_mutation_counter,
      insertion_deletion_counter
    } < std::tuple{
      other.point_mutation_counter,
      other.insertion_deletion_counter
    };
  }

  template<typename Archive> void serialize( Archive & ar ) { ar(
    CEREAL_NVP( point_mutation_counter ),
    CEREAL_NVP( insertion_deletion_counter ),
    CEREAL_NVP( mutation_occurrence_counter )
  ); }

};

} // namespace dish2

#endif // #ifndef DISH2_GENOME_MUTATIONCOUNTER_HPP_INCLUDE
