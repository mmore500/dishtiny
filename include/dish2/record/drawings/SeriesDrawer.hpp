#pragma once
#ifndef DISH2_RECORD_DRAWINGS_SERIESDRAWER_HPP_INCLUDE
#define DISH2_RECORD_DRAWINGS_SERIESDRAWER_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <ratio>
#include <string>

#include "../../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../config/thread_idx.hpp"
#include "../../world/ThreadWorld.hpp"

#include "GridDrawer.hpp"

namespace dish2 {

template< typename Spec, typename Artist >
class SeriesDrawer {

  emp::vector<
    emp::optional< dish2::GridDrawer< Spec, Artist > >
  > drawers;

public:

  using artist_t = Artist;

  SeriesDrawer(
    const dish2::ThreadWorld<Spec>& thread_world
  ) : drawers( Artist::GetSeriesLength( thread_world ) ) {

    for (size_t i{}; i < drawers.size(); ++i) drawers[i].emplace(
      thread_world, i
    );

  }

  void SaveToFileAsDrawing() {
    // only save series for main thread on root process
    // in order to reduce computational intensity
    if ( dish2::thread_idx == 0 && uitsl::is_root() ) {
      for ( auto& drawer : drawers ) drawer->SaveToFileAsDrawing();
    }
  }

  void SaveToFileAsFrame() {
    // only save series for main thread on root process
    // in order to reduce computational intensity
    if ( dish2::thread_idx == 0 && uitsl::is_root() ) {
      for ( auto& drawer : drawers ) drawer->SaveToFileAsFrame();
    }
  }

};

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DRAWINGS_SERIESDRAWER_HPP_INCLUDE
