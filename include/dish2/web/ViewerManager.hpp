#pragma once
#ifndef DISH2_WEB_VIEWERMANAGER_HPP_INCLUDE
#define DISH2_WEB_VIEWERMANAGER_HPP_INCLUDE

#include <string>

namespace dish2 {

// base case
template<typename... SubsequentViewers> struct ViewerManager {

  template<typename... Args> ViewerManager( Args&&... args ) {}

  void Redraw() {}
  void Save( const size_t update ) {}
  void Close() {}

};

// adapted from https://stackoverflow.com/a/35284581
template<typename FirstViewer, typename... SubsequentViewers>
struct ViewerManager<FirstViewer, SubsequentViewers...> {

  using subsequent_viewers_t = dish2::ViewerManager<SubsequentViewers...>;

  FirstViewer first_viewer;
  ViewerManager<SubsequentViewers...> subsequent_viewers;

  template<typename... Args>
  ViewerManager( Args&&... args )
  : first_viewer( std::forward<Args>( args )... )
  , subsequent_viewers( std::forward<Args>( args )... )
  {}

  void Redraw() {
    if ( first_viewer.IsActivated() ) first_viewer.Redraw();
    subsequent_viewers.Redraw();
  }

  void Save( const size_t update ) {
    if ( first_viewer.IsActivated() ) first_viewer.Save( update );
    subsequent_viewers.Save( update );
  }

  // closes every activated viewer
  void Close() {
    if ( first_viewer.IsActivated() ) first_viewer.Deactivate();
    subsequent_viewers.Close();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_VIEWERMANAGER_HPP_INCLUDE
