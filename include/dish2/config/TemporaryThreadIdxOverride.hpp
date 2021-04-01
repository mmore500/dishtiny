#pragma once
#ifndef DISH2_CONFIG_TEMPORARYTHREADIDXOVERRIDE_HPP_INCLUDE
#define DISH2_CONFIG_TEMPORARYTHREADIDXOVERRIDE_HPP_INCLUDE

#include <utility>

#include "thread_idx.hpp"

namespace dish2 {

class TemporaryThreadIdxOverride {

  size_t thread_idx_bak;

public:

  TemporaryThreadIdxOverride(const size_t temporary_override)
  : thread_idx_bak( std::exchange(dish2::thread_idx, temporary_override) )
  {}

  ~TemporaryThreadIdxOverride() { dish2::thread_idx = thread_idx_bak; }

};

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_TEMPORARYTHREADIDXOVERRIDE_HPP_INCLUDE
