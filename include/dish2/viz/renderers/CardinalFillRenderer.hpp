#pragma once
#ifndef DISH2_VIZ_RENDERERS_CARDINALFILLRENDERER_HPP_INCLUDE
#define DISH2_VIZ_RENDERERS_CARDINALFILLRENDERER_HPP_INCLUDE

namespace dish2 {

template<typename ColorMap, typename Getter>
class CardinalFillRenderer {

  ColorMap color_map;
  Getter getter;

public:

  using getter_t = Getter;

  template<typename... Args>
  CardinalFillRenderer( Args&&... args )
  : color_map( std::forward<Args>(args)... )
  , getter( std::forward<Args>(args)... )
  {}

  void Render( emp::web::Canvas& canvas ) {}

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_RENDERERS_CARDINALFILLRENDERER_HPP_INCLUDE
