#pragma once
#ifndef DISH2_WEB_CLICK_ELEMENTS_HPP_INCLUDE
#define DISH2_WEB_CLICK_ELEMENTS_HPP_INCLUDE

#include <algorithm>
#include <string>

#include <emscripten.h>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

namespace dish2 {

// clicks the elements with the given ids, in order

inline void click_elements( const emp::vector<std::string>& ids ) {

  if ( ids.empty() ) return;

  // web elements cannot contain spaces
  emp_assert( std::none_of(
    ids.begin(),
    ids.end(),
    []( const std::string& id ){ return id.find( ' ' ) != std::string::npos; }
  ) );

  const std::string joined = emp::join( ids, " " );

  MAIN_THREAD_EM_ASM({
    const ids = UTF8ToString( $0 ).split( ' ' );
    const click_next = function( i, num_tries ) {
      if ( i >= ids.length ) return;
      const element = document.getElementById( ids[i] );
      if ( element !== null ) {
        element.click();
        click_next( i + 1, 0 );
      } else if ( num_tries < 5 ) {
        setTimeout( function() { click_next( i, num_tries + 1 ); }, 50 );
      } else {
        console.warn( 'no element to click with id "' + ids[i] + '"' );
        click_next( i + 1, 0 );
      }
    };
    setTimeout( function() { click_next( 0, 0 ); }, 0 );
  }, joined.c_str() );

}

} // namespace dish2

#endif // #ifndef DISH2_WEB_CLICK_ELEMENTS_HPP_INCLUDE
