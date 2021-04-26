#pragma once
#ifndef DISH2_VIZ_ARTISTS_LEARNEDQUORUMBITARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_LEARNEDQUORUMBITARTIST_HPP_INCLUDE

#include <string>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/BooleanColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/LearnedQuorumBitGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::learned_quorum_bit_artist {

  template<
    typename LearnedQuorumBitGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::BooleanColorMap,
      LearnedQuorumBitGetter
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

} // namespace internal::learned_quorum_bit_artist

template<
  typename Spec,
  typename LearnedQuorumBitGetter=dish2::LearnedQuorumBitGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class LearnedQuorumBitArtist
: public internal::learned_quorum_bit_artist::parent_t<
  LearnedQuorumBitGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::learned_quorum_bit_artist::parent_t<
    LearnedQuorumBitGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Learned Quorum Bit"; }

  static size_t GetSeriesLength(const dish2::ThreadWorld<Spec>&) {
    return Spec::NLEV;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_LEARNEDQUORUMBITARTIST_HPP_INCLUDE
