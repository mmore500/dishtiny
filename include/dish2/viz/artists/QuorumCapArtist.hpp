#pragma once
#ifndef DISH2_VIZ_ARTISTS_QUORUMCAPARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_QUORUMCAPARTIST_HPP_INCLUDE

#include <string>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/BooleanColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/QuorumCapGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::quorum_cap_artist {

  template<
    typename QuorumCapGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::BooleanColorMap,
      QuorumCapGetter
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

} // namespace internal::quorum_cap_artist

template<
  typename Spec,
  typename QuorumCapGetter=dish2::QuorumCapGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class QuorumCapArtist
: public internal::quorum_cap_artist::parent_t<
  QuorumCapGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::quorum_cap_artist::parent_t<
    QuorumCapGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Quorum Cap"; }

  static size_t GetSeriesLength(const dish2::ThreadWorld<Spec>&) {
    return Spec::NLEV;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_QUORUMCAPARTIST_HPP_INCLUDE
