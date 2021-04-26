#pragma once
#ifndef DISH2_CONFIGBYROOT_ROOTMUTATIONCONFIGS_HPP_INCLUDE
#define DISH2_CONFIGBYROOT_ROOTMUTATIONCONFIGS_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/datastructs/SmallVector.hpp"

#include "RootMutationConfig.hpp"

namespace dish2 {

class RootMutationConfigs {

  emp::SmallVector<dish2::RootMutationConfig, 2> data;

  dish2::RootMutationConfig dummy;

public:

  const RootMutationConfig& View( const size_t root ) const {

    if ( root >= data.size() ) return dummy;
    else return data[ root ];

  }

  RootMutationConfig& Get( const size_t root ) {

    data.resize( root + 1 );
    return data[ root ];

  }

};

} // namespace dish2

#endif // #ifndef DISH2_CONFIGBYROOT_ROOTMUTATIONCONFIGS_HPP_INCLUDE
