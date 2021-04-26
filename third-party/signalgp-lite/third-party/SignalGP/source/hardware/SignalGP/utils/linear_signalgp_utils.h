#ifndef EMP_LINEAR_SIGNALGP_UTILS
#define EMP_LINEAR_SIGNALGP_UTILS

#include <iostream>
#include <utility>
#include <memory>

#include "emp/base/Ptr.hpp"
#include "emp/base/vector.hpp"
#include "emp/math/Random.hpp"
#include "emp/matching/MatchBin.hpp"
#include "emp/matching/matchbin_utils.hpp"

#include "../EventLibrary.h"
#include "InstructionLibrary.h"

namespace sgp { namespace lsgp_utils {


  /// Library of flow types.
  /// e.g., WHILE, IF, ROUTINE, et cetera
  /// NOTE - I'm not sure that I'm a fan of how this is organized/named/setup.
  /// BASIC: if statements (for now)
  enum class FlowType : size_t { BASIC, WHILE_LOOP, ROUTINE, CALL };

  /// Everything the execution stepper needs to know to manage (open, close, break)
  /// any of the execution flow types.
  struct FlowInfo {
    FlowType type;    ///< Flow type ID?
    size_t mp;        ///< Module pointer. Which module is being executed?
    size_t ip;        ///< Instruction pointer. Which instruction is executed?
    size_t begin;     ///< Where does the flow begin?
    size_t end;       ///< Where does the flow end?

    FlowInfo(FlowType _type, size_t _mp=(size_t)-1, size_t _ip=(size_t)-1,
              size_t _begin=(size_t)-1, size_t _end=(size_t)-1)
      : type(_type), mp(_mp), ip(_ip), begin(_begin), end(_end) { ; }

    size_t GetBegin() const { return begin; }
    size_t GetEnd() const { return end; }
    size_t GetMP() const { return mp; }
    size_t GetIP() const { return ip; }
    FlowType GetType() const { return type; }
    bool IsBasic() const { return type == FlowType::BASIC; }
    bool IsWhileLoop() const { return type == FlowType::WHILE_LOOP; }
    bool IsRoutine() const { return type == FlowType::ROUTINE; }
    bool IsCall() const { return type == FlowType::CALL; }
  };

  // NEED: hardware_t, flow_t, exec_state_t,
  template<typename HARDWARE_T, typename EXEC_STATE_T>
  struct FlowHandler {
    using exec_state_t = EXEC_STATE_T;
    using hardware_t = HARDWARE_T;
    using fun_end_flow_t = std::function<void(hardware_t&, exec_state_t &)>;
    using fun_open_flow_t  = std::function<void(hardware_t&, exec_state_t &, const FlowInfo &)>;

    struct FlowControl {
      fun_open_flow_t open_flow_fun;
      fun_end_flow_t close_flow_fun;
      fun_end_flow_t break_flow_fun;
    };

    /// Mapping from flow type to flow control structure.
    // todo - could change this to an array because we know things at compile time
    std::map<FlowType, FlowControl> lib = {{FlowType::BASIC, FlowControl()},
                                           {FlowType::WHILE_LOOP, FlowControl()},
                                           {FlowType::ROUTINE, FlowControl()},
                                           {FlowType::CALL, FlowControl()} };

    FlowControl & operator[](FlowType type) {
      emp_assert(emp::Has(lib, type), "FlowType not recognized!");
      return lib[type];
    }

    const FlowControl & operator[](FlowType type) const {
      emp_assert(emp::Has(lib, type), "FlowType not recognized!");
      return lib[type];
    }

    std::string FlowTypeToString(FlowType type) const {
      switch (type) {
        case FlowType::BASIC: return "BASIC";
        case FlowType::WHILE_LOOP: return "WHILE_LOOP";
        case FlowType::ROUTINE: return "ROUTINE";
        case FlowType::CALL: return "CALL";
        default: return "UNKNOWN";
      }
    }

    void OpenFlow(hardware_t & hw, const FlowInfo & new_flow, exec_state_t & state) {
      FlowType type = new_flow.type;
      emp_assert(emp::Has(lib, type), "FlowType not recognized!");
      lib[type].open_flow_fun(hw, state, new_flow);
    }

    void CloseFlow(hardware_t & hw, FlowType type, exec_state_t & state) {
      emp_assert(emp::Has(lib, type), "FlowType not recognized!");
      lib[type].close_flow_fun(hw, state);
    }

    void BreakFlow(hardware_t & hw, FlowType type, exec_state_t & state) {
      emp_assert(emp::Has(lib, type), "FlowType not recognized!");
      lib[type].break_flow_fun(hw, state);
    }
  };

  /// State information for a function call.
  template<typename MEMORY_STATE_T>
  struct CallState {
    MEMORY_STATE_T memory;            ///< Memory local to this call state.
    emp::vector<FlowInfo> flow_stack; ///< Stack of 'Flow' (a stack of fancy read heads)
    bool circular;                    ///< Should call wrap when IP goes off end? Or, implicitly return?

    CallState(const MEMORY_STATE_T & _mem=MEMORY_STATE_T(), bool _circular=false)
      : memory(_mem), flow_stack(), circular(_circular) { ; }

    bool IsFlow() const { return !flow_stack.empty(); }

    emp::vector<FlowInfo> & GetFlowStack() { return flow_stack; }

    FlowInfo & GetTopFlow() {
      emp_assert(flow_stack.size());
      return flow_stack.back();
    }

    const FlowInfo & GetTopFlow() const {
      emp_assert(flow_stack.size());
      return flow_stack.back();
    }

    bool IsCircular() const { return circular; }

    MEMORY_STATE_T & GetMemory() { return memory; }

    // --- For your convenience shortcuts: ---
    /// Set the instruction pointer of the 'flow' at the top of the flow stack.
    void SetIP(size_t i) { emp_assert(flow_stack.size()); flow_stack.back().ip = i; }

    /// Set the module pointer at the top of the flow stack.
    void SetMP(size_t m) { emp_assert(flow_stack.size()); flow_stack.back().mp = m; }

    /// Get the instruction pointer at the top of the flow stack.
    size_t GetIP() const { emp_assert(flow_stack.size()); return flow_stack.back().ip; }

    /// Get the module pointer at the top of the flow stack.
    size_t GetMP() const { emp_assert(flow_stack.size()); return flow_stack.back().mp; }

    /// Get a mutable reference to the instruction pointer from the top of the flow stack.
    size_t & IP() { emp_assert(flow_stack.size()); return flow_stack.back().ip; }

    /// Get a mutable reference to the module pointer from the top of the flow stack.
    size_t & MP() { emp_assert(flow_stack.size()); return flow_stack.back().mp; }
  };

  /// Execution State. TODO - add label?
  template<typename MEMORY_MODEL_T>
  struct ExecState {
    using memory_state_t = typename MEMORY_MODEL_T::memory_state_t;
    using call_state_t = CallState<memory_state_t>;
    emp::vector<call_state_t> call_stack;   ///< Program call stack.

    /// Empty out the call stack.
    void Clear() { call_stack.clear(); }
    void Reset() { call_stack.clear(); }

    /// Get a reference to the current (top) call state on the call stack.
    /// Requires the call stack to be not empty.
    call_state_t & GetTopCallState() {
      emp_assert(call_stack.size(), "Cannot get top call state from empty call stack.");
      return call_stack.back();
    }

    /// Get a mutable reference to the entire call stack.
    emp::vector<call_state_t> & GetCallStack() { return call_stack; }
  };

}}

#endif