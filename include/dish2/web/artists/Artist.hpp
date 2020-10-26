#pragma once
#ifndef DISH2_WEB_ARTISTS_ARTIST_HPP_INCLUDE
#define DISH2_WEB_ARTISTS_ARTIST_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/web/Canvas.h"

namespace dish2 {

// base case
template<typename... Renderer> struct Artist {
  template<typename Getter>
  explicit Artist(Getter getter) {}
};

// adapted from https://stackoverflow.com/a/35284581
template<typename FirstRenderer, typename... SubsequentRenderers>
class Artist<FirstRenderer, SubsequentRenderers...> {

  FirstRenderer first_renderer;

  dish2::Artist<SubsequentRenderers...> subsequent_artist;

  using getter_t = typename FirstRenderer::getter_t;

public:

  explicit Artist( getter_t getter )
  : first_renderer( getter )
  , subsequent_artist( getter )
  {}

  template<bool ClearCanvas=true>
  void Draw( emp::web::Canvas& canvas ) {

    if constexpr ( ClearCanvas ) canvas.Clear();
    first_renderer.Render( canvas );
    if constexpr ( sizeof...(SubsequentRenderers) > 0 ) {
      subsequent_artist.template Draw<false>( canvas );
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_ARTISTS_ARTIST_HPP_INCLUDE
