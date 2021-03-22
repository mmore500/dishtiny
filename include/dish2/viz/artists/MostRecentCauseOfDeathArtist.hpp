#pragma once
#ifndef DISH2_VIZ_ARTISTS_MOSTRECENTCAUSEOFDEATHARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_MOSTRECENTCAUSEOFDEATHARTIST_HPP_INCLUDE

#include <string>

#include "../fill_colormaps/CauseOfDeathColorMap.hpp"
#include "../getters/MostRecentCauseOfDeathGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::most_recent_cause_of_death_artist {

  template<
    typename MostRecentCauseOfDeathGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::CauseOfDeathColorMap,
      MostRecentCauseOfDeathGetter
    >
  >;

} // namespace internal::most_recent_cause_of_death_artist

template<
  typename Spec,
  typename MostRecentCauseOfDeathGetter
    =dish2::MostRecentCauseOfDeathGetter<Spec>
>
class MostRecentCauseOfDeathArtist
: public internal::most_recent_cause_of_death_artist::parent_t<
  MostRecentCauseOfDeathGetter
> {

  using parent_t = internal::most_recent_cause_of_death_artist::parent_t<
    MostRecentCauseOfDeathGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Most Recent Cause of Death"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_MOSTRECENTCAUSEOFDEATHARTIST_HPP_INCLUDE
