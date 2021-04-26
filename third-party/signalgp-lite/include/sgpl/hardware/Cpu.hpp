#pragma once
#ifndef SGPL_HARDWARE_CPU_HPP_INCLUDE
#define SGPL_HARDWARE_CPU_HPP_INCLUDE

#include <utility>

#include "../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"

#include "../utility/RingResevoir.hpp"

#include "Core.hpp"
#include "JumpTable.hpp"

namespace sgpl {

template<typename Spec>
class Cpu {

  using core_t = sgpl::Core<Spec>;
  using global_jump_table_t
    = sgpl::JumpTable<Spec, typename Spec::global_matching_t>;

  struct impl_ {

    sgpl::RingResevoir<core_t, Spec::num_cores> scheduler;

    size_t active_core_idx{};

    emp::array<global_jump_table_t, Spec::num_global_jump_tables>
      global_jump_tables{};

    // total number of instructions executed over the lifetime of the object
    size_t lifetime_cycle_clock{};

  } data;

  using tag_t = typename Spec::tag_t;

  void RefreshCoreGlobalJumpTablePtrs() {
    for (size_t i{}; i < data.scheduler.GetCapacity(); ++i) {
      data.scheduler.GetBuffer()[i].SetGlobalJumpTables(
        data.global_jump_tables
      );
    }
  }

public:

  /// Default constructor.
  Cpu() { RefreshCoreGlobalJumpTablePtrs(); }

  /// Copy constructor.
  Cpu(const Cpu& other) : data(other.data) { RefreshCoreGlobalJumpTablePtrs(); }

  /// Move constructor.
  Cpu(Cpu&& other) noexcept : data( std::move(other.data) ) {
    RefreshCoreGlobalJumpTablePtrs();
  }

  /// Copy assignment operator.
  Cpu& operator=(const Cpu& other) { return *this = Cpu(other); }

  /// Move assignment operator.
  Cpu& operator=(Cpu&& other) noexcept {
    std::swap(data, other.data);
    return *this;
  }

  void ActivateNextCore() {
    emp_assert( GetNumBusyCores() );
    ++data.active_core_idx %= GetNumBusyCores();
  }

  bool TryActivateNextCore() {
    if ( HasActiveCore() ) { ActivateNextCore(); return true; }
    else { emp_assert( data.active_core_idx == 0 ); return false; }
  }

  void ActivatePrevCore() {
    emp_assert( GetNumBusyCores() );
    data.active_core_idx += GetNumBusyCores() - 1;
    data.active_core_idx %= GetNumBusyCores();
  }

  bool TryActivatePrevCore() {
    if ( HasActiveCore() ) { ActivatePrevCore(); return true; }
    else { emp_assert( data.active_core_idx == 0 ); return false; }
  }

  __attribute__ ((hot))
  core_t& GetActiveCore() {
    emp_assert( HasActiveCore() );
    return data.scheduler.Get( data.active_core_idx );
  };

  core_t& GetFreshestCore() {
    emp_assert( HasActiveCore() );
    return data.scheduler.GetHead();
  };

  void KillActiveCore() {
    emp_assert( HasActiveCore() );
    for ( const auto& req : GetActiveCore().fork_requests ) {
      if ( !TryLaunchCore(req) ) break;
    }
    data.scheduler.Release(data.active_core_idx);
    TryActivatePrevCore();
  }

  void KillStaleCore() {
    emp_assert( !HasFreeCore() );
    data.scheduler.ReleaseTail();
    // no need to activate prev core, killed core is idx 0
  }

  void DoLaunchCore() {
    emp_assert( HasFreeCore() );
    auto& acquired = data.scheduler.Acquire();
    acquired.Reset();
  }

  bool TryLaunchCore() {
    if ( ! HasFreeCore() ) return false;
    else { DoLaunchCore(); return true; }
  }

  void ForceLaunchCore() {
    if ( ! HasFreeCore() ) KillStaleCore();
    DoLaunchCore();
  }

  void DoLaunchCore( const tag_t& tag, const size_t jt_idx=0 ) {
    emp_assert( HasFreeCore() );
    auto& acquired = data.scheduler.Acquire();
    acquired.Reset();
    acquired.JumpToGlobalAnchorMatch( tag, jt_idx );
  }

  bool TryLaunchCore( const tag_t& tag, const size_t jt_idx=0 ) {
    if ( ! HasFreeCore() ) return false;
    else { DoLaunchCore( tag, jt_idx ); return true; }
  }

  void ForceLaunchCore( const tag_t& tag, const size_t jt_idx=0 ) {
    if ( ! HasFreeCore() ) KillStaleCore();
    DoLaunchCore( tag, jt_idx );
  }

  size_t GetNumBusyCores() const { return data.scheduler.GetSize(); }

  size_t GetNumFreeCores() const {
    return data.scheduler.GetAvailableCapacity();
  }

  size_t GetMaxCores() const { return data.scheduler.GetCapacity(); }

  __attribute__ ((hot))
  bool HasActiveCore() const { return GetNumBusyCores(); }

  __attribute__ ((hot))
  bool HasFreeCore() const { return GetNumFreeCores(); }

  void Reset() {
    data.scheduler.Reset();
    data.active_core_idx = {};
    for ( auto& table : data.global_jump_tables ) table.Clear();
  }

  void InitializeAnchors(const sgpl::Program<Spec>& program) {
    Reset();
    for( size_t i{}; i < data.global_jump_tables.size() ; ++i ) {
      data.global_jump_tables[i].InitializeGlobalAnchors(
        program,
        Spec::global_jump_table_inclusion_mods[ i ]
      );
    }
  }

  const core_t& GetCore( const size_t idx ) const {
    return data.scheduler.Get( idx );
  }

  const global_jump_table_t& GetGlobalJumpTable( const size_t idx=0 ) const {
    return data.global_jump_tables[ idx ];
  }

  void DecayGlobalRegulators() {
    for ( auto& gjt : data.global_jump_tables ) gjt.DecayRegulators();
  }

  void AdvanceCycleClock(const size_t amt) { data.lifetime_cycle_clock += amt; }

  size_t GetCyclesSinceConstruction() const {
    return data.lifetime_cycle_clock;
  }

};


} // namespace sgpl

#endif // #ifndef SGPL_HARDWARE_CPU_HPP_INCLUDE
