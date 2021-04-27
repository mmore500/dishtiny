#pragma once
#ifndef DISH2_GENOME_KINGROUPID_HPP_INCLUDE
#define DISH2_GENOME_KINGROUPID_HPP_INCLUDE

#include <algorithm>
#include <utility>

#include "../../../third-party/cereal/include/cereal/cereal.hpp"
#include "../../../third-party/cereal/include/cereal/types/array.hpp"
#include "../../../third-party/conduit/include/uitsl/algorithm/count_equal.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

namespace dish2 {

template<typename Spec>
struct KinGroupID {

  using buffer_t =  emp::array< uint64_t, Spec::NLEV >;
  buffer_t data;
  buffer_t ancestor_data;

  KinGroupID() = default;

  KinGroupID(std::in_place_t) {
    sgpl::tlrand.Get().RandFill(
      reinterpret_cast<unsigned char*>( data.data() ),
      sizeof( typename decltype(data)::value_type ) * data.size()
    );
  }

  bool operator==(const KinGroupID& other) const { return data == other.data; }

  bool operator<(const KinGroupID& other) const { return data < other.data; }

  void ApplyInheritance( const size_t rep_lev ) {
    emp_assert( rep_lev <= Spec::NLEV );

    std::copy_n(
      std::begin( data ),
      rep_lev,
      std::begin( ancestor_data )
    );

    std::generate_n(
      std::begin( data ),
      rep_lev,
      [](){
        uint64_t res;
        sgpl::tlrand.Get().RandFill(
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

  bool IsKinAncestor( const KinGroupID& other, const size_t lev ) const {
    emp_assert( lev < Spec::NLEV );
    return std::equal(
      std::next( std::begin( ancestor_data ), lev ),
      std::end( ancestor_data ),
      std::next( std::begin( other.data ), lev )
    );
  }

  size_t CountCommonality( const KinGroupID& other ) const {
    const auto mismatch = std::mismatch(
      std::rbegin( data ), std::rend( data ),
      std::rbegin( other.data )
    ).first;
    const size_t res = uitsl::audit_cast<size_t>(
      std::distance( std::rbegin(data), mismatch )
    );
    emp_assert( res == uitsl::count_equal(
      std::begin(data), std::end(data), std::begin(other.data)
    ) );
    return res;
  }

  template <class Archive>
  void serialize( Archive & ar ) {
    ar( CEREAL_NVP( data ) );
    ar( CEREAL_NVP( ancestor_data ) );
  }

  const buffer_t& GetBuffer() const { return data; }

};


} // namespace dish2

#endif // #ifndef DISH2_GENOME_KINGROUPID_HPP_INCLUDE
