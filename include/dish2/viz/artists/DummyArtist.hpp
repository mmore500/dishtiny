#pragma once
#ifndef DISH2_VIZ_ARTISTS_DUMMYARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_DUMMYARTIST_HPP_INCLUDE

#include <utility>

#include "../../../../third-party/Empirical/source/web/Canvas.h"

namespace dish2 {

struct DummyArtist {

  template<typename... Args> explicit DummyArtist( Args&&... args ) {}

  void Draw( const emp::web::Canvas& ) { }

  static std::string GetName() { return "Dummy"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_DUMMYARTIST_HPP_INCLUDE
