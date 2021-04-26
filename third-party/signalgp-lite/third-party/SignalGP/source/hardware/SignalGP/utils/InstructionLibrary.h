#ifndef EMP_INSTRUCTION_LIBRARY_H
#define EMP_INSTRUCTION_LIBRARY_H

#include <map>
#include <unordered_set>
#include <string>

#include "emp/base/Ptr.hpp"
#include "emp/base/vector.hpp"
#include "emp/datastructs/map_utils.hpp"
#include "emp/tools/string_utils.hpp"
#include "emp/control/Signal.hpp"

// Comments:
// - @AML: Why are instruction functions stored in multiple places? (inst def & fun_call lib)

// Requirements:
// - instruction_t MUST have a valid GetID() function

// Instruction library associates instruction IDs with definitions
// - Pretty name
// - unique ID
// - arguments (e.g., arguments)
// - fun_call (what does it do when executed?)
// - category(ies?)
//   - Instructions can have a broad type (tracked by an internal inst type library)

// Hardware specifies instruction type? => specifies argument type?

namespace sgp {

  template<typename HARDWARE_T, typename INSTRUCTION_T, typename INSTRUCTION_PROPERTY_T=size_t>
  class InstructionLibrary {

  public:
    using hardware_t = HARDWARE_T;
    using inst_t = INSTRUCTION_T;
    using inst_fun_t = std::function<void(hardware_t &, const inst_t &)>;
    using inst_prop_t = INSTRUCTION_PROPERTY_T;

    struct InstructionDef {
      std::string name;           ///< Name of this instruction.
      inst_fun_t fun_call;        ///< Function to call when instruction is executed.
      std::string desc;           ///< Description of instruction.
      std::unordered_set<inst_prop_t> properties;
      // Maybe need an instruction category?

      InstructionDef(const std::string & _name,
                     inst_fun_t _fun_call,
                     const std::string & _desc,
                     const std::unordered_set<inst_prop_t> & _properties=std::unordered_set<inst_prop_t>())
        : name(_name), fun_call(_fun_call), desc(_desc), properties(_properties)
      { ; }
      InstructionDef(const InstructionDef &) = default;
    };

  protected:

    emp::vector<InstructionDef> inst_lib;      ///< Full definitions for instructions.
    std::map<std::string, size_t> name_map;    ///< How do names link to instructions?

    emp::Signal<void(hardware_t&, const inst_t&)> before_inst_exec;

  public:

    InstructionLibrary() : inst_lib(), name_map(), before_inst_exec() { ; }
    InstructionLibrary(const InstructionLibrary &) = delete;    // @AML: Why?
    InstructionLibrary(InstructionLibrary &&) = delete;
    ~InstructionLibrary() { ; }

    InstructionLibrary & operator=(const InstructionLibrary &) = default;                   ///< Copy Operator
    InstructionLibrary & operator=(InstructionLibrary &&) = default;

    /// Remove all instructions from the instruction library.
    void Clear() {
      inst_lib.clear();
      name_map.clear();
    }

    /// Return the name associated with the specified instruction ID.
    const std::string & GetName(size_t id) const { return inst_lib[id].name; }

    /// Return the function associated with the specified instruction ID.
    const inst_fun_t & GetFunction(size_t id) const { return inst_lib[id].fun_call; }

    /// Return the provided description for the provided instruction ID.
    const std::string & GetDesc(size_t id) const { return inst_lib[id].desc; }

    /// Get the number of instructions in this set.
    size_t GetSize() const { return inst_lib.size(); }

    /// Retrieve a unique letter associated with the specified instruction ID.
    static constexpr char GetSymbol(size_t id) {
      if (id < 26) return ('a' + id);
      if (id < 52) return ('A' + (id - 26));
      if (id < 62) return ('0' + (id - 52));
      return '+';
    }

    /// Return the ID of the instruction associated with the specified symbol.
    static constexpr size_t GetID(char symbol) {
      if (symbol >= 'a' && symbol <= 'z') return (size_t) (symbol - 'a');
      if (symbol >= 'A' && symbol <= 'Z') return (size_t) (symbol - 'A' + 26);
      if (symbol >= '0' && symbol <= '9') return (size_t) (symbol - '0' + 52);
      return (size_t) 62;
    }

    /// Does instruction have a particular property?
    bool HasProperty(size_t id, const inst_prop_t & prop) {
      emp_assert(id < GetSize());
      return inst_lib[id].properties.count(prop);
    }

    /// Is the given instruction (specified by name) in the instruction library?
    bool IsInst(const std::string & name) const {
        return emp::Has(name_map, name);
    }

    /// Return the ID of the instruction that has the specified name.
    size_t GetID(const std::string & name) const {
      emp_assert(emp::Has(name_map, name), name);
      return emp::Find(name_map, name, (size_t) -1);
    }

    /// Add a new instruction to the instruction set.
    void AddInst(const std::string & name,
                 const inst_fun_t & fun_call,
                 const std::string & desc="",
                 const std::unordered_set<inst_prop_t> & properties=std::unordered_set<inst_prop_t>()) {

      const size_t id = inst_lib.size();
      inst_lib.emplace_back(name, fun_call, desc, properties);
      name_map[name] = id;
    }

    /// Process a specified instruction in the provided hardware.
    void ProcessInst(hardware_t & hw, const inst_t & inst) {
      before_inst_exec.Trigger(hw, inst);
      inst_lib[inst.GetID()].fun_call(hw, inst);
    }

    /// Process a specified instruction on hardware that can be converted to the correct type.
    template <typename IN_HW>
    void ProcessInst(emp::Ptr<IN_HW> hw, const inst_t & inst) {
      emp_assert( dynamic_cast<hardware_t*>(hw.Raw()) );
      ProcessInst(*(hw.template Cast<hardware_t>()), inst);
    }

    emp::SignalKey OnBeforeInstExec(const std::function<void(hardware_t&, const inst_t&)> & fun) {
      return before_inst_exec.AddAction(fun);
    }

    void ResetBeforeInstExecSignal() {
      before_inst_exec.Clear();
    }

  };
}

#endif