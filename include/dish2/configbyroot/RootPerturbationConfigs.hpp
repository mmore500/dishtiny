#pragma once
#ifndef DISH2_CONFIGBYROOT_ROOTPERTURBATIONCONFIGS_HPP_INCLUDE
#define DISH2_CONFIGBYROOT_ROOTPERTURBATIONCONFIGS_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/datastructs/SmallVector.hpp"

#include "RootPerturbationConfig.hpp"

namespace dish2 {

class RootPerturbationConfigs {

  emp::SmallVector<dish2::RootPerturbationConfig, 2> data;

  dish2::RootPerturbationConfig dummy;

public:

  const RootPerturbationConfig& View( const size_t root ) const {

    if ( root >= data.size() ) return dummy;
    else return data[ root ];

  }

  RootPerturbationConfig& Get( const size_t root ) {

    data.resize( root + 1 );
    return data[ root ];

  }

};

} // namespace dish2

#endif // #ifndef DISH2_CONFIGBYROOT_ROOTPERTURBATIONCONFIGS_HPP_INCLUDE
