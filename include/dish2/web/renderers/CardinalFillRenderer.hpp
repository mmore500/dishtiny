#pragma once
#ifndef DISH2_WEB_RENDERERS_CARDINALFILLRENDERER_HPP_INCLUDE
#define DISH2_WEB_RENDERERS_CARDINALFILLRENDERER_HPP_INCLUDE

namespace dish2 {

template<typename ColorMap, typename Getter>
class CardinalFillRenderer {

  ColorMap color_map;
  Getter getter;

public:

  using getter_t = Getter;

  CardinalFillRenderer( Getter getter_ )
  : color_map( getter_ )
  , getter( getter_ )
  {}

  void Render( emp::web::Canvas& canvas ) {}

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_RENDERERS_CARDINALFILLRENDERER_HPP_INCLUDE
