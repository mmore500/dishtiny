#pragma once
#ifndef UITSL_DEBUG_ONCEPERTHREADCHECKER_HPP_INCLUDE
#define UITSL_DEBUG_ONCEPERTHREADCHECKER_HPP_INCLUDE

#include "../debug/IsFirstExecutionChecker.hpp"
#include "../parallel/ThreadMap.hpp"

namespace uitsl {

class OncePerThreadChecker {

  uitsl::ThreadMap<uitsl::IsFirstExecutionChecker> map;

public:

  void Check() { map.GetWithDefault().Check(); }

};

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_ONCEPERTHREADCHECKER_HPP_INCLUDE
