#pragma once
#ifndef SGPL_OPERATIONS_UNARY_RANDOMBOOL_HPP_INCLUDE
#define SGPL_OPERATIONS_UNARY_RANDOMBOOL_HPP_INCLUDE

#include <map>
#include <set>
#include <string>

#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../hardware/Core.hpp"
#include "../../program/Instruction.hpp"
#include "../../program/Program.hpp"
#include "../../utility/ThreadLocalRandom.hpp"

namespace sgpl {

class RandomBool {

  template<typename Spec>
  static double map_to_unit( const typename Spec::tag_t& tag ) {

    constexpr double max_double = Spec::tag_t::MaxDouble();

    return tag.GetDouble() / max_double;

  }

public:

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) {

    const double p = map_to_unit<Spec>( inst.tag );

    core.registers[ inst.args[0] ] = sgpl::tlrand.Get().P( p );

  }

  static std::string name() { return "Generate Random Bool"; }

  static size_t prevalence() { return 1; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "probability", emp::to_string( map_to_unit<Spec>( inst.tag ) ) },
      { "summary", "if p, a = 1; else a = 0" },
    };
  }

  template<typename Spec>
  static std::set<std::string> categories(const sgpl::Instruction<Spec>&) {
    return {
      "calculation",
      "intrinsic",
      "op",
      "stochastic",
    };
  }

};

} // namespace sgpl

#endif // #ifndef SGPL_OPERATIONS_UNARY_RANDOMBOOL_HPP_INCLUDE
