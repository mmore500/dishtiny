#ifndef EMP_SIGNALGP_LINEAR_PROGRAM_H
#define EMP_SIGNALGP_LINEAR_PROGRAM_H

#include <iostream>
#include <utility>

#include "emp/base/Ptr.hpp"
#include "emp/base/vector.hpp"
#include "emp/math/Random.hpp"
#include "emp/matching/MatchBin.hpp"
#include "emp/matching/matchbin_utils.hpp"
#include "emp/math/Range.hpp"

#include "../EventLibrary.h"
#include "InstructionLibrary.h"

#include "../../../random_utils.h"

namespace sgp {

  // Simple program:
  // - Linear.
  // - Instructions can have tag or numeric arguments.
  template<typename TAG_T, typename ARGUMENT_T=int>
  class LinearProgram {
  public:
    struct Instruction;
    using tag_t = TAG_T;
    using arg_t = ARGUMENT_T;
    using inst_t = Instruction;

    // todo => print arg fun
    // todo => print tag fun

    struct Instruction {
      size_t id;                      ///< Instruction ID
      emp::vector<arg_t> args;
      emp::vector<tag_t> tags;

      Instruction(size_t _id,
                  const emp::vector<arg_t> & _args=emp::vector<arg_t>(),
                  const emp::vector<tag_t> & _tags=emp::vector<tag_t>())
        : id(_id), args(_args), tags(_tags) { ; }

      bool operator==(const Instruction & other) const {
        return std::tie(id, args, tags) == std::tie(other.id, other.args, other.tags);
      }

      bool operator!=(const Instruction & other) const {
        return !(*this == other);
      }

      bool operator<(const Instruction & other) const {
        return std::tie(id, args, tags) < std::tie(other.id, other.args, other.tags);
      }

      void SetID(size_t _id) { id = _id; }
      size_t GetID() const { return id; }

      emp::vector<arg_t> & GetArgs() { return args; }
      const emp::vector<arg_t> & GetArgs() const { return args; }
      emp::vector<tag_t> & GetTags() { return tags; }
      const emp::vector<tag_t> & GetTags() const { return tags; }

      const arg_t & GetArg(size_t i) const { return args[i]; }
      const tag_t & GetTag(size_t i) const { return tags[i]; }

      // Print each of the instruction's tag followed by the instruction and its arguments
      template<typename HARDWARE_T, typename INST_PROPERTY_T>
      void Print(std::ostream& out, const InstructionLibrary<HARDWARE_T, inst_t, INST_PROPERTY_T>& ilib) const{
        out << "\t";
        // Skip last tag & arg so we dont get an extra delimiter.
        std::copy(tags.begin(), tags.end() - 1, std::ostream_iterator<tag_t>(out, "\n\t"));
        out << tags.back() << " " << ilib.GetName(id) << " [";
        std::copy(args.begin(), args.end() - 1, std::ostream_iterator<arg_t>(out, ", "));
        out << args.back() << "]\n";
      }
    };

  protected:
    emp::vector<Instruction> inst_seq;

  public:
    LinearProgram(const emp::vector<Instruction> & iseq=emp::vector<Instruction>())
      : inst_seq(iseq) { ; }

    LinearProgram(const LinearProgram &) = default;

    bool operator==(const LinearProgram & other) const { return inst_seq == other.inst_seq; }
    bool operator!=(const LinearProgram & other) const { return !(*this == other); }
    bool operator<(const LinearProgram & other) const { return inst_seq < other.inst_seq; }

    /// Allow program's instruction sequence to be indexed as if a vector.
    Instruction & operator[](size_t id) {
      emp_assert(id < inst_seq.size());
      return inst_seq[id];
    }

    /// Allow program's instruction sequence to be indexed as if a vector.
    const Instruction & operator[](size_t id) const {
      emp_assert(id < inst_seq.size());
      return inst_seq[id];
    }

    /// Clear the program's instruction sequence.
    void Clear() { inst_seq.clear(); }

    /// Get program size.
    size_t GetSize() const { return inst_seq.size(); }

    /// Is a given position valid in this program?
    bool IsValidPosition(size_t pos) const { return pos < GetSize(); }

    /// Set program's instruction sequence to the one given.
    void SetProgram(const emp::vector<Instruction> & p) { inst_seq = p; }

