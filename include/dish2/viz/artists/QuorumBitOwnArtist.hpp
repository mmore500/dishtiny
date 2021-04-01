#pragma once
#ifndef DISH2_VIZ_ARTISTS_QUORUMBITOWNARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_QUORUMBITOWNARTIST_HPP_INCLUDE

#include <string>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/BooleanColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/QuorumBitOwnGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::quorum_bit_own_artist {

  template<
    typename QuorumBitOwnGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::BooleanColorMap,
      QuorumBitOwnGetter
    >,
    dish2::CellFillRenderer<
      dish2::IsAliveColorMap,
      IsAliveGetter
    >,
    dish2::CellBorderRenderer<
      dish2::KinGroupIDBorderColorMap,
      KinGroupIDGetter
    >
  >;

} // namespace internal::quorum_bit_own_artist

template<
  typename Spec,
  typename QuorumBitOwnGetter=dish2::QuorumBitOwnGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class QuorumBitOwnArtist
: public internal::quorum_bit_own_artist::parent_t<
  QuorumBitOwnGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::quorum_bit_own_artist::parent_t<
    QuorumBitOwnGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Own Quorum Bit"; }

  static size_t GetSeriesLength(const dish2::ThreadWorld<Spec>&) {
    return Spec::NLEV;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_QUORUMBITOWNARTIST_HPP_INCLUDE
