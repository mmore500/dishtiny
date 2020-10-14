#pragma once
#ifndef DISH__CELL_CELL_HPP_INCLUDE
#define DISH__CELL_CELL_HPP_INCLUDE

#include "../../../third-party/Empirical/source/base/vector.h"

#include "../genome/Genome.hpp"

#include "Cardinal.hpp"

namespace dish2 {

template<typename Spec>
struct Cell {

  emp::vector<dish2::Cardinal<Spec>> cardinals;

  dish2::Genome<Spec> genome;

};

} // namespace dish2

#endif // #ifndef DISH__CELL_CELL_HPP_INCLUDE