    /// Push instruction to instruction set.
    /// - No validation! We're trusting that 'id' is legit!
    void PushInst(size_t id,
                  const emp::vector<arg_t> & args=emp::vector<arg_t>(),
                  const emp::vector<tag_t> & tags=emp::vector<tag_t>()) {
      inst_seq.emplace_back(id, args, tags);
    }

    /// Push instruction to program by name.
    template<typename HARDWARE_T, typename INST_PROPERTY_T>
    void PushInst(const InstructionLibrary<HARDWARE_T, Instruction, INST_PROPERTY_T> & ilib,
                  const std::string & name,
                  const emp::vector<arg_t> & args=emp::vector<arg_t>(),
                  const emp::vector<tag_t> & tags=emp::vector<tag_t>()) {
      emp_assert(ilib.IsInst(name), "Unknown instruction name", name);
      PushInst(ilib.GetID(name), args, tags);
    }

    /// Push instruction to program.
    void PushInst(const Instruction & inst) { inst_seq.emplace_back(inst); }

    /// Is the given instruction valid?
    template<typename HARDWARE_T, typename INST_PROPERTY_T>
    static bool IsValidInst(const InstructionLibrary<HARDWARE_T, Instruction, INST_PROPERTY_T> & ilib,
                            const Instruction & inst) {
      return inst.id < ilib.GetSize();
    }

    // Print each instruction out
    template<typename HARDWARE_T, typename INST_PROPERTY_T>
    void Print(std::ostream& out, const InstructionLibrary<HARDWARE_T, inst_t, INST_PROPERTY_T>& ilib) const{
      for(auto const& inst : inst_seq){
        inst.Print(out, ilib);
      }
    }
  };

  //////////////////////////////////////////////////////////////////////////////
  // Random utilities

  template<typename HARDWARE_T, size_t TAG_WIDTH>
  typename LinearProgram<emp::BitSet<TAG_WIDTH>, int>::Instruction
    GenRandInst(emp::Random & rnd,
                const InstructionLibrary<HARDWARE_T,
                                         typename LinearProgram< emp::BitSet<TAG_WIDTH>, int>::Instruction,
                                         typename HARDWARE_T::inst_prop_t> & inst_lib,
                size_t num_tags=1,
                size_t num_args=3,
                const emp::Range<int> & arg_val_range={0, 15})
  {
    emp_assert(inst_lib.GetSize() > 0, "Instruction library must have at least one instruction definition before being used to generate a random instruction.");
    emp::vector<int> args(num_args);
    for (size_t i = 0; i < args.size(); ++i) args[i] = rnd.GetInt(arg_val_range.GetLower(), arg_val_range.GetUpper()+1);
    return {rnd.GetUInt(inst_lib.GetSize()), args, emp::RandomBitSets<TAG_WIDTH>(rnd, num_tags)};
  }

  template<typename HARDWARE_T, size_t TAG_WIDTH>
  LinearProgram<emp::BitSet<TAG_WIDTH>, int> GenRandLinearProgram(
    emp::Random & rnd,
    const InstructionLibrary<HARDWARE_T,
                             typename LinearProgram< emp::BitSet<TAG_WIDTH>, int>::Instruction,
                             typename HARDWARE_T::inst_prop_t> & inst_lib,
    const emp::Range<size_t> & inst_cnt_range={1, 32},
    // size_t min_inst_cnt=1, size_t max_inst_cnt=32,
    size_t num_inst_tags=1, size_t num_inst_args=3,
    // size_t min_arg_val=0, size_t max_arg_val=15
    const emp::Range<int> & arg_val_range={0, 15}
  ) {
    emp_assert(inst_lib.GetSize() > 0, "Instruction library must have at least one instruction definition before being used to generate a random instruction.");
    LinearProgram<emp::BitSet<TAG_WIDTH>, int> new_program;
    size_t inst_cnt = rnd.GetUInt(inst_cnt_range.GetLower(), inst_cnt_range.GetUpper()+1);
    for (size_t i = 0; i < inst_cnt; ++i) {
      new_program.PushInst(GenRandInst<HARDWARE_T, TAG_WIDTH>(rnd, inst_lib,
                                                              num_inst_tags,
                                                              num_inst_args,
                                                              arg_val_range));
    }
    return new_program;
  }

}

#endif
