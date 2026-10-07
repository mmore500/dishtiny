#pragma once
#ifndef DISH2_WEB_TOGGLE_VIEW_KEY_HPP_INCLUDE
#define DISH2_WEB_TOGGLE_VIEW_KEY_HPP_INCLUDE

#include <string>

#include <emscripten.h>

namespace dish2 {

// shows the key (if any) for a view just below its selector button
inline void toggle_view_key(
  const std::string& slug, const bool show
) {
  MAIN_THREAD_EM_ASM({
    const slug = UTF8ToString($0);
    const key = document.getElementById( slug + '-key' );
    if ( key === null ) return;

    // cancel any wait for the selector to be added
    if ( key.observer ) key.observer.disconnect();
    key.observer = null;

    // hidden keys are kept out from between selectors to preserve spacing
    if ( !$1 ) {
      document.getElementById( 'view-key-holder' ).appendChild( key );
      key.style.display = 'none';
      return;
    }

    // returns false if the selector is not in the document yet
    const show = function() {
      const selector = document.getElementById( slug + '-selector' );
      if ( selector === null ) return false;
      key.style.marginBottom = '.5rem';
      selector.parentNode.insertBefore( key, selector.nextSibling );
      key.style.display = 'block';
      return true;
    };

    if ( show() ) return;

    // otherwise, show the key once the selector is added
    key.observer = new MutationObserver( function() {
      if ( show() ) key.observer.disconnect();
    } );
    key.observer.observe( document.body, { childList: true, subtree: true } );
  }, slug.c_str(), show );
}

} // namespace dish2

#endif // #ifndef DISH2_WEB_TOGGLE_VIEW_KEY_HPP_INCLUDE
