#pragma once
#ifndef UITSL_DEBUG_OCCUPANCYGUARD_HPP_INCLUDE
#define UITSL_DEBUG_OCCUPANCYGUARD_HPP_INCLUDE

#include "OccupancyCap.hpp"

namespace uitsl {

class OccupancyGuard {

  OccupancyCap & space;

public:
  OccupancyGuard(OccupancyCap & space_)
  : space(space_)
  { space.Enter(); }

  ~OccupancyGuard() { space.Exit(); }

};

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_OCCUPANCYGUARD_HPP_INCLUDE
