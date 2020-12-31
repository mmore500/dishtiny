#pragma once
#ifndef DISH2_RECORD_DRAWINGS_SERIESDRAWER_HPP_INCLUDE
#define DISH2_RECORD_DRAWINGS_SERIESDRAWER_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <ratio>
#include <string>

#include "../../../../third-party/Empirical/include/emp/base/optional.hpp"

#include "../../world/ThreadWorld.hpp"

#include "GridDrawer.hpp"

namespace dish2 {

template< typename Artist, size_t SeriesLength >
class SeriesDrawer {

  emp::array<
    emp::optional< dish2::GridDrawer< Artist > >,
    SeriesLength
  > drawers;

  size_t thread_idx;

public:

  SeriesDrawer(
    const dish2::ThreadWorld<dish2::Spec>& thread_world,
    const size_t thread_idx_
  ) : thread_idx( thread_idx_ ) {

    for (size_t i{}; i < SeriesLength; ++i) drawers[i].emplace(
      thread_world, thread_idx, i
    );

  }

  void SaveToFile() {
    // only save series for main thread
    if ( thread_idx == 0 ) for ( auto& drawer : drawers ) drawer->SaveToFile();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DRAWINGS_SERIESDRAWER_HPP_INCLUDE
