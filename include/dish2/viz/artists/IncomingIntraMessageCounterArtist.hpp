#pragma once
#ifndef DISH2_VIZ_ARTISTS_INCOMINGINTRAMESSAGECOUNTERARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_INCOMINGINTRAMESSAGECOUNTERARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IncomingInterMessageCounterColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/IncomingIntraMessageCounterGetter.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::incoming_intra_message_counter_artist {

  template<
    typename IncomingIntraMessageCounterGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::IncomingInterMessageCounterColorMap,
      IncomingIntraMessageCounterGetter
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

} // namespace internal::incoming_intra_message_counter_artist

template<
  typename Spec,
  typename IncomingIntraMessageCounterGetter
    =dish2::IncomingIntraMessageCounterGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class IncomingIntraMessageCounterArtist
: public internal::incoming_intra_message_counter_artist::parent_t<
  IncomingIntraMessageCounterGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::incoming_intra_message_counter_artist::parent_t<
    IncomingIntraMessageCounterGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Incoming Intra Message Counter"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_INCOMINGINTRAMESSAGECOUNTERARTIST_HPP_INCLUDE
