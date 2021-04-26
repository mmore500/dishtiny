#pragma once
#ifndef DISH2_RECORD_DRAWINGS_GRIDDRAWER_HPP_INCLUDE
#define DISH2_RECORD_DRAWINGS_GRIDDRAWER_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <functional>
#include <ratio>
#include <string>

#include "../../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/web/Canvas.hpp"

#include "../../config/cfg.hpp"
#include "../../utility/pare_keyname_filename.hpp"
#include "../../world/ThreadWorld.hpp"

#include "../make_filename/make_drawing_filename.hpp"
#include "../make_filename/make_drawing_path.hpp"
#include "../make_filename/make_frame_path.hpp"

namespace dish2 {

template<typename Spec, typename Artist>
class GridDrawer {

  Artist artist;

  emp::web::Canvas canvas{ cfg.DRAWING_WIDTH_PX(), cfg.DRAWING_HEIGHT_PX() };

  std::reference_wrapper<const dish2::ThreadWorld<Spec>> thread_world;

  size_t series_idx;

public:
  using artist_t = Artist;

  GridDrawer(
    const dish2::ThreadWorld<Spec>& thread_world_,
    const size_t series_idx_=0
  )
  : artist( thread_world_, series_idx_ )
  , thread_world( thread_world_ )
  , series_idx( series_idx_ )
  { }

  void SaveToFileAsDrawing() {
    const std::string filename = dish2::make_drawing_filename(
      series_idx,
      thread_world.get().GetUpdate(),
      emp::slugify( std::string{Artist::GetName()} )
    );

    const std::string out_filename = dish2::pare_keyname_filename(
      filename, dish2::make_drawing_path()
    );

    artist.Draw( canvas );

    canvas.SavePNG( dish2::make_drawing_path( out_filename ) );
  }

  void SaveToFileAsFrame() {
    const std::string filename = dish2::make_drawing_filename(
      series_idx,
      thread_world.get().GetUpdate(),
      emp::slugify( std::string{Artist::GetName()} )
    );

    const std::string out_filename = dish2::pare_keyname_filename(
      filename, dish2::make_frame_path()
    );

    artist.Draw( canvas );

    canvas.SavePNG( dish2::make_frame_path( out_filename ) );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DRAWINGS_GRIDDRAWER_HPP_INCLUDE
