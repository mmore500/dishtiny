#pragma once
#ifndef DISH2_VIZ_ARTISTS_READABLESTATEBYADDRARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_READABLESTATEBYADDRARTIST_HPP_INCLUDE

#include <string_view>

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/SignedContinuousColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/ReadableStateByAddrGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::readable_state_by_addr_artist {

  template<
    typename ReadableStateByAddrGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::SignedContinuousColorMap,
      ReadableStateByAddrGetter
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

} // namespace internal::readable_state_by_addr_artist

template<
  typename Spec,
  typename ReadableStateByAddrGetter=dish2::ReadableStateByAddrGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class ReadableStateByAddrArtist
: public internal::readable_state_by_addr_artist::parent_t<
  ReadableStateByAddrGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::readable_state_by_addr_artist::parent_t<
    ReadableStateByAddrGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() {
    return "Readable State by Address";
  }

  static size_t GetSeriesLength(const dish2::ThreadWorld<Spec>&) {
    return dish2::ReadableState<Spec>::GetSize();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_READABLESTATEBYADDRARTIST_HPP_INCLUDE
