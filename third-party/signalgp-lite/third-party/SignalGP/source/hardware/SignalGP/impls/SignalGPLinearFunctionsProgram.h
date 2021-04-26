#ifndef EMP_LINEAR_FUNCTIONS_PROGRAM_SIGNALGP_H
#define EMP_LINEAR_FUNCTIONS_PROGRAM_SIGNALGP_H

#include <iostream>
#include <utility>
#include <memory>

#include "emp/base/Ptr.hpp"
#include "emp/base/vector.hpp"
#include "emp/math/Random.hpp"
#include "emp/matching/MatchBin.hpp"
#include "emp/matching/matchbin_utils.hpp"

#include "../EventLibrary.h"
#include "../utils/InstructionLibrary.h"

#include "../SignalGPBase.h"
#include "../utils/linear_signalgp_utils.h"
#include "../utils/LinearFunctionsProgram.h"

namespace sgp {
  // @discussion: std::ratio for thresh is unintuitive w/out some handholding
  template<typename MEMORY_MODEL_T,
           typename TAG_T=emp::BitSet<16>,
           typename INST_ARGUMENT_T=int,
           typename MATCHBIN_T=emp::MatchBin< size_t,
                                              emp::HammingMetric<16>,
                                              emp::RankedSelector<>,
                                              emp::AdditiveCountdownRegulator<>
                                            >,
           typename CUSTOM_COMPONENT_T=sgp::DefaultCustomComponent>
  class LinearFunctionsProgramSignalGP : public SignalGPBase<LinearFunctionsProgramSignalGP<MEMORY_MODEL_T,TAG_T,INST_ARGUMENT_T,MATCHBIN_T,CUSTOM_COMPONENT_T>,
                                                             lsgp_utils::ExecState<MEMORY_MODEL_T>,
                                                             TAG_T,
                                                             CUSTOM_COMPONENT_T>

