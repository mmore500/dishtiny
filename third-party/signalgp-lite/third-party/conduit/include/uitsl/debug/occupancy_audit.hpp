#pragma once
#ifndef UITSL_DEBUG_OCCUPANCY_AUDIT_HPP_INCLUDE
#define UITSL_DEBUG_OCCUPANCY_AUDIT_HPP_INCLUDE

#include "OccupancyCaps.hpp"
#include "OccupancyGuard.hpp"

#ifdef UITSL_AUDIT_OCCUPANCY

  #define uitsl_occupancy_auditor \
    uitsl::OccupancyCaps __uitsl_occupancy_auditor__

  #define uitsl_occupancy_audit(count) \
    const uitsl::OccupancyGuard guard{ \
      __uitsl_occupancy_auditor__.Get(__LINE__, count) \
    }

#else

  #define uitsl_occupancy_auditor \
    static_assert( \
      true, \
      "semi-colon required after this macro, adapted from https://stackoverflow.com/a/59153563/1261599" \
    )

  #define uitsl_occupancy_audit(count) \
    static_assert( \
      true, \
      "semi-colon required after this macro, adapted from https://stackoverflow.com/a/59153563/1261599" \
    )

#endif

#endif // #ifndef UITSL_DEBUG_OCCUPANCY_AUDIT_HPP_INCLUDE
