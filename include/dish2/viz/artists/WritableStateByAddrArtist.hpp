#pragma once
#ifndef DISH2_VIZ_ARTISTS_WRITABLESTATEBYADDRARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_WRITABLESTATEBYADDRARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/SignedContinuousColorMap.hpp"
#include "../getters/WritableStateByAddrGetter.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::writable_state_by_addr_artist {

  template<
    typename WritableStateByAddrGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::SignedContinuousColorMap,
      WritableStateByAddrGetter
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

} // namespace internal::writable_state_by_addr_artist

template<
  typename Spec,
  typename WritableStateByAddrGetter=dish2::WritableStateByAddrGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class WritableStateByAddrArtist
: public internal::writable_state_by_addr_artist::parent_t<
  WritableStateByAddrGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::writable_state_by_addr_artist::parent_t<
    WritableStateByAddrGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() {
    return "Writable State by Address";
  }

  static size_t GetSeriesLength(const dish2::ThreadWorld<Spec>&) {
    return dish2::WritableStateByAddrGetter<Spec>::GetNumAddrs();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_WRITABLESTATEBYADDRARTIST_HPP_INCLUDE
