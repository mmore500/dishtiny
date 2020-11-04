#pragma once
#ifndef DISH2_VIZ_ARTISTS_QUORUMBITSARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_QUORUMBITSARTIST_HPP_INCLUDE

#include <string>

#include "../../spec/Spec.hpp"

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/QuorumBitsColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/QuorumBitsGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::quorum_bits_artist {

  template<
    typename QuorumBitsGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::PhylogeneticRootColorMap,
      QuorumBitsGetter
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

} // namespace internal::quorum_bits_artist

template<
  typename QuorumBitsGetter=dish2::QuorumBitsGetter<dish2::Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<dish2::Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<dish2::Spec>
>
class QuorumBitsArtist
: public internal::quorum_bits_artist::parent_t<
  QuorumBitsGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::quorum_bits_artist::parent_t<
    QuorumBitsGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Quorum Bits"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_QUORUMBITSARTIST_HPP_INCLUDE
