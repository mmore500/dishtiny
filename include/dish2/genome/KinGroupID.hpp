#pragma once
#ifndef DISH2_GENOME_KINGROUPID_HPP_INCLUDE
#define DISH2_GENOME_KINGROUPID_HPP_INCLUDE

#include <algorithm>
#include <utility>

#include "../../../third-party/cereal/include/cereal/types/array.hpp"
#include "../../../third-party/Empirical/source/base/array.h"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

namespace dish2 {

template<typename Spec>
struct KinGroupID {

  using buffer_t =  emp::array< size_t, Spec::NLEV >;
  buffer_t data;

  KinGroupID() = default;

  KinGroupID(std::in_place_t) {
    sgpl::ThreadLocalRandom::Get().RandFill(
      reinterpret_cast<unsigned char*>( data.data() ),
      sizeof( typename decltype(data)::value_type ) * data.size()
    );
  }

  bool operator==(const KinGroupID& other) const { return data == other.data; }

  void ApplyInheritance( const size_t rep_lev ) {
    emp_assert( rep_lev <= Spec::NLEV );
    std::generate(
      std::begin( data ),
      std::next( std::begin( data ), rep_lev ),
      [](){
        size_t res;
        sgpl::ThreadLocalRandom::Get().RandFill(
          reinterpret_cast<unsigned char*>( &res ),
          sizeof( res )
        );
        return res;
      }
    );
  }

  bool IsKin( const KinGroupID& other, const size_t lev ) const {
    emp_assert( lev < Spec::NLEV );
    return std::equal(
      std::next( std::begin( data ), lev ),
      std::end( data ),
      std::next( std::begin( other.data ), lev )
    );
  }

  template <class Archive> void serialize( Archive & ar ) { ar( data ); }

  const buffer_t& GetBuffer() const { return data; }

};


} // namespace dish2

#endif // #ifndef DISH2_GENOME_KINGROUPID_HPP_INCLUDE
