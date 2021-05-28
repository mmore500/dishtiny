#pragma once
#ifndef DISH2_GENOME_KINGROUPEPOCHSTAMPS_HPP_INCLUDE
#define DISH2_GENOME_KINGROUPEPOCHSTAMPS_HPP_INCLUDE

#include <algorithm>
#include <utility>

#include "../../../third-party/cereal/include/cereal/cereal.hpp"
#include "../../../third-party/cereal/include/cereal/types/array.hpp"
#include "../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

namespace dish2 {

template<typename Spec>
struct KinGroupEpochStamps {

  using buffer_t = std::array< uint64_t, Spec::NLEV >;
  buffer_t data{};

  KinGroupEpochStamps() = default;

  bool operator==(const KinGroupEpochStamps& other) const {
    return data == other.data;
  }

  bool operator<(const KinGroupEpochStamps& other) const {
    return data < other.data;
  }

  void ApplyInheritance( const size_t rep_lev, const size_t epoch ) {
    emp_assert( rep_lev <= Spec::NLEV );
    std::fill(
      std::begin( data ),
      std::next( std::begin( data ), rep_lev ),
      epoch
    );
  }

  const buffer_t& GetBuffer() const { return data; }

  template <class Archive>
  void serialize( Archive & ar ) { ar(
    CEREAL_NVP( data )
  ); }

};


} // namespace dish2

#endif // #ifndef DISH2_GENOME_KINGROUPEPOCHSTAMPS_HPP_INCLUDE
