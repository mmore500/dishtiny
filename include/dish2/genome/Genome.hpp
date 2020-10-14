#pragma once
#ifndef DISH2_GENOME_GENOME_HPP_INCLUDE
#define DISH2_GENOME_GENOME_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

namespace dish2 {

template<typename Spec>
class Genome {

  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  sgpl::Program<sgpl_spec_t> program;

};

} // namespace dish2

#endif // #ifndef DISH2_GENOME_GENOME_HPP_INCLUDE
