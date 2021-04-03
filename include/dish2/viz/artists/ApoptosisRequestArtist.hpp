#pragma once
#ifndef DISH2_VIZ_ARTISTS_APOPTOSISREQUESTARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_APOPTOSISREQUESTARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/BooleanColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/ApoptosisRequestGetter.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::apoptosis_request_artist {

  template<
    typename ApoptosisRequestGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::BooleanColorMap,
      ApoptosisRequestGetter
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

} // namespace internal::apoptosis_request_artist

template<
  typename Spec,
  typename ApoptosisRequestGetter=dish2::ApoptosisRequestGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class ApoptosisRequestArtist
: public internal::apoptosis_request_artist::parent_t<
  ApoptosisRequestGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::apoptosis_request_artist::parent_t<
    ApoptosisRequestGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Apoptosis Request"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_APOPTOSISREQUESTARTIST_HPP_INCLUDE
