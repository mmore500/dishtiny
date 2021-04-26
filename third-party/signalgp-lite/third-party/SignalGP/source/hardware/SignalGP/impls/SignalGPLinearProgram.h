#ifndef EMP_LINEAR_PROGRAM_SIGNALGP_H
#define EMP_LINEAR_PROGRAM_SIGNALGP_H

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
#include "../utils/LinearProgram.h"

namespace sgp {

  template<typename MEMORY_MODEL_T,
           typename TAG_T=emp::BitSet<16>,
           typename INST_ARGUMENT_T=int,
           typename MATCHBIN_T=emp::MatchBin< size_t,
                                              emp::HammingMetric<16>,
                                              emp::RankedSelector<>,
                                              emp::AdditiveCountdownRegulator<>
                                            >,
           typename CUSTOM_COMPONENT_T=sgp::DefaultCustomComponent>
  class LinearProgramSignalGP : public SignalGPBase<LinearProgramSignalGP<MEMORY_MODEL_T,TAG_T,INST_ARGUMENT_T,MATCHBIN_T,CUSTOM_COMPONENT_T>,
                                                    lsgp_utils::ExecState<MEMORY_MODEL_T>,
                                                    TAG_T,
                                                    CUSTOM_COMPONENT_T>
  {
  public:
    // Forward declarations.
    // struct ExecState;
    struct Module;
    // struct CallState;
    enum class InstProperty;

    // Type aliases.
    using this_t = LinearProgramSignalGP<MEMORY_MODEL_T,TAG_T,INST_ARGUMENT_T,MATCHBIN_T,CUSTOM_COMPONENT_T>;

    using exec_state_t = lsgp_utils::ExecState<MEMORY_MODEL_T>;
    using call_state_t = typename exec_state_t::call_state_t;
    using flow_t = lsgp_utils::FlowType;
    using flow_info_t = lsgp_utils::FlowInfo;
    using flow_handler_t = lsgp_utils::FlowHandler<this_t, exec_state_t>;

    // using exec_state_t = ExecState;
    using tag_t = TAG_T;
    using arg_t = INST_ARGUMENT_T;
    using module_t = Module;
    using matchbin_t = MATCHBIN_T;

    using memory_model_t = MEMORY_MODEL_T;
    using memory_state_t = typename memory_model_t::memory_state_t;

    using program_t = sgp::LinearProgram<tag_t, arg_t>;

    using base_hw_t = SignalGPBase<this_t, exec_state_t, tag_t, CUSTOM_COMPONENT_T>;
    using thread_t = typename base_hw_t::Thread;
    using event_lib_t = sgp::EventLibrary<this_t>;
    using event_t = typename base_hw_t::event_t;

    /// Blocks are within-module flow control segments (e.g., while loops, if statements, etc)
    enum class InstProperty { MODULE, BLOCK_CLOSE, BLOCK_DEF };
    using inst_t = typename program_t::inst_t;
    using inst_lib_t = InstructionLibrary<this_t, inst_t, InstProperty>;
    using inst_prop_t = InstProperty;

    // using fun_end_flow_t = std::function<void(this_t&, exec_state_t &)>;                   // note - pass hardware down?
    // using fun_open_flow_t = std::function<void(this_t&, exec_state_t &, const flow_info_t &)>;

    using fun_end_flow_t = typename flow_handler_t::fun_end_flow_t;
    using fun_open_flow_t = typename flow_handler_t::fun_open_flow_t;

    /// Module definition.
    struct Module {
      size_t id;      ///< Module ID. Used to call/reference module.
      size_t begin;   ///< First instruction in module (will be the module definition instruction).
      size_t end;     ///< The last instruction in the module.
      tag_t tag;      ///< Module tag. Used to call/reference module.
      std::unordered_set<size_t> in_module; ///< instruction positions belonging to this module.

      Module(size_t _id, size_t _begin=0, size_t _end=0, const tag_t & _tag=tag_t())
        : id(_id), begin(_begin), end(_end), tag(_tag), in_module() { ; }

      /// How many instructions are in this module?
      size_t GetSize() const { return in_module.size(); }

      /// What's our module id?
      size_t GetID() const { return id; }

      /// Get a mutable reference to the module's tag.
      tag_t & GetTag() { return tag; }

      /// Get a const reference to the module's tag.
      const tag_t & GetTag() const { return tag; }

      /// On which instruction does this module begin?
      size_t GetBegin() const { return begin; }

      /// On which instruction does this module end?
      size_t GetEnd() const { return end; }

      /// Returns whether or not a given instruction position within this module.
      bool InModule(size_t ip) const { return emp::Has(in_module, ip); }
    };

  protected:
    inst_lib_t & inst_lib;  ///< Library of program instructions.
    flow_handler_t flow_handler;       ///< The flow handler manages the behavior of different types of execution flow.
    memory_model_t memory_model;    ///< The memory model manages any global memory state and specifies call state memory.
    program_t program;              ///< Program loaded on this execution stepper.
    emp::vector<module_t> modules;  ///< List of modules in program.
    tag_t default_module_tag;       ///< What is the default tag to used for modules (in case the program doesn't specify)?

    emp::Random& random;            ///< Random number generator. (TODO - make this a smart pointer)

    matchbin_t matchbin;            ///< the match bin specifies how modules are referenced
    bool is_matchbin_cache_dirty;
    std::function<void()> fun_clear_matchbin_cache = [this](){ this->ResetMatchBin(); }; // todo - can we do a better job baking this in?

    size_t max_call_depth;          ///< Maximum size of a call stack.

    /// Setup default flow control functions for opening, closing, and breaking
    /// each type of control flow: BASIC, WHILE_LOOP, CALL, ROUTINE.
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
          emp_assert(exec_state.call_stack.size(), "Failed to close BASIC flow. No calls on call stack.");
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

    /// Full reset.
    void ResetImpl() {
      ResetHardwareState();
      ResetProgram();
    }

  public:
    LinearProgramSignalGP(emp::Random & rnd, inst_lib_t & ilib, event_lib_t & elib)
      : base_hw_t(elib),
        inst_lib(ilib),
        flow_handler(),
        memory_model(),
        program(),
        modules(),
        default_module_tag(),
        random(rnd),
        matchbin(rnd),
        is_matchbin_cache_dirty(true),
        max_call_depth(256)
    {
      // Configure default flow control
      SetupDefaultFlowControl();
    }

    LinearProgramSignalGP(LinearProgramSignalGP &&) = default;
    LinearProgramSignalGP(const LinearProgramSignalGP &) = default;

    /// Reset hardware state: memory model state.
    void ResetHardwareState() {
      this->ResetBaseHardwareState();
      memory_model.Reset(); // Reset global memory
    }

    /// Reset loaded program.
    void ResetProgram() {
      modules.clear(); // Clear modules.
      program.Clear(); // Clear program.
      ResetMatchBin(); // Reset matchbin.
    }

    /// Reset match bin.
    void ResetMatchBin() {
      matchbin.Clear();
      is_matchbin_cache_dirty = false;
      for (size_t i = 0; i < modules.size(); ++i) {
        matchbin.Set(i, modules[i].GetTag(), i);
      }
    }

    /// Return whether a given a module ID and an instruction position is a valid
    /// position in the program. I.e., mp is a valid module and ip is inside of
    /// module mp.
    bool IsValidProgramPosition(size_t mp, size_t ip) const {
      if (mp < modules.size()) {
        if (modules[mp].InModule(ip)) return true;
      }
      return false;
    }

    /// Advance given execution state on given hardware by a single step. I.e.,
    /// process a single instruction on this hardware.
    void SingleExecutionStep(this_t & hardware, thread_t & thread) {
      exec_state_t & exec_state = thread.GetExecState();
      // If there's a call state on the call stack, execute an instruction.
      while (exec_state.call_stack.size()) {
        // There's something on the call stack.
        call_state_t & call_state = exec_state.call_stack.back();
        // Is there anything on the flow stack?
        if (call_state.IsFlow()) {
          // std::cout << "- There's some flow." << std::endl;
          flow_info_t & flow_info = call_state.flow_stack.back();
          size_t mp = flow_info.mp;
          size_t ip = flow_info.ip;
          // std::cout << ">> MP=" << mp << "; IP=" << ip << std::endl;
          emp_assert(mp < GetNumModules(), "Invalid module pointer: ", mp);
          // Process current instruction (if any)!
          if (modules[mp].InModule(ip)) {
            // NOTE - should we increment the IP before or after executing?
            // Only BEFORE executing an instruction do we have any guarantees about
            // the state of our flow info. After processing an instruction, this
            // flow info reference could be invalid. Our call state reference could
            // even be invalid. Thus, we must increment the IP before processing
            // the current instruction.
            ++flow_info.ip; // Move instruction pointer forward (might be invalid location).
            inst_lib.ProcessInst(hardware, program[ip]);
          } else if (ip >= program.GetSize()
                    && modules[mp].InModule(0)
                    && modules[mp].end < modules[mp].begin) {
            // The instruction pointer is off the edge of the program.
            // HERE, we handle if this module wraps back to the beginning of the program.
            // in which case, we need to move the IP.
            ip = 0;
            flow_info.ip = 1; // See comment above for why we do this before ProcessInst.
            inst_lib.ProcessInst(hardware, program[ip]);
          } else {
            // IP not valid for this module. Close flow.
            flow_handler.CloseFlow(hardware, flow_info.type, exec_state);
            continue;
          }
        } else {
          // No flow!
          ReturnCall(exec_state);
        }
        break; // We executed *something*, break from loop.
      }
      // If execution state's call stack is empty, mark thread as dead.
      if (exec_state.call_stack.empty()) {
        thread.SetDead();
      }
    }

    /// Initialize thread by calling given module id on it.
    void InitThread(thread_t & thread, size_t module_id) {
      emp_assert(module_id < modules.size(), "Invalid module ID.");
      exec_state_t & state = thread.GetExecState();
      if (state.call_stack.size()) { state.Clear(); } /// Reset thread's call stack.
      CallModule(module_id, state);
    }

    /// Get reference to random number generator used by this hardware.
    emp::Random & GetRandom() { return random; }

    flow_handler_t & GetFlowHandler() { return flow_handler; }

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

    /// Find end of code block (i.e., internal flow control code segment).
    // @todo - test explicitly!
    size_t FindEndOfBlock(size_t mp, size_t ip) const {
      emp_assert(mp < modules.size(), "Invalid module!");
      int depth = 1;
      std::unordered_set<size_t> seen;
      while (true) {
        if (!IsValidProgramPosition(mp, ip)) break;
        const inst_t & inst = program[ip];
        if (inst_lib.HasProperty(inst.GetID(), InstProperty::BLOCK_DEF)) {
          ++depth;
        } else if (inst_lib.HasProperty(inst.GetID(), InstProperty::BLOCK_CLOSE)) {
          --depth;
          if (depth == 0) break;
        }
        seen.emplace(ip);
        ++ip;
        if (ip >= program.GetSize() && seen.size() < modules[mp].GetSize()) ip %= program.GetSize();
      }
      return ip;
    }

    /// Use the matchbin to find the n matching modules to a given tag.
    emp::vector<size_t> FindModuleMatch(const tag_t & tag, size_t n=1) {
      // Find n matches.
      if (is_matchbin_cache_dirty) {
        ResetMatchBin();
      }
      // no need to transform to values because we're using
      // matchbin uids equivalent to function uids
      return matchbin.Match(tag, n);
    }

    /// Call a module (specified by given tag) on the given execution state.
    void CallModule(const tag_t & tag, exec_state_t & exec_state, bool circular=false) {
      emp::vector<size_t> matches(FindModuleMatch(tag));
      if (matches.size()) {
        CallModule(matches[0], exec_state, circular);
      }
    }

    /// Call module specified directly by module_id on the given execution state.
    void CallModule(size_t module_id, exec_state_t & exec_state, bool circular=false) {
      emp_assert(module_id < modules.size());
      if (exec_state.call_stack.size() >= max_call_depth) return;
      // Push new state onto stack.
      exec_state.call_stack.emplace_back(memory_model.CreateMemoryState(), circular);
      module_t & module_info = modules[module_id];
      flow_handler.OpenFlow(*this, {flow_t::CALL, module_id, module_info.begin, module_info.begin, module_info.end}, exec_state);
      if (exec_state.call_stack.size() > 1) {
        call_state_t & caller_state = exec_state.call_stack[exec_state.call_stack.size() - 2];
        call_state_t & new_state = exec_state.call_stack.back();
        memory_model.OnModuleCall(caller_state.GetMemory(), new_state.GetMemory());
      }
    }

    // todo - test!
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

    /// Set program for this hardware object.
    /// After updating hardware's program, 'compile' the program to extract module
    /// information (i.e., run UpdateModules).
    void SetProgram(const program_t & _program) {
      this->Reset();
      program = _program;
      UpdateModules();
    }

    /// Configure the default module tag. Assigned to default module if a loaded
    /// program has no module definition in it.
    void SetDefaultTag(const tag_t & _tag) { default_module_tag = _tag; }

    /// Analyze program and extract module information. Use to update modules
    /// vector.
    /// @todo - check to see if this works
    void UpdateModules() {
      // std::cout << "Update modules!" << std::endl;
      // Clear out the current modules.
      modules.clear();
      // Do nothing if there aren't any instructions to look at.
      if (!program.GetSize()) return;
      // Scan program for module definitions.
      std::unordered_set<size_t> dangling_instructions;
      for (size_t pos = 0; pos < program.GetSize(); ++pos) {
        inst_t & inst = program[pos];
        // Is this a module definition?
        if (inst_lib.HasProperty(inst.GetID(), InstProperty::MODULE)) {
          // If this isn't the first module we've found, mark this position as the
          // last position of the previous module.
          if (modules.size()) { modules.back().end = pos; }
          emp_assert(inst.GetTags().size(), "MODULE-defining instructions must have tag arguments to be used with this execution stepper.");
          const size_t mod_id = modules.size(); // Module ID for new module.
          modules.emplace_back(mod_id, ( (pos+1) < program.GetSize() ) ? pos+1 : 0, -1, inst.GetTags()[0]);
        } else {
          // We didn't find a new module. Track which module this instruction belongs to:
          // - If we've found a module, add it to the current module.
          // - If we haven't found a module, note that this instruction is dangling.
          if (modules.size()) { modules.back().in_module.emplace(pos); }
          else { dangling_instructions.emplace(pos); }
        }
      }
      // At this point, we know about all of the modules (if any).
      // First, we need to set the end point for the last module we found.
      if (modules.size()) {
        // If the first module begins at the beginning of the instruction, the last
        // module must end at the end of the program.
        // Otherwise, the last module ends where the first module begins.
        // if (modules[0].begin == 0) modules.back().end = program.GetSize();
        // else
        modules.back().end = (modules[0].begin - 1 > 0) ? modules[0].begin - 1 : program.GetSize();
      } else {
        // Found no modules. Add a default module that starts at the beginning and
        // ends at the end.
        modules.emplace_back(0, 0, program.GetSize(), default_module_tag);
      }
      // Now, we need to take care of the dangling instructions.
      // - We're going to assume the program is circular, so dangling instructions
      //   belong to the last module we found.
      for (size_t val : dangling_instructions) modules.back().in_module.emplace(val);
      // Reset matchbin
      ResetMatchBin();
    }

    /// Get a reference to the set of known modules.
    emp::vector<module_t> & GetModules() { return modules;  }

    /// Get a reference to a particular module. Requested module must be a valid
    /// module id.
    module_t & GetModule(size_t i) { emp_assert(i < modules.size()); return modules[i]; }

    /// How many modules does the current program have?
    size_t GetNumModules() const { return modules.size(); }

    /// Grab a reference to the current program.
    program_t & GetProgram() { return program; }

    /// Get a const reference to the current program.
    const program_t & GetProgram() const { return program; }

    /// Get a reference to the hardware's memory model.
    memory_model_t & GetMemoryModel() { return memory_model; }

    /// Print information on loaded modules.
    void PrintModules(std::ostream & os=std::cout) const {
      os << "Modules: [";
      for (size_t i = 0; i < modules.size(); ++i) {
        if (i) os << ",";
        os << "{id:" << modules[i].id << ", begin:" << modules[i].begin << ", end:" << modules[i].end << ", tag:" << modules[i].tag << "}";
      }
      os << "]";
    }

    /// Print a given execution state.
    void PrintExecutionState(const exec_state_t & state, std::ostream & os=std::cout) const {
      // -- Call stack --
      // todo
      os << "Call stack (" << state.call_stack.size() << "):\n";
      os << "------ TOP ------\n";
      for (auto it = state.call_stack.rbegin(); it != state.call_stack.rend(); ++it) {
        const call_state_t & call_state = *it;
        const size_t num_flow = call_state.flow_stack.size();
        // os << "--- CALL ---\n";
        memory_model.PrintMemoryState(call_state.memory, os);
        os << "Flow Stack:\n";
        for (size_t i = 0; i < num_flow; ++i) {
          // if (i) os << "---\n";
          const flow_info_t & flow = call_state.flow_stack[num_flow - 1 - i];
          // MP, IP, ...
          // type, mp, ip, begin, end
          // todo - print full flow stack!
          os << "  Flow: {mp:" << flow.mp
              << ", ip:" << flow.ip
              << ", flow-begin:" << flow.begin
              << ", flow-end:" << flow.end
              << ", flow-type:" << flow_handler.FlowTypeToString(flow.type)
              << "}; ";
          // if is valid instruction
          os << "Instruction: ";
          if (IsValidProgramPosition(flow.mp, flow.ip)) {
            // Name[tags](args)
            const inst_t & inst = program[flow.ip];
            os << inst_lib.GetName(inst.id);
            os << "[";
            for (size_t ti = 0; ti < inst.tags.size(); ++ti) {
              if (ti) os << ",";
              os << inst.tags[ti];
            }
            os << "](";
            for (size_t ai = 0; ai < inst.args.size(); ++ai) {
              if (ai) os << ",";
              os << inst.args[ai];
            }
            os << ")\n";
          } else {
            os << "NONE\n";
          }
        }
        os << "---\n";
      }
      os << "-----------------";
    }
  };
}

#endif