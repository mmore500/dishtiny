#pragma once
#ifndef UITSL_PARALLEL_ALIGNEDIMPLICIT_HPP_INCLUDE
#define UITSL_PARALLEL_ALIGNEDIMPLICIT_HPP_INCLUDE

#include <sstream>

#include "cache_line.hpp"

namespace uitsl {

template <typename T>
class alignas(CACHE_LINE_SIZE) AlignedImplicit {

  T val{};

public:

  AlignedImplicit() { ; }
  AlignedImplicit(const T& val_) : val(val_) { ; }

  operator const T&() const { return val; }
  operator T&() { return val; }

  std::string ToString() const {
    std::stringstream ss;
    ss << val;
    return ss.str();
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL_ALIGNEDIMPLICIT_HPP_INCLUDE
