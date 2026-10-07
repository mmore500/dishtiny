#pragma once
#ifndef DISH2_WEB_URL_ENCODE_HPP_INCLUDE
#define DISH2_WEB_URL_ENCODE_HPP_INCLUDE

#include <cstdlib>
#include <string>

#include <emscripten.h>

namespace dish2 {

// encodes using URLSearchParams
// TODO replace with emp::url_encode after version bump
inline std::string url_encode( const std::string& in ) {

  char* const buffer = reinterpret_cast<char*>( MAIN_THREAD_EM_ASM_INT({
    const encoded = new URLSearchParams(
      [['k', UTF8ToString( $0 )]]
    ).toString().slice( 2 );
    const num_bytes = lengthBytesUTF8( encoded ) + 1;
    const buffer = Module._malloc( num_bytes );
    stringToUTF8( encoded, buffer, num_bytes );
    return buffer;
  }, in.c_str() ) );

  const std::string res( buffer );
  std::free( buffer );
  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_WEB_URL_ENCODE_HPP_INCLUDE
