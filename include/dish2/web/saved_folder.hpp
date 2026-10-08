#pragma once
#ifndef DISH2_WEB_SAVED_FOLDER_HPP_INCLUDE
#define DISH2_WEB_SAVED_FOLDER_HPP_INCLUDE

#include <filesystem>
#include <functional>
#include <numeric>
#include <ostream>
#include <string>

#include <emscripten.h>

#include "../../../third-party/bxzstr/include/bxzstr.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/web/Canvas.hpp"

namespace dish2::saved_folder {

// in-memory folder (Emscripten MEMFS) that saved files are written to
inline const std::filesystem::path directory{ "/saved" };

inline void make() {
  std::filesystem::create_directories( dish2::saved_folder::directory );
}

// writes a canvas to the folder as a png
inline void save_png(
  const emp::web::Canvas& canvas, const std::string& name
) {

  dish2::saved_folder::make();

  MAIN_THREAD_EM_ASM({
    const url = document.getElementById( UTF8ToString( $0 ) ).toDataURL(
      'image/png'
    );
    const binary = atob( url.split( ',' )[1] );
    const bytes = Uint8Array.from( binary, c => c.charCodeAt( 0 ) );
    FS.writeFile( UTF8ToString( $1 ), bytes );
  },
    canvas.GetID().c_str(),
    ( dish2::saved_folder::directory / name ).c_str()
  );

}

// calls write to appends to the data file in the saved folder
inline void save_data( const std::function<void( std::ostream& )>& write ) {

  dish2::saved_folder::make();

  const std::string name = emp::keyname::pack({
    {"a", "saved"}, {"what", "dishtiny"}, {"ext", ".jsonl.gz"}
  });

  // closed (and so finished) when out goes out of scope
  // multiple gzip closes OK
  bxz::ofstream out(
    ( dish2::saved_folder::directory / name ).string(),
    std::ios_base::app, bxz::z, 6
  );
  write( out );

}

// total size of the files in the folder
inline double get_num_bytes() {

  dish2::saved_folder::make();

  return std::accumulate(
    std::filesystem::recursive_directory_iterator(
      dish2::saved_folder::directory
    ),
    std::filesystem::recursive_directory_iterator{},
    0.0,
    []( const double total, const std::filesystem::directory_entry& entry ){
      return total + ( entry.is_regular_file() ? entry.file_size() : 0 );
    }
  );

}

// deletes the files in the folder
inline void clear() {

  std::filesystem::remove_all( dish2::saved_folder::directory );
  dish2::saved_folder::make();

}

// downloads the files in the folder as a zip
// the download button shows progress while the zip is made
inline void download( const std::string& name ) {

  dish2::saved_folder::make();

  MAIN_THREAD_EM_ASM({
    const filename = UTF8ToString( $0 );
    const directory = UTF8ToString( $1 );
    const button = document.getElementById( 'download-saved-button' );
    const label = button.textContent;
    button.disabled = true;
    button.textContent = 'Zipping...';
    const restore = function() {
      button.textContent = label;
      button.disabled = false;
    };
    // wait a moment so the button updates before the work starts
    setTimeout( function() {
      try {
        const zip = new JSZip();
        for ( const name of FS.readdir( directory ) ) {
          if ( name === '.' || name === '..' ) continue;
          zip.file( name, FS.readFile( directory + '/' + name ) );
        }
        zip.generateAsync(
          { type: 'blob' },
          function( metadata ) {
            const percent = Math.round( metadata.percent );
            button.textContent = 'Zipping ' + percent + '%';
          }
        ).then( function( blob ) {
          const link = document.createElement( 'a' );
          link.href = URL.createObjectURL( blob );
          link.download = filename;
          document.body.appendChild( link );
          link.click();
          document.body.removeChild( link );
          setTimeout( function() { URL.revokeObjectURL( link.href ); }, 1000 );
          restore();
        }, function( error ) {
          console.error( error );
          restore();
        });
      } catch ( error ) {
        console.error( error );
        restore();
      }
    }, 50 );
  }, name.c_str(), dish2::saved_folder::directory.c_str() );

}

} // namespace dish2::saved_folder

#endif // #ifndef DISH2_WEB_SAVED_FOLDER_HPP_INCLUDE
