#pragma once
#ifndef SGPL_SPEC_SPEC_HPP_INCLUDE
#define SGPL_SPEC_SPEC_HPP_INCLUDE

#include <array>
#include <ratio>
#include <type_traits>

#include "../../../third-party/Empirical/include/emp/matching/matchbin_metrics.hpp"
#include "../../../third-party/Empirical/include/emp/matching/MatchDepository.hpp"
#include "../../../third-party/Empirical/include/emp/matching/regulators/PlusCountdownRegulator.hpp"
#include "../../../third-party/Empirical/include/emp/matching/selectors_static/RankedSelector.hpp"

#include "../library/prefab/CompleteOpLibrary.hpp"
#include "../utility/EmptyType.hpp"

namespace sgpl {

template<
  typename Library=sgpl::CompleteOpLibrary,
  typename Peripheral=sgpl::EmptyType
>
struct Spec {

  using library_t = Library;

  using peripheral_t = Peripheral;

  // make match cutoff threshold strict because there are many potential matches
  // among modules (each module is a potential match)
  /// What matching implementation should we use for global jump tables?
  using global_matching_t = emp::MatchDepository<
    unsigned short, // program index type
    // matching metric
    emp::OptimizedApproxDualStreakMetric<64>,
    // match selector
    emp::statics::RankedSelector<
      std::ratio<1, 5> // match threshold
    >,
    // regulator
    emp::PlusCountdownRegulator<
      std::deci, // Slope
      std::ratio<1,4>, // MaxUpreg
      std::deci, // ClampLeeway
      2 // CountdownStart
    >,
    true, // raw caching
    8 // regulated caching
  >;

  // make match cutoff threshold lax because there aren't very many potential
  // matches within a module
  /// What matching datastructure implementation should we use for local jump
  /// tables?
  using local_matching_t = emp::MatchDepository<
    unsigned short, // program index type
    // matching metric
    emp::OptimizedApproxDualStreakMetric<64>,
    // match selector
    emp::statics::RankedSelector<
      std::ratio<1, 2> // match threshold
    >,
    // regulator
    emp::PlusCountdownRegulator<
      std::deci, // Slope
      std::ratio<1,4>, // MaxUpreg
      std::deci, // ClampLeeway
      2 // CountdownStart
    >,
    false, // raw caching
    0 // regulated caching
  >;

  using tag_t = typename global_matching_t::tag_t;

  static_assert( std::is_same<
    typename global_matching_t::tag_t, typename local_matching_t::tag_t
  >::value );

  /// How many virtual cores should a virtual CPU be able to support?
  static constexpr inline size_t num_cores{ 16 };

  /// How many fork requests can a virtual core make at most?
  static constexpr inline size_t num_fork_requests{ 3 };

  // must be power of 2
  /// How many registers should each virtual core contain?
  static constexpr inline size_t num_registers{ 8 };

  /// Maximum num steps executed on one core before next core is executed.
  static constexpr inline size_t switch_steps{ 8 };

  static constexpr inline std::array<size_t, 2>
    global_jump_table_inclusion_mods{ 1, 2 };

  static constexpr inline size_t num_global_jump_tables
    = global_jump_table_inclusion_mods.size();

};

} // namespace sgpl

#endif // #ifndef SGPL_SPEC_SPEC_HPP_INCLUDE
