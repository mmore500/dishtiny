#pragma once
#ifndef SGPL_HARDWARE_CORE_HPP_INCLUDE
#define SGPL_HARDWARE_CORE_HPP_INCLUDE

#include <limits>

#include "../../../third-party/Empirical/include/emp/base/array.hpp"

#include "../utility/CappedSet.hpp"

#include "JumpTable.hpp"

namespace sgpl {

template<typename Spec>
class Core {

  size_t program_counter{};

  sgpl::JumpTable<Spec, typename Spec::local_matching_t> local_jump_table;

  using global_jump_table_t
    = sgpl::JumpTable<Spec, typename Spec::global_matching_t>;
  using global_jump_table_array_t
    = emp::array<global_jump_table_t, Spec::num_global_jump_tables>;
  global_jump_table_array_t* global_jump_tables{}; // non-owning ptr

  using tag_t = typename Spec::tag_t;

public:

  Core() = default;

  Core(
    global_jump_table_array_t& global_jump_tables_
  ) : global_jump_tables(&global_jump_tables_)
  { ; }

  using registers_t = emp::array<float, Spec::num_registers>;
  registers_t registers{}; // value initialize

  sgpl::CappedSet<tag_t, Spec::num_fork_requests> fork_requests{};

  inline void Terminate() noexcept {
    program_counter = std::numeric_limits<size_t>::max();
  };

  __attribute__ ((hot))
  inline bool HasTerminated() const noexcept {
    return program_counter == std::numeric_limits<size_t>::max();
  }

  __attribute__ ((hot))
  inline size_t GetProgramCounter() const noexcept { return program_counter; }

  __attribute__ ((hot))
  void AdvanceProgramCounter(const size_t program_length) noexcept {
    // equivalent to
    // if ( HasTerminated() == false ) {
    //   ++program_counter;
    //  program_counter %= program_length;
    // }
    const bool has_termianted{ HasTerminated() };
    ++program_counter; // if has terminated, wraps around
    program_counter %= program_length;
    program_counter -= has_termianted;

    emp_assert( has_termianted || program_counter < program_length );
  }

  inline bool HasLocalAnchors() const noexcept {
    return local_jump_table.GetSize();
  }

  inline void LoadLocalAnchors( const sgpl::Program<Spec>& program ) noexcept {
    emp_assert( ! HasLocalAnchors() );
    local_jump_table.InitializeLocalAnchors( program, GetProgramCounter() );
  }

  void JumpToGlobalAnchorMatch(
    const tag_t& query, const size_t jt_idx=0
  ) noexcept {
    const auto res { global_jump_tables->at( jt_idx ).MatchRegulated(query) };
    if ( res.size() ) {
      program_counter = global_jump_tables->at( jt_idx ).GetVal(res.front());
    } else Terminate();
    local_jump_table.Clear();
  }

  void JumpToLocalAnchorMatch(const tag_t& query) noexcept {
    const auto res { local_jump_table.MatchRegulated(query) };
    if ( res.size() ) program_counter = local_jump_table.GetVal( res.front() );
  }

  inline auto& GetLocalJumpTable() noexcept { return local_jump_table; }

  inline auto& GetGlobalJumpTable(const size_t jt_idx=0) noexcept {
    return global_jump_tables->at( jt_idx );
  }

  bool RequestFork(const tag_t& tag) noexcept {
    return fork_requests.try_push_back( tag );
  }

  void ResetRegisters() noexcept { registers.fill( {} ); }

  void SetRegisters(const registers_t& set) noexcept { registers = set; }

  const registers_t& GetRegisters() noexcept { return registers; }

  void Reset() noexcept {
    fork_requests.clear();
    ResetRegisters();
    local_jump_table.Clear();
    program_counter = 0;
  }

  void SetGlobalJumpTables(global_jump_table_array_t& j_tables) noexcept {
    global_jump_tables = &j_tables;
  }

  void DecayRegulators() noexcept { local_jump_table.DecayRegulators(); }

};

} // namespace sgpl

#endif // #ifndef SGPL_HARDWARE_CORE_HPP_INCLUDE
