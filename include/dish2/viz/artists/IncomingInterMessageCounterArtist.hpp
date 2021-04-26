#pragma once
#ifndef DISH2_VIZ_ARTISTS_INCOMINGINTERMESSAGECOUNTERARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_INCOMINGINTERMESSAGECOUNTERARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IncomingInterMessageCounterColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/IncomingInterMessageCounterGetter.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::incoming_inter_message_counter_artist {

  template<
    typename IncomingInterMessageCounterGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::IncomingInterMessageCounterColorMap,
      IncomingInterMessageCounterGetter
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

} // namespace internal::incoming_inter_message_counter_artist

template<
  typename Spec,
  typename IncomingInterMessageCounterGetter
    =dish2::IncomingInterMessageCounterGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class IncomingInterMessageCounterArtist
: public internal::incoming_inter_message_counter_artist::parent_t<
  IncomingInterMessageCounterGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::incoming_inter_message_counter_artist::parent_t<
    IncomingInterMessageCounterGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Incoming Inter Message Counter"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_INCOMINGINTERMESSAGECOUNTERARTIST_HPP_INCLUDE
