#pragma once
#ifndef DISH2_WEB_SAVED_FOLDER_HPP_INCLUDE
#define DISH2_WEB_SAVED_FOLDER_HPP_INCLUDE

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <numeric>
#include <optional>
#include <ostream>
#include <set>
#include <string>

#include <emscripten.h>

#include "../../../third-party/bxzstr/include/bxzstr.hpp"
#include "../../../third-party/cereal/include/cereal/external/rapidjson/ostreamwrapper.h"
#include "../../../third-party/cereal/include/cereal/external/rapidjson/prettywriter.h"
#include "../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/web/Canvas.hpp"

#include "../config/cfg.hpp"
#include "../spec/_NLEV.hpp"

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

// file names for the data file and the files that describe it
inline std::string data_name() {
  return emp::keyname::pack({
    {"a", "saved"}, {"what", "dishtiny"}, {"ext", ".jsonl.gz"}
  });
}

inline std::string schema_name() {
  return emp::keyname::pack({
    {"a", "saved"}, {"what", "schema"}, {"ext", ".json"}
  });
}

inline std::string metadata_name() {
  return emp::keyname::pack({
    {"a", "saved"}, {"what", "metadata"}, {"ext", ".json"}
  });
}

// what has been saved to the data file, for the schema and metadata
struct SavedViewer {
  std::string slug;
  std::set<size_t> series;
};
inline std::map<std::string, SavedViewer> saved_viewers;
inline std::set<size_t> saved_updates;

// records that a viewer (and series, if it has them) was saved at an update
inline void note_saved(
  const std::string& artist_name,
  const std::string& slug,
  const size_t update,
  const std::optional<size_t> series=std::nullopt
) {
  auto& viewer = dish2::saved_folder::saved_viewers[ artist_name ];
  viewer.slug = slug;
  if ( series ) viewer.series.insert( *series );
  dish2::saved_folder::saved_updates.insert( update );
}

// the description of a view from the page's view keys, or empty if none
inline std::string get_view_key_description( const std::string& slug ) {

  char* const buffer = reinterpret_cast<char*>( MAIN_THREAD_EM_ASM_INT({
    let text = '';
    const element = document.getElementById( 'view-key-descriptions' );
    if ( element !== null ) {
      text = JSON.parse( element.textContent )[ UTF8ToString( $0 ) ] || '';
    }
    const num_bytes = lengthBytesUTF8( text ) + 1;
    const buffer = Module._malloc( num_bytes );
    stringToUTF8( text, buffer, num_bytes );
    return buffer;
  }, slug.c_str() ) );

  const std::string res( buffer );
  std::free( buffer );
  return res;

}

// current time as an ISO 8601 string
inline std::string get_utc_timestamp() {

  char* const buffer = reinterpret_cast<char*>( MAIN_THREAD_EM_ASM_INT({
    const text = new Date().toISOString();
    const num_bytes = lengthBytesUTF8( text ) + 1;
    const buffer = Module._malloc( num_bytes );
    stringToUTF8( text, buffer, num_bytes );
    return buffer;
  }) );

  const std::string res( buffer );
  std::free( buffer );
  return res;

}

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

// (re)writes the schema and metadata files that describe the data file
inline void write_schema_and_metadata() {

  dish2::saved_folder::make();

  using writer_t = rapidjson::PrettyWriter<rapidjson::OStreamWrapper>;

  const auto write_string = []( writer_t& writer, const std::string& str ){
    writer.String( str.c_str(), str.size() );
  };

  const auto write_field = [&](
    writer_t& writer, const std::string& name, const std::string& description
  ){
    writer.Key( name.c_str() );
    writer.StartObject();
    writer.Key( "description" );
    write_string( writer, description );
    writer.EndObject();
  };

  {
    std::ofstream file(
      ( dish2::saved_folder::directory / dish2::saved_folder::schema_name()
      ).string()
    );
    rapidjson::OStreamWrapper wrapper{ file };
    writer_t writer{ wrapper };

    writer.StartObject();

    writer.Key( "data_file" );
    write_string( writer, dish2::saved_folder::data_name() );
    writer.Key( "format" );
    write_string( writer,
      "gzip-compressed JSON Lines, with one record per line"
    );

    // each record has these fields
    writer.Key( "fields" );
    writer.StartObject();
    write_field( writer, "update", "Simulation update the view was saved at." );
    write_field( writer, "series",
      "Which series of the viewer was saved, for viewers with multiple series."
    );
    write_field( writer, "artist", "Name of the viewer, as a key in viewers." );
    write_field( writer, "cell", "Index of the cell the value is from." );
    write_field( writer, "cardinal",
      "Index of the cardinal the value is from, for per-cardinal values."
    );
    write_field( writer, "attribute", "What the value is a measurement of." );
    write_field( writer, "value", "The value that was measured." );
    writer.EndObject();

    // each viewer that was saved is described using its view key
    writer.Key( "viewers" );
    writer.StartObject();
    for ( const auto& [name, viewer] : dish2::saved_folder::saved_viewers ) {
      writer.Key( name.c_str() );
      writer.StartObject();
      writer.Key( "description" );
      write_string(
        writer, dish2::saved_folder::get_view_key_description( viewer.slug )
      );
      if ( !viewer.series.empty() ) {
        writer.Key( "series" );
        writer.StartArray();
        for ( const size_t i : viewer.series ) writer.Uint64( i );
        writer.EndArray();
      }
      writer.EndObject();
    }
    writer.EndObject();

    writer.EndObject();
  }

  {
    std::ofstream file(
      ( dish2::saved_folder::directory / dish2::saved_folder::metadata_name()
      ).string()
    );
    rapidjson::OStreamWrapper wrapper{ file };
    writer_t writer{ wrapper };

    writer.StartObject();

    writer.Key( "software" );
    write_string( writer, "dishtiny" );
    writer.Key( "software_version" );
    write_string( writer, EMP_STRINGIFY(DISHTINY_HASH_) );
    writer.Key( "nlev" );
    writer.Uint( DISH2_NLEV );
    writer.Key( "config" );
    writer.StartObject();
    const std::map<
      std::string, decltype( dish2::cfg.begin()->second )
    > alph( dish2::cfg.begin(), dish2::cfg.end() );
    for ( const auto& [name, entry] : alph ) {
      writer.Key( name.c_str() );
      write_string( writer, entry->GetValue() );
    }
    writer.EndObject();
    writer.Key( "browser" );
    const std::string browser = dish2::saved_folder::get_browser_info_json();
    writer.RawValue( browser.c_str(), browser.size(), rapidjson::kObjectType );
    writer.Key( "timestamp" );
    write_string( writer, dish2::saved_folder::get_utc_timestamp() );
    writer.Key( "data_file" );
    write_string( writer, dish2::saved_folder::data_name() );
    writer.Key( "schema_file" );
    write_string( writer, dish2::saved_folder::schema_name() );

    writer.Key( "saved_updates" );
    writer.StartArray();
    for ( const size_t update : dish2::saved_folder::saved_updates ) {
      writer.Uint64( update );
    }
    writer.EndArray();

    writer.Key( "saved_viewers" );
    writer.StartArray();
    for ( const auto& [name, viewer] : dish2::saved_folder::saved_viewers ) {
      write_string( writer, name );
    }
    writer.EndArray();

    writer.EndObject();
  }

}

// calls write to appends to the data file in the saved folder
inline void save_data( const std::function<void( std::ostream& )>& write ) {

  dish2::saved_folder::make();

  const std::string name = dish2::saved_folder::data_name();

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
  dish2::saved_folder::saved_viewers.clear();
  dish2::saved_folder::saved_updates.clear();

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
