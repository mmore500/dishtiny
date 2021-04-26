#pragma once
#ifndef DISH2_VIZ_ARTISTS_DUMMYARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_DUMMYARTIST_HPP_INCLUDE

#include <utility>

#include "../../../../third-party/Empirical/include/emp/web/Canvas.hpp"

namespace dish2 {

struct DummyArtist {

  template<typename... Args> explicit DummyArtist( Args&&... args ) {}

  DummyArtist(const DummyArtist&) = default;
  DummyArtist& operator=(const DummyArtist&) { return *this; }

  void Draw( const emp::web::Canvas& ) { }

  static constexpr std::string_view GetName() { return "Dummy"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_DUMMYARTIST_HPP_INCLUDE
