#ifndef EMP_SIGNALGP_V2_MEMORY_MODEL_H
#define EMP_SIGNALGP_V2_MEMORY_MODEL_H

#include <iostream>
#include <utility>

#include "emp/base/Ptr.hpp"
#include "emp/base/vector.hpp"
#include "emp/math/Random.hpp"
#include "emp/matching/MatchBin.hpp"
#include "emp/matching/matchbin_utils.hpp"

namespace sgp {

  // TODO - make on return/on call re-configurable
  // Demonstrative memory model based on original version of SignalGP.
  class SimpleMemoryModel {
  public:
    struct SimpleMemoryState;
    using memory_state_t = SimpleMemoryState;
    using mem_buffer_t = std::unordered_map<int, double>;

    /// SimpleMemoryModel's memory state struct.
    /// - Consists of: working, input, and output memory buffers.
    struct SimpleMemoryState {
      mem_buffer_t working_mem;      // Working memory buffer!
      mem_buffer_t input_mem;        // Input memory buffer!
      mem_buffer_t output_mem;       // Output memory buffer!

      SimpleMemoryState(const mem_buffer_t & w=mem_buffer_t(),
                        const mem_buffer_t & i=mem_buffer_t(),
                        const mem_buffer_t & o=mem_buffer_t())
        : working_mem(w), input_mem(i), output_mem(o) { ; }
      SimpleMemoryState(const SimpleMemoryState &) = default;
      SimpleMemoryState(SimpleMemoryState &&) = default;

      /// Set value at given key in working memory. No questions asked.
      void SetWorking(int key, double value) { working_mem[key] = value;  }

      /// Set value at given key in input memory. No questions asked.
      void SetInput(int key, double value) { input_mem[key] = value; }

      /// Set value at given key in output memory. No questions asked.
      void SetOutput(int key, double value) { output_mem[key] = value; }

      /// Get a reference to value at particular key in working memory. If key
      /// not yet in buffer, add key w/value of 0.
      double & AccessWorking(int key) {
        if (!emp::Has(working_mem, key)) working_mem[key] = 0;
        return working_mem[key];
      }

      /// Get a reference to value at particular key in input memory. If key
      /// not yet in buffer, add key w/value of 0.
      double & AccessInput(int key) {
        if (!emp::Has(input_mem, key)) input_mem[key] = 0;
        return input_mem[key];
      }

      /// Get a reference to value at particular key in output memory. If key
      /// not yet in buffer, add key w/value of 0.
      double & AccessOutput(int key) {
        if (!emp::Has(output_mem, key)) output_mem[key] = 0;
        return output_mem[key];
      }

      double GetWorking(int key) { return emp::Find(working_mem, key, 0.0); }
      double GetInput(int key) { return emp::Find(input_mem, key, 0.0); }
      double GetOutput(int key) { return emp::Find(output_mem, key, 0.0); }

      mem_buffer_t & GetWorkingMemory() { return working_mem; }
      const mem_buffer_t & GetWorkingMemory() const { return working_mem; }
      mem_buffer_t & GetInputMemory() { return input_mem; }
      const mem_buffer_t & GetInputMemory() const { return input_mem; }
      mem_buffer_t & GetOutputMemory() { return output_mem; }
      const mem_buffer_t & GetOutputMemory() const { return output_mem; }
    };

  protected:
    mem_buffer_t global_mem=mem_buffer_t(); /// 'Global memory' buffer.

  public:

    SimpleMemoryState CreateMemoryState(const mem_buffer_t & working=mem_buffer_t(),
                                        const mem_buffer_t & input=mem_buffer_t(),
                                        const mem_buffer_t & output=mem_buffer_t())
    { return {working, input, output}; }

    /// Reset memory model state.
    void Reset() {
      global_mem.clear();
    }

    /// Print a single memory buffer.
    void PrintMemoryBuffer(const mem_buffer_t & buffer, std::ostream & os=std::cout) const {
      os << "[";
      bool comma = false;
      for (auto mem : buffer) {
        if (comma) os << ", ";
        os << "{" << mem.first << ":" << mem.second << "}";
        comma = true;
      }
      os << "]";
    }

    /// Print the state of memory.
    void PrintMemoryState(const memory_state_t & state, std::ostream & os=std::cout) const {
      os << "Working memory (" << state.working_mem.size() << "): ";
      PrintMemoryBuffer(state.working_mem, os);
      os << "\n";
      os << "Input memory (" << state.input_mem.size() << "): ";
      PrintMemoryBuffer(state.input_mem, os);
      os << "\n";
      os << "Output memory (" << state.output_mem.size() << "): ";
      PrintMemoryBuffer(state.output_mem, os);
      os << "\n";
    }

    void PrintState(std::ostream & os=std::cout) const {
      os << "Global memory (" << global_mem.size() << "): ";
      PrintMemoryBuffer(global_mem, os);
    }

    mem_buffer_t & GetGlobalBuffer() { return global_mem; }
    const mem_buffer_t & GetGlobalBuffer() const { return global_mem; }

    void SetGlobal(int key, double val) { global_mem[key] = val; }

    double GetGlobal(int key) { return emp::Find(global_mem, key, 0.0); }

    double & AccessGlobal(int key) {
      if (!emp::Has(global_mem, key)) global_mem[key] = 0;
      return global_mem[key];
    }

    void OnModuleCall(memory_state_t & caller_mem, memory_state_t & callee_mem) {
      for (auto & mem : caller_mem.working_mem) {
        callee_mem.SetInput(mem.first, mem.second);
      }
    }

    // Handle Module return
    void OnModuleReturn(memory_state_t & returning_mem, memory_state_t & caller_mem) {
      for (auto & mem : returning_mem.output_mem) {
        caller_mem.SetWorking(mem.first, mem.second);
      }
    }

  };

}

#endif