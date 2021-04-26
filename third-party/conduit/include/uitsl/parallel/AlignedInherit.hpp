#pragma once
#ifndef UITSL_PARALLEL_ALIGNEDINHERIT_HPP_INCLUDE
#define UITSL_PARALLEL_ALIGNEDINHERIT_HPP_INCLUDE

#include <sstream>

#include "cache_line.hpp"

namespace uitsl {

template <typename T>
class alignas(CACHE_LINE_SIZE) AlignedInherit : public T {

public:

  std::string ToString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL_ALIGNEDINHERIT_HPP_INCLUDE
