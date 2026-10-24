#pragma once
#ifndef DISH2_WEB_GET_VIEW_KEYS_JSON_HPP_INCLUDE
#define DISH2_WEB_GET_VIEW_KEYS_JSON_HPP_INCLUDE

#include <cstdlib>
#include <string>

#include <emscripten.h>

namespace dish2 {

// the page's view keys (i.e., keys.toml) as JSON, keyed by view slug
inline std::string get_view_keys_json() {

  char* const buffer = reinterpret_cast<char*>( MAIN_THREAD_EM_ASM_INT({
    const element = document.getElementById( 'view-keys-data' );
    const text = element === null ? '{}' : element.textContent;
    const num_bytes = lengthBytesUTF8( text ) + 1;
    const buffer = Module._malloc( num_bytes );
    stringToUTF8( text, buffer, num_bytes );
    return buffer;
  }) );

  const std::string res( buffer );
  std::free( buffer );
  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_WEB_GET_VIEW_KEYS_JSON_HPP_INCLUDE
