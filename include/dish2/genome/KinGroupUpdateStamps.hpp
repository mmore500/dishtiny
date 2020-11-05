#pragma once
#ifndef DISH2_GENOME_KINGROUPUPDATESTAMPS_HPP_INCLUDE
#define DISH2_GENOME_KINGROUPUPDATESTAMPS_HPP_INCLUDE

#include <algorithm>
#include <utility>

#include "../../../third-party/cereal/include/cereal/types/array.hpp"
#include "../../../third-party/Empirical/source/base/array.h"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

namespace dish2 {

// TODO FIXME multiproc multithread
// add some sort of update sync between simulation components
template<typename Spec>
struct KinGroupUpdateStamps {

  using buffer_t =  emp::array< size_t, Spec::NLEV >;
  buffer_t data{};

  KinGroupUpdateStamps() = default;

  bool operator==(const KinGroupUpdateStamps& other) const {
    return data == other.data;
  }

  void ApplyInheritance( const size_t rep_lev, const size_t update ) {
    emp_assert( rep_lev <= Spec::NLEV );
    std::fill(
      std::begin( data ),
      std::next( std::begin( data ), rep_lev ),
      update
    );
  }

  template <class Archive> void serialize( Archive & ar ) { ar( data ); }

  const buffer_t& GetBuffer() const { return data; }

};


} // namespace dish2

#endif // #ifndef DISH2_GENOME_KINGROUPUPDATESTAMPS_HPP_INCLUDE
