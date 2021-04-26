#pragma once
#ifndef DISH2_VIZ_ARTISTS_ARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_ARTIST_HPP_INCLUDE

#include <utility>

#include "../../../../third-party/Empirical/include/emp/web/Canvas.hpp"

#include "../../world/ThreadWorld.hpp"

namespace dish2 {

// base case
template<typename... Renderer> struct Artist {
  template<typename... Args>
  explicit Artist( Args&&... args ) {}
};

// adapted from https://stackoverflow.com/a/35284581
template<typename FirstRenderer, typename... SubsequentRenderers>
class Artist<FirstRenderer, SubsequentRenderers...> {

  FirstRenderer first_renderer;

  dish2::Artist<SubsequentRenderers...> subsequent_artist;

  using getter_t = typename FirstRenderer::getter_t;

public:

  template<typename... Args>
  explicit Artist( Args&&... args )
  : first_renderer( std::forward<Args>(args)... )
  , subsequent_artist( std::forward<Args>(args)... )
  {}

  template<bool ClearCanvas=true>
  void Draw( emp::web::Canvas& canvas ) {

    if constexpr ( ClearCanvas ) canvas.Clear();
    first_renderer.Render( canvas );
    if constexpr ( sizeof...(SubsequentRenderers) > 0 ) {
      subsequent_artist.template Draw<false>( canvas );
    }

  }

  template< typename Spec >
  static size_t GetSeriesLength(const dish2::ThreadWorld<Spec>& thread_world) {
    return 1;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_ARTIST_HPP_INCLUDE
