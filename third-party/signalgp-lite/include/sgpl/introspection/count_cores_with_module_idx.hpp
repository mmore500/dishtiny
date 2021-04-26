#pragma once
#ifndef SGPL_INTROSPECTION_COUNT_CORES_WITH_MODULE_IDX_HPP_INCLUDE
#define SGPL_INTROSPECTION_COUNT_CORES_WITH_MODULE_IDX_HPP_INCLUDE

#include <algorithm>

#include "../hardware/Cpu.hpp"
#include "../program/GlobalAnchorIterator.hpp"
#include "../program/Program.hpp"
#include "../utility/CountingIterator.hpp"

#include "get_cur_module_idx.hpp"

namespace sgpl {

template<typename Spec>
size_t count_cores_with_module_idx(
  const sgpl::Cpu<Spec>& cpu,
  const sgpl::Program<Spec>& program,
  const size_t module_idx
) {


  return std::count_if(
    sgpl::CountingIterator{},
    sgpl::CountingIterator{ cpu.GetNumBusyCores() },
    [&](const auto& core_idx){
      return sgpl::get_cur_module_idx(
        cpu.GetCore( core_idx ),
        program
      ) == module_idx;
    }
  );

}

} // namespace sgpl

#endif // #ifndef SGPL_INTROSPECTION_COUNT_CORES_WITH_MODULE_IDX_HPP_INCLUDE
