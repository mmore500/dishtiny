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

  void RefreshCoreGlobalJumpTablePtrs() noexcept {
    for (size_t i{}; i < data.scheduler.GetCapacity(); ++i) {
      data.scheduler.GetBuffer()[i].SetGlobalJumpTables(
        data.global_jump_tables
      );
    }
  }

public:

  /// Default constructor.
  Cpu() noexcept { RefreshCoreGlobalJumpTablePtrs(); }

  /// Copy constructor.
  Cpu(const Cpu& other) noexcept : data(other.data) {
    RefreshCoreGlobalJumpTablePtrs();
  }

  /// Move constructor.
  Cpu(Cpu&& other) noexcept : data( std::move(other.data) ) {
    RefreshCoreGlobalJumpTablePtrs();
  }

  /// Copy assignment operator.
  Cpu& operator=(const Cpu& other) noexcept { return *this = Cpu(other); }

  /// Move assignment operator.
  Cpu& operator=(Cpu&& other) noexcept {
    std::swap(data, other.data);
    return *this;
  }

  void ActivateNextCore() noexcept {
    emp_assert( GetNumBusyCores() );
    ++data.active_core_idx %= GetNumBusyCores();
  }

  bool TryActivateNextCore() noexcept {
    if ( HasActiveCore() ) { ActivateNextCore(); return true; }
    else { emp_assert( data.active_core_idx == 0 ); return false; }
  }

  void ActivatePrevCore() noexcept {
    emp_assert( GetNumBusyCores() );
    data.active_core_idx += GetNumBusyCores() - 1;
    data.active_core_idx %= GetNumBusyCores();
  }

  bool TryActivatePrevCore() noexcept {
    if ( HasActiveCore() ) { ActivatePrevCore(); return true; }
    else { emp_assert( data.active_core_idx == 0 ); return false; }
  }

  __attribute__ ((hot))
  core_t& GetActiveCore() noexcept {
    emp_assert( HasActiveCore() );
    return data.scheduler.Get( data.active_core_idx );
  };

  core_t& GetFreshestCore() noexcept {
    emp_assert( HasActiveCore() );
    return data.scheduler.GetHead();
  };

  void KillActiveCore() noexcept {
    emp_assert( HasActiveCore() );
    for ( const auto& req : GetActiveCore().fork_requests ) {
      if ( !TryLaunchCore(req) ) break;
    }
    data.scheduler.Release(data.active_core_idx);
    TryActivatePrevCore();
  }

  void KillStaleCore() noexcept {
    emp_assert( !HasFreeCore() );
    data.scheduler.ReleaseTail();
    // no need to activate prev core, killed core is idx 0
  }

  void DoLaunchCore() noexcept {
    emp_assert( HasFreeCore() );
    auto& acquired = data.scheduler.Acquire();
    acquired.Reset();
  }

  bool TryLaunchCore() noexcept {
    if ( ! HasFreeCore() ) return false;
    else { DoLaunchCore(); return true; }
  }

  void ForceLaunchCore() noexcept {
    if ( ! HasFreeCore() ) KillStaleCore();
    DoLaunchCore();
  }

  void DoLaunchCore( const tag_t& tag, const size_t jt_idx=0 ) noexcept {
    emp_assert( HasFreeCore() );
    auto& acquired = data.scheduler.Acquire();
    acquired.Reset();
    acquired.JumpToGlobalAnchorMatch( tag, jt_idx );
  }

  bool TryLaunchCore( const tag_t& tag, const size_t jt_idx=0 ) noexcept {
    if ( ! HasFreeCore() ) return false;
    else { DoLaunchCore( tag, jt_idx ); return true; }
  }

  void ForceLaunchCore( const tag_t& tag, const size_t jt_idx=0 ) noexcept {
    if ( ! HasFreeCore() ) KillStaleCore();
    DoLaunchCore( tag, jt_idx );
  }

  size_t GetNumBusyCores() const noexcept { return data.scheduler.GetSize(); }

  size_t GetNumFreeCores() const noexcept {
    return data.scheduler.GetAvailableCapacity();
  }

  size_t GetMaxCores() const noexcept { return data.scheduler.GetCapacity(); }

  __attribute__ ((hot))
  bool HasActiveCore() const noexcept { return GetNumBusyCores(); }

  __attribute__ ((hot))
  bool HasFreeCore() const noexcept { return GetNumFreeCores(); }

  void Reset() noexcept {
    data.scheduler.Reset();
    data.active_core_idx = {};
    for ( auto& table : data.global_jump_tables ) table.Clear();
  }

  void InitializeAnchors(const sgpl::Program<Spec>& program) noexcept {
    Reset();
    for( size_t i{}; i < data.global_jump_tables.size() ; ++i ) {
      data.global_jump_tables[i].InitializeGlobalAnchors(
        program,
        Spec::global_jump_table_inclusion_mods[ i ]
      );
    }
  }

  const core_t& GetCore( const size_t idx ) const noexcept {
    return data.scheduler.Get( idx );
  }

  const global_jump_table_t& GetGlobalJumpTable( const size_t idx=0 )
    const noexcept
  {
    return data.global_jump_tables[ idx ];
  }

  void DecayGlobalRegulators() noexcept {
    for ( auto& gjt : data.global_jump_tables ) gjt.DecayRegulators();
  }

  void AdvanceCycleClock(const size_t amt) noexcept {
    data.lifetime_cycle_clock += amt;
   }

  size_t GetCyclesSinceConstruction() const noexcept {
    return data.lifetime_cycle_clock;
  }

};


} // namespace sgpl

#endif // #ifndef SGPL_HARDWARE_CPU_HPP_INCLUDE
