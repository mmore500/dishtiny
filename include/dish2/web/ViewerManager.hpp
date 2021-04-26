#pragma once
#ifndef DISH2_WEB_VIEWERMANAGER_HPP_INCLUDE
#define DISH2_WEB_VIEWERMANAGER_HPP_INCLUDE

namespace dish2 {

// base case
template<typename... SubsequentViewers> struct ViewerManager {

  template<typename... Args> ViewerManager( Args&&... args ) {}

  void Redraw() {}

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

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_VIEWERMANAGER_HPP_INCLUDE