  {
  public:
    // Type aliases :scream:
    using this_t = LinearFunctionsProgramSignalGP<MEMORY_MODEL_T,TAG_T,INST_ARGUMENT_T,MATCHBIN_T,CUSTOM_COMPONENT_T>;
    using exec_state_t = lsgp_utils::ExecState<MEMORY_MODEL_T>;
    using call_state_t = typename exec_state_t::call_state_t;
    using flow_t = lsgp_utils::FlowType;
    using flow_info_t = lsgp_utils::FlowInfo;
    using flow_handler_t = lsgp_utils::FlowHandler<this_t, exec_state_t>;
    using tag_t = TAG_T;
    using arg_t = INST_ARGUMENT_T;
    using matchbin_t = MATCHBIN_T;
    using memory_model_t = MEMORY_MODEL_T;
    using memory_state_t = typename memory_model_t::memory_state_t;
    using program_t = sgp::LinearFunctionsProgram<tag_t, arg_t>;
    using base_hw_t = SignalGPBase<this_t, exec_state_t, tag_t, CUSTOM_COMPONENT_T>;
    using thread_t = typename base_hw_t::Thread;
    using event_lib_t = typename base_hw_t::event_lib_t; // EventLibrary<this_t>
    using event_t = typename base_hw_t::event_t;

    enum class InstProperty { BLOCK_CLOSE, BLOCK_DEF }; /// Instruction-definition properties.
    using inst_t = typename program_t::inst_t;
    using inst_lib_t = InstructionLibrary<this_t, inst_t, InstProperty>;
    using inst_prop_t = InstProperty;

    using fun_end_flow_t = typename flow_handler_t::fun_end_flow_t;
    using fun_open_flow_t = typename flow_handler_t::fun_open_flow_t;

  protected:
    inst_lib_t & inst_lib;
    flow_handler_t flow_handler;
    memory_model_t memory_model;
    program_t program;
    emp::Random & random;
    matchbin_t matchbin;
    bool is_matchbin_cache_dirty;
    std::function<void()> fun_clear_matchbin_cache = [this](){ this->ResetMatchBin(); }; // todo - can we do a better job baking this in?

    size_t max_call_depth;

    /// Setup default flow control functions for opening, closing, and breaking
    /// each type of control flow: BASIC, WHILE_LOOP, CALL, ROUTINE.
    // TODO
    void SetupDefaultFlowControl() {
      // --- BASIC Flow ---
      // On open:
      flow_handler[flow_t::BASIC].open_flow_fun =
        [](this_t & hw, exec_state_t & exec_state, const flow_info_t & new_flow) {
          emp_assert(exec_state.call_stack.size(), "Failed to open BASIC flow. No calls on call stack.");
          call_state_t & call_state = exec_state.call_stack.back();
          call_state.flow_stack.emplace_back(new_flow);
        };

      // On close
      // - Pop current flow from stack.
      // - Set new top of flow stack (if any)'s IP and MP to returning IP and MP.
      flow_handler[flow_t::BASIC].close_flow_fun =
        [](this_t & hw, exec_state_t & exec_state) {
          call_state_t & call_state = exec_state.call_stack.back();
          emp_assert(call_state.IsFlow(), "Failed to close BASIC flow. No flow to close.");
          const size_t ip = call_state.GetTopFlow().ip;
          const size_t mp = call_state.GetTopFlow().mp;
          call_state.flow_stack.pop_back();
          if (call_state.IsFlow()) {
            flow_info_t & top = call_state.GetTopFlow();
            top.ip = ip;
            top.mp = mp;
          }
        };

      // On break!
      flow_handler[flow_t::BASIC].break_flow_fun =
        [](this_t & hw, exec_state_t & exec_state) {
          emp_assert(exec_state.call_stack.size(), "Failed to break BASIC flow. No calls on call stack.");
          call_state_t & call_state = exec_state.call_stack.back();
          emp_assert(call_state.IsFlow(), "Failed to break BASIC flow. No flow to close.");
          const size_t flow_end = call_state.GetTopFlow().GetEnd();
          call_state.flow_stack.pop_back();
          if (call_state.IsFlow()) {
            call_state.SetIP(flow_end);
            if (hw.IsValidProgramPosition(call_state.GetMP(), call_state.GetIP())) {
              ++call_state.IP();
            }
          }
        };

      flow_handler[flow_t::WHILE_LOOP].open_flow_fun =
        [](this_t & hw, exec_state_t & exec_state, const flow_info_t & new_flow) {
          emp_assert(exec_state.call_stack.size(), "Failed to open WHILE_LOOP flow. No calls on call stack.");
          call_state_t & call_state = exec_state.call_stack.back();
          call_state.flow_stack.emplace_back(new_flow);
        };

      flow_handler[flow_t::WHILE_LOOP].close_flow_fun =
        [](this_t & hw, exec_state_t & exec_state) {
          emp_assert(exec_state.call_stack.size(), "Failed to close WHILE_LOOP flow. No calls on call stack.");
          // Move IP to start of block
          call_state_t & call_state = exec_state.call_stack.back();
          const size_t loop_begin = call_state.GetTopFlow().begin;
          const size_t mp = call_state.GetTopFlow().mp;
          call_state.flow_stack.pop_back();
          if (call_state.IsFlow()) {
            call_state.SetIP(loop_begin);
            call_state.SetMP(mp);
          }
        };

      flow_handler[flow_t::WHILE_LOOP].break_flow_fun =
        [](this_t & hw, exec_state_t & exec_state) {
          emp_assert(exec_state.call_stack.size(), "Failed to break WHILE_LOOP flow. No calls on call stack.");
          call_state_t & call_state = exec_state.call_stack.back();
          emp_assert(call_state.IsFlow(), "Failed to break WHILE_LOOP flow. No flow to close.");
          const size_t flow_end = call_state.GetTopFlow().GetEnd();
          call_state.flow_stack.pop_back();
          if (call_state.IsFlow()) {
            call_state.SetIP(flow_end);
            if (hw.IsValidProgramPosition(call_state.GetMP(), call_state.GetIP())) {
              ++call_state.IP();
            }
          }
        };

      flow_handler[flow_t::ROUTINE].open_flow_fun =
        [](this_t & hw, exec_state_t & exec_state, const flow_info_t & new_flow) {
          emp_assert(exec_state.call_stack.size(), "Failed to open ROUTINE flow. No calls on call stack.");
          call_state_t & call_state = exec_state.GetTopCallState();
          call_state.flow_stack.emplace_back(new_flow);
        };

      flow_handler[flow_t::ROUTINE].close_flow_fun =
        [](this_t & hw, exec_state_t & exec_state) {
          emp_assert(exec_state.call_stack.size(), "Failed to close ROUTINE flow. No calls on call stack.");
          call_state_t & call_state = exec_state.call_stack.back();
          emp_assert(call_state.IsFlow(), "Failed to break ROUTINE flow. No flow to close.");
          // Closing a ROUTINE flow:
          // - Pop flow from flow stack
          // - No need to pass IP and MP down (we want to return to previous IP/MP)
          call_state.flow_stack.pop_back();
        };

      // breaking from a routine is the same as closing a routine
      flow_handler[flow_t::ROUTINE].break_flow_fun =
        [](this_t & hw, exec_state_t & exec_state) {
          hw.GetFlowHandler().CloseFlow(hw, flow_t::ROUTINE, exec_state);
        };

      flow_handler[flow_t::CALL].open_flow_fun =
        [](this_t & hw, exec_state_t & exec_state, const flow_info_t & new_flow) {
          emp_assert(exec_state.call_stack.size(), "Failed to open CALL flow. No calls on call stack.");
          call_state_t & call_state = exec_state.call_stack.back();
          call_state.flow_stack.emplace_back(new_flow);
        };

      flow_handler[flow_t::CALL].close_flow_fun =
        [](this_t & hw, exec_state_t & exec_state) {
          emp_assert(exec_state.call_stack.size(), "Failed to close CALL flow. No calls on call stack.");
          call_state_t & call_state = exec_state.call_stack.back();
          emp_assert(call_state.IsFlow(), "Failed to close CALL flow. No flow to close.");
          // Closing a CALL flow:
          // - Pop call flow from flow stack.
          // - No need to pass IP and MP down (presumably, this was the bottom
          //   of the flow stack).
          if (call_state.IsCircular()) {
            flow_info_t & top = call_state.GetTopFlow();
            top.ip = top.begin;
          } else {
            call_state.GetFlowStack().pop_back();
          }
        };

      flow_handler[flow_t::CALL].break_flow_fun =
        [](this_t & hw, exec_state_t & exec_state) {
          hw.GetFlowHandler().CloseFlow(hw, flow_t::CALL, exec_state);
        };
    }

    /// Full hardware reset.
    void ResetImpl() {
      ResetProgram(); // this will reset program + hardware
    }

  public:
    LinearFunctionsProgramSignalGP(emp::Random & rnd, inst_lib_t & ilib, event_lib_t & elib)
      : base_hw_t(elib),
        inst_lib(ilib),
        random(rnd),
        matchbin(rnd),
        is_matchbin_cache_dirty(true),
        max_call_depth(256)
    {
      // Configure default flow control.
      SetupDefaultFlowControl();
    }

    LinearFunctionsProgramSignalGP(LinearFunctionsProgramSignalGP &&) = default;
    LinearFunctionsProgramSignalGP(const LinearFunctionsProgramSignalGP &) = default;

    /// Reset only hardware state information (memory, threads, etc)
    void ResetHardwareState() {
      emp_assert(!this->IsExecuting());
      this->ResetBaseHardwareState();
      memory_model.Reset();
    }

    /// Reset program. Requires that we reset the hardware state (if we don't,
    /// the hardware is likely to be in an illegal state).
    void ResetProgram() {
      emp_assert(!this->IsExecuting());
      ResetHardwareState();
      program.Clear();
      ResetMatchBin();
    }

    void ResetMatchBin() {
      matchbin.Clear();
      is_matchbin_cache_dirty = false;
      for (size_t i = 0; i < program.GetSize(); ++i) {
        matchbin.Set(i, program[i].GetTag(), i);
      }
    }

    bool IsValidProgramPosition(size_t mp, size_t ip) const {
      return program.IsValidPosition(mp, ip);
    }

    size_t GetNumModules() const {
      return program.GetSize();
    }

    /// Get a reference to a random number generator used by this hardware.
    emp::Random & GetRandom() { return random; }

    /// Get a reference to the hardware's flow handler.
    flow_handler_t & GetFlowHandler() { return flow_handler; }

    program_t & GetProgram() { return program; }
    const program_t & GetProgram() const { return program; }

    /// Get a reference to the hardware's memory model.
    memory_model_t & GetMemoryModel() { return memory_model; }
    const memory_model_t & GetMemoryModel() const { return memory_model; }

    /// Get a reference to the hardware's matchbin
    matchbin_t & GetMatchBin() { return matchbin; }
    const matchbin_t & GetMatchBin() const { return matchbin; }

    /// Set program for this hardware object.
    void SetProgram(const program_t & p) {
      this->Reset();   // Full hardware reset
      program = p;     // Update current program.
      ResetMatchBin(); // Update matchbin with current program information.
    }

    /// Set open flow handler for given flow type.
    void SetOpenFlowFun(flow_t type, const fun_open_flow_t & fun) {
      flow_handler[type].open_flow_fun = fun;
    }

    // Set close flow handler for a given flow type.
    void SetCloseFlowFun(flow_t type, const fun_end_flow_t & fun) {
      flow_handler[type].close_flow_fun = fun;
    }

    // Set break flow handler for a given flow type.
    void SetBreakFlowFun(flow_t type, const fun_end_flow_t & fun) {
      flow_handler[type].break_flow_fun = fun;
    }

    void SingleExecutionStep(this_t & hardware, thread_t & thread) {
      exec_state_t & exec_state = thread.GetExecState();
      // If there's a call state on the call stack, execute an instruction.
      while (exec_state.call_stack.size()) {
        // There's something on the call stack.
        call_state_t & call_state = exec_state.call_stack.back();
        // Is there anything on the flow stack?
        if (call_state.IsFlow()) {
          flow_info_t & flow_info = call_state.flow_stack.back();
          size_t mp = flow_info.mp;
          size_t ip = flow_info.ip;
          emp_assert(mp < GetNumModules(), "Invalid module pointer.", mp, GetNumModules());
          if (program.IsValidPosition(mp, ip)) {
            // NOTE - should we increment the IP before or after executing?
            // Only BEFORE executing an instruction do we have any guarantees about
            // the state of our flow info. After processing an instruction, this
            // flow info reference could be invalid. Our call state reference could
            // even be invalid. Thus, we must increment the IP before processing
            // the current instruction.
            ++flow_info.ip; // Move IP forward (maybe to an invalid location)
            inst_lib.ProcessInst(hardware, program[mp][ip]);
          } else { // @discussion if we wanted option to have modules be circular, we could add a condition before this else!
            // The IP is off the edge of the module.
            flow_handler.CloseFlow(hardware, flow_info.type, exec_state);
            continue;
          }
        } else {
          // No flow! Return.
          ReturnCall(exec_state);
          // should we continue here?
        }
        break; // We executed *something*, break from the loop.
      }
      // If the execution state's call stack is empty, mark this thread as dead.
      if (exec_state.call_stack.empty()) {
        thread.SetDead();
      }
    }

    /// Initialize a thread by calling given module (function) ID on it.
    void InitThread(thread_t & thread, size_t module_id) {
      emp_assert(module_id < program.GetSize(), "Invalid module_id.", module_id);
      exec_state_t & state = thread.GetExecState();
      if (state.call_stack.size()) { state.Clear(); } // reset the thread's call stack.
      emp_assert(state.call_stack.size() == 0);
      CallModule(module_id, state);
    }

    // InstPropertyBLOCK_CLOSEBLOCK_DEF
    size_t FindEndOfBlock(size_t mp, size_t ip) const {
      emp_assert(mp < program.GetSize(), "Invalid module id: ", mp);
      int depth = 1;
      while (true) {
        if (!IsValidProgramPosition(mp, ip)) break;
        const inst_t & inst = program[mp][ip];
        if (inst_lib.HasProperty(inst.id, InstProperty::BLOCK_DEF)) {
          ++depth;
        } else if (inst_lib.HasProperty(inst.id, InstProperty::BLOCK_CLOSE)) {
          --depth;
          if (depth == 0) break;
        }
        ++ip;
      }
      return ip;
    }

    /// Use matchbin to find the n matching modules to a given
    emp::vector<size_t> FindModuleMatch(const tag_t & tag, size_t n=1) {
      // find n matches
      if (is_matchbin_cache_dirty) {
        ResetMatchBin();
      }
      return matchbin.Match(tag, n);
    }

    void CallModule(const tag_t & tag, exec_state_t & exec_state, bool circular=false) {
      // Are we at max depth already?
      if (exec_state.call_stack.size() >= max_call_depth) return;
      // Find the best matching module!
      emp::vector<size_t> matches(FindModuleMatch(tag));
      if (matches.size()) {
        CallModule(matches[0], exec_state, circular);
      }
    }

    void CallModule(size_t module_id, exec_state_t & exec_state, bool circular=false) {
      emp_assert(module_id < program.GetSize());
      // Are we at max depth already?
      if (exec_state.call_stack.size() >= max_call_depth) return;
      if (program[module_id].GetSize() < 1) return;
      // Push new state to call stack.
      exec_state.call_stack.emplace_back(memory_model.CreateMemoryState(), circular);
      // note - flow info is different?
      // todo - double check that this FlowInfo is fine
      flow_handler.OpenFlow(*this, {flow_t::CALL, module_id, 0, 0, program[module_id].GetSize()}, exec_state);
      if (exec_state.call_stack.size() > 1) {
        call_state_t & caller_state = exec_state.call_stack[exec_state.call_stack.size() - 2];
        call_state_t & new_state = exec_state.call_stack.back();
        memory_model.OnModuleCall(caller_state.GetMemory(), new_state.GetMemory());
      }
    }

    void ReturnCall(exec_state_t & exec_state) {
      if (exec_state.call_stack.empty()) return; // Nothing to return from.
      // Get the current call state.
      call_state_t & returning_state = exec_state.call_stack.back();
      // Is there anything to return to?
      if (exec_state.call_stack.size() > 1) {
        // Yes! Copy the returning state's output memory into the caller state's local memory.
        call_state_t & caller_state = exec_state.call_stack[exec_state.call_stack.size() - 2];
        // @TODO - setup configurable memory return! (lambda)
        memory_model.OnModuleReturn(returning_state.GetMemory(), caller_state.GetMemory());
      }
      // Pop the returning state from call stack.
      exec_state.call_stack.pop_back();
    }

  };

}

#endif