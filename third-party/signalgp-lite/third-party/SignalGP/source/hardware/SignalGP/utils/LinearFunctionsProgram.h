#ifndef EMP_SIGNALGP_LINEAR_FUNCTIONS_PROGRAM_H
#define EMP_SIGNALGP_LINEAR_FUNCTIONS_PROGRAM_H

#include <iostream>
#include <utility>
#include <tuple>

#include "emp/base/Ptr.hpp"
#include "emp/base/vector.hpp"
#include "emp/math/Random.hpp"
#include "emp/matching/MatchBin.hpp"
#include "emp/matching/matchbin_utils.hpp"
#include "emp/math/Range.hpp"

#include "../EventLibrary.h"
#include "InstructionLibrary.h"
#include "LinearProgram.h"

namespace sgp {

  template<typename TAG_T, typename ARGUMENT_T>
  class LinearFunction {
  public:
    using tag_t = TAG_T;
    using arg_t = ARGUMENT_T;
    using sequence_t = LinearProgram<tag_t, arg_t>;
    using inst_t = typename sequence_t::inst_t;

  protected:
    emp::vector<tag_t> tags;      // @discussion Would there be a reason a function would want multiple tags?
    sequence_t inst_sequence;

  public:
    LinearFunction(const emp::vector<tag_t> & _tags,
                   const sequence_t & _inst_sequence=sequence_t())
      : tags(_tags), inst_sequence(_inst_sequence)
    {
      emp_assert(tags.size(), "A function MUST have at least one tag.");
    }

    LinearFunction(const tag_t & _tag=tag_t(),
                   const sequence_t & _inst_sequence=sequence_t())
      : tags({_tag}), inst_sequence(_inst_sequence) { ; }

    inst_t & operator[](size_t id) { return inst_sequence[id]; }
    const inst_t & operator[](size_t id) const { return inst_sequence[id]; }

    bool operator==(const LinearFunction & other) const {
      return std::tie(inst_sequence, tags) == std::tie(other.inst_sequence, other.tags);
    }

    bool operator!=(const LinearFunction & other) const {
      return !(*this == other);
    }

    /// Allows ordering to be imposed on LinearFunction objects.
    bool operator<(const LinearFunction & other) const {
      return std::tie(inst_sequence, tags) < std::tie(other.inst_sequence, other.tags);
    }

    size_t GetSize() const { return inst_sequence.GetSize(); }

    tag_t & GetTag(size_t id=0) {  emp_assert(tags.size()); return tags[id]; }
    const tag_t & GetTag(size_t id=0) const {  emp_assert(tags.size()); return tags[id]; }

    emp::vector<tag_t> & GetTags() { return tags; } // WARNING: no safety features here!
    const emp::vector<tag_t> & GetTags() const { return tags; } // WARNING: no safety features here!

    void SetTag(const tag_t & tag) { SetTag(0, tag); }
    void SetTag(size_t id, const tag_t & tag) { emp_assert(id < tags.size()); tags[id] = tag; }

    sequence_t & GetInstSequence() { return inst_sequence; }
    const sequence_t & GetInstSequence() const { return inst_sequence; }

    void SetInstSequence(const sequence_t & seq) { inst_sequence = seq; }

    bool IsValidPosition(size_t pos) const { return inst_sequence.IsValidPosition(pos); }

    // expose program-manipulating instructions

    /// Push instruction to instruction set.
    /// - No validation! We're trusting that 'id' is legit!
    void PushInst(size_t id,
                  const emp::vector<arg_t> & args=emp::vector<arg_t>(),
                  const emp::vector<tag_t> & tags=emp::vector<tag_t>()) {
      inst_sequence.PushInst(id, args, tags);
    }

    /// Push instruction to program by name.
    template<typename HARDWARE_T, typename INST_PROPERTY_T>
    void PushInst(const InstructionLibrary<HARDWARE_T, inst_t, INST_PROPERTY_T> & ilib,
                  const std::string & name,
                  const emp::vector<arg_t> & args=emp::vector<arg_t>(),
                  const emp::vector<tag_t> & tags=emp::vector<tag_t>()) {
      emp_assert(ilib.IsInst(name), "Uknown instruction name", name);
      inst_sequence.PushInst(ilib.GetID(name), args, tags);
    }

    /// Push instruction to program.
    void PushInst(const inst_t & inst) { inst_sequence.PushInst(inst); }

    /// Is the given instruction valid?
    template<typename HARDWARE_T, typename INST_PROPERTY_T>
    bool IsValidInst(const InstructionLibrary<HARDWARE_T, inst_t, INST_PROPERTY_T> & ilib,
                     const inst_t & inst) {
      return inst_sequence.IsValidInst(ilib, inst);
    }

    template<typename HARDWARE_T, typename INST_PROPERTY_T>
    void Print(std::ostream& out, const InstructionLibrary<HARDWARE_T, inst_t, INST_PROPERTY_T>& ilib) const{
      inst_sequence.Print(out, ilib);
    }
  };

  template<typename TAG_T, typename ARGUMENT_T=int>
  class LinearFunctionsProgram {
  public:
    using this_t = LinearFunctionsProgram<TAG_T, ARGUMENT_T>;
    using tag_t = TAG_T;
    using arg_t = ARGUMENT_T;
    using inst_t = typename LinearProgram<tag_t, arg_t>::Instruction;
    using function_t = LinearFunction<tag_t, arg_t>;

  protected:
    emp::vector<function_t> program;

  public:
    LinearFunctionsProgram(const emp::vector<function_t> & fseq=emp::vector<function_t>())
      : program(fseq) { }
    LinearFunctionsProgram(const this_t &) = default;

    bool operator==(const this_t & other) const { return program == other.program; }
    bool operator!=(const this_t & other) const { return !(*this == other); }
    bool operator<(const this_t & other) const { return program < other.program; }

    /// Allow program's function set to be indexed as if a vector.
    function_t & operator[](size_t id) {
      emp_assert(id < program.size());
      return program[id];
    }

    /// Allow program's function set to be indexed as if a vector.
    const function_t & operator[](size_t id) const {
      emp_assert(id < program.size());
      return program[id];
    }

    /// Clear program's functions.
    void Clear() { program.clear(); }

    /// Get number of functions that make up this program.
    size_t GetSize() const { return program.size(); }

    size_t GetInstCount() const {
      size_t cnt = 0;
      for (size_t fID = 0; fID < program.size(); ++fID)
        cnt += program[fID].GetSize();
      return cnt;
    }

    /// Is this a valid position?
    bool IsValidPosition(size_t func_id, size_t inst_id) const {
      if (func_id >= program.size()) return false;
      return program[func_id].IsValidPosition(inst_id);
    }

    bool IsValidFunction(size_t func_id) { return func_id < program.size(); }

    void SetProgram(const emp::vector<function_t> & p) { program = p; }

    /// Pop last function off program.
    void PopFunction() {
      if (program.size()) {
        program.pop_back();
      }
    }

    /// Push new function into program. New function will be a copy given function.
    void PushFunction(const function_t & func) { program.emplace_back(func); }

    /// Push new function into program. Construct function from given tag and from
    /// given LinearProgram object.
    void PushFunction(const tag_t & tag, const LinearProgram<tag_t, arg_t> & seq=LinearProgram<tag_t, arg_t>()) {
      program.emplace_back(tag, seq);
    }

    /// Push new function into program. Construct function from given vector of tags
    /// and from given LinearProgram object.
    void PushFunction(const emp::vector<tag_t> & tags, const LinearProgram<tag_t, arg_t> & seq=LinearProgram<tag_t, arg_t>()) {
      program.emplace_back(tags, seq);
    }

    /// Push instruction to end of function specified by function_id.
    /// Requires that 'function_id' specifies a valid function.
    /// Instruction is specified by arguments: 'inst_id', 'args' and 'tags'.
    void PushInst(size_t function_id,
                  size_t inst_id,
                  const emp::vector<arg_t> & args=emp::vector<arg_t>(),
                  const emp::vector<tag_t> & tags=emp::vector<tag_t>()) {
      emp_assert(IsValidFunction(function_id), "Invalid function_id", function_id);
      program[function_id].PushInst(inst_id, args, tags);
    }

    /// Push instruction to end of last function in program.
    /// If no functions exist yet, create one.
    /// Instruction is specified by arguments: 'inst_id', 'args' and 'tags'.
    void PushInst(size_t inst_id,
                  const emp::vector<arg_t> & args=emp::vector<arg_t>(),
                  const emp::vector<tag_t> & tags=emp::vector<tag_t>())
    {
      if (program.empty()) program.emplace_back();
      const size_t fp = program.size() - 1;
      PushInst(fp, inst_id, args, tags);
    }

    template<typename HARDWARE_T, typename INST_PROPERTY_T>
    void PushInst(size_t function_id,
                  const InstructionLibrary<HARDWARE_T, inst_t, INST_PROPERTY_T> & ilib,
                  const std::string & name,
                  const emp::vector<arg_t> & args=emp::vector<arg_t>(),
                  const emp::vector<tag_t> & tags=emp::vector<tag_t>())
    {
      emp_assert(IsValidFunction(function_id), "Invalid function_id", function_id);
      program[function_id].PushInst(ilib, name, args, tags);
    }

    template<typename HARDWARE_T, typename INST_PROPERTY_T>
    void PushInst(const InstructionLibrary<HARDWARE_T, inst_t, INST_PROPERTY_T> & ilib,
                  const std::string & name,
                  const emp::vector<arg_t> & args=emp::vector<arg_t>(),
                  const emp::vector<tag_t> & tags=emp::vector<tag_t>())
    {
      if (program.empty()) program.emplace_back();
      const size_t fp = program.size() - 1;
      PushInst(fp, ilib, name, args, tags);
    }

    void PushInst(size_t function_id, const inst_t & inst) {
      emp_assert(IsValidFunction(function_id), "Invalid function_id", function_id);
      program[function_id].PushInst(inst);
    }

    void PushInst(const inst_t & inst) {
      if (program.empty()) program.emplace_back();
      const size_t fp = program.size() - 1;
      PushInst(fp, inst);
    }

    // todo - various load from string functions (configurable?)

    // todo - support tabbing/levels for block type instructions
    // todo - easier to read print without tags

    // Full print for a program. Prints all tags, functions, instructions and args.
    // Prints each tag on a new line followed by the function number.
    template<typename HARDWARE_T, typename INST_PROPERTY_T>
    void Print(std::ostream& out, const InstructionLibrary<HARDWARE_T, inst_t, INST_PROPERTY_T>& ilib) const{
      for (size_t i = 0; i < GetSize(); ++i){
        // Skip the last tag so we dont get an extra delimeter
        std::copy(program[i].GetTags().begin(), program[i].GetTags().end() - 1, std::ostream_iterator<tag_t>(out, "\n"));
        out << program[i].GetTags().back() << " Fn-"<< i << "\n";
        program[i].Print(out, ilib);
        out << "\n";
      }
    }
  };

  // GenRandLinearFunction => {RandomBitSet, GenRandLinearProgram}
  // todo - clean up arguments
  template<typename HARDWARE_T, size_t TAG_WIDTH>
  LinearFunction<emp::BitSet<TAG_WIDTH>, int> GenRandLinearFunction(
    emp::Random & rnd,
    const InstructionLibrary<HARDWARE_T,
                             typename LinearProgram< emp::BitSet<TAG_WIDTH>, int>::Instruction,
                             typename HARDWARE_T::inst_prop_t> & inst_lib,
    size_t num_func_tags=1,
    const emp::Range<size_t> & inst_cnt_range={1, 32},
    size_t num_inst_tags=1,
    size_t num_inst_args=3,
    const emp::Range<int> & arg_val_range={0, 15}
  ) {
    return {emp::RandomBitSets<TAG_WIDTH>(rnd, num_func_tags),
            GenRandLinearProgram<HARDWARE_T,TAG_WIDTH>(rnd, inst_lib,
                                                       inst_cnt_range,
                                                       num_inst_tags, num_inst_args,
                                                       arg_val_range)};
  }

  // Todo - DEAR GOD THESE ARGUMENTS ARE TERRIBLE
  template<typename HARDWARE_T, size_t TAG_WIDTH>
  LinearFunctionsProgram<emp::BitSet<TAG_WIDTH>, int> GenRandLinearFunctionsProgram(
    emp::Random & rnd,
    const InstructionLibrary<HARDWARE_T,
                             typename LinearProgram< emp::BitSet<TAG_WIDTH>, int>::Instruction,
                             typename HARDWARE_T::inst_prop_t> & inst_lib,
    const emp::Range<size_t> & num_func_range={1, 4},
    size_t num_func_tags=1,
    const emp::Range<size_t> & func_inst_cnt_range={1, 32},
    size_t num_inst_tags=1,
    size_t num_inst_args=3,
    const emp::Range<int> & arg_val_range={0, 15}
  ) {
    const size_t func_cnt = rnd.GetUInt(num_func_range.GetLower(), num_func_range.GetUpper()+1);
    LinearFunctionsProgram<emp::BitSet<TAG_WIDTH>, int> new_program;
    for (size_t fID = 0; fID < func_cnt; ++fID) {
      new_program.PushFunction(GenRandLinearFunction<HARDWARE_T, TAG_WIDTH>(rnd, inst_lib,
                                                                            num_func_tags,
                                                                            func_inst_cnt_range,
                                                                            num_inst_tags,
                                                                            num_inst_args,
                                                                            arg_val_range));
    }
    return new_program;
  }
}

#endif
