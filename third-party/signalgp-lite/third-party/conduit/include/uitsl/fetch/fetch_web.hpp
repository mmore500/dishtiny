#pragma once
#ifndef UITSL_FETCH_FETCH_WEB_HPP_INCLUDE
#define UITSL_FETCH_FETCH_WEB_HPP_INCLUDE

#include "../polyfill/filesystem.hpp"

#include "make_temp_filepath.hpp"
#include "resolve_fetched_filename.hpp"

namespace uitsl {

/*
 * Synchronously fetch data from a url to a temporary file,
 * return temporary file path. Requires `xmlhttprequest` package when running
 * with Node.js.
 */
inline std::filesystem::path fetch_web( const std::string& url ) {

  const std::filesystem::path bodypath{ uitsl::make_temp_filepath() };
  const std::filesystem::path headerpath{ uitsl::make_temp_filepath() };

  // has to run on the main thread in order to write to main thread filesystem,
  // where all C filesystem access calls are proxied to
  // see https://github.com/emscripten-core/emscripten/issues/8624
  MAIN_THREAD_EM_ASM({

    // adapted from https://stackoverflow.com/a/11058858
    // and https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/Sending_and_Receiving_Binary_Data#receiving_binary_data_in_older_browsers
    function str2ab(str) {
      var buf = new ArrayBuffer(str.length);
      var bufView = new Uint8Array(buf);
      for (var i=0, strLen=str.length; i<strLen; i++) {
        bufView[i] = str.charCodeAt(i) & 0xff;
      }
      return buf;
    }

    function do_fetch(url, bodypath, headerpath) {

      console.log("fetching url", url);

      var data;
      var headers;
      if ( typeof XMLHttpRequest == "undefined" ) {
        console.log("nodejs environment detected");
        var request = require("sync-request");
        var res = request("GET", url, {
          gzip: false,
          headers: { "X-Requested-With" : "XMLHttpRequest" },
          // see https://stackoverflow.com/a/14855016
          encoding: null,
        });
        data = res.body;
        headers = Object.entries(res.headers).map(
          (kv, i) => `${kv[0]}: ${kv[1]}`
        ).join("\n");
      } else {
        console.log("browser environment detected");
        var xhr = new XMLHttpRequest();
        xhr.open("GET", url, false);  // synchronous request
        xhr.setRequestHeader( "X-Requested-With", "XMLHttpRequest" );
        // adapted from https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/Sending_and_Receiving_Binary_Data#receiving_binary_data_in_older_browsers
        // XHR binary charset opt by Marcus Granado 2006 [http://mgran.blogspot.com]
        xhr.overrideMimeType("text\/plain; charset=x-user-defined");
        xhr.send(null);
        console.log( "request status", xhr.status);
        data = str2ab( xhr.responseText );
        headers = xhr.getAllResponseHeaders();
      }

      console.log( headers );

      var arr = new Uint8Array(data);
      console.log( "array length", arr.length );
      var bodystream = FS.open(bodypath, 'w');
      FS.write(bodystream, arr, 0, arr.length, 0);
      FS.close(bodystream);

      FS.writeFile( headerpath, headers );
    }

    try {
      do_fetch(
        UTF8ToString($0),
        UTF8ToString($1),
        UTF8ToString($2)
      );
    } catch (error) {
      console.log("trying cors proxy to allow cross-origin access in browser");
      do_fetch(
        "https://cors.bridged.cc/" + UTF8ToString($0),
        UTF8ToString($1),
        UTF8ToString($2)
      );
    }

  }, url.c_str(), bodypath.c_str(), headerpath.c_str() );

  return uitsl::resolve_fetched_filename( url, bodypath, headerpath );

}

} // namespace uitsl

#endif // #ifndef UITSL_FETCH_FETCH_WEB_HPP_INCLUDE
