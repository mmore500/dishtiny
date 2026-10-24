#pragma once
#ifndef DISH2_WEB_GET_BROWSER_INFO_JSON_HPP_INCLUDE
#define DISH2_WEB_GET_BROWSER_INFO_JSON_HPP_INCLUDE

#include <cstdlib>
#include <string>

#include <emscripten.h>

namespace dish2 {

// information about the browser the simulation is running in, as JSON
inline std::string get_browser_info_json() {

  char* const buffer = reinterpret_cast<char*>( MAIN_THREAD_EM_ASM_INT({
    const text = JSON.stringify({
      user_agent: navigator.userAgent,
      language: navigator.language,
      platform: navigator.platform,
      hardware_concurrency: navigator.hardwareConcurrency,
      url: window.location.href
    });
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

#endif // #ifndef DISH2_WEB_GET_BROWSER_INFO_JSON_HPP_INCLUDE
