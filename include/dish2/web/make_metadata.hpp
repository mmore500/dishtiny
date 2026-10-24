#pragma once
#ifndef DISH2_WEB_MAKE_METADATA_HPP_INCLUDE
#define DISH2_WEB_MAKE_METADATA_HPP_INCLUDE

#include <chrono>
#include <map>
#include <string>

#include "../../../third-party/cereal/include/cereal/archives/json.hpp"
#include "../../../third-party/cereal/include/cereal/external/rapidjson/prettywriter.h"
#include "../../../third-party/cereal/include/cereal/external/rapidjson/stringbuffer.h"
#include "../../../third-party/Empirical/include/emp/base/macros.hpp"

#include "../config/cfg.hpp"
#include "../spec/_NLEV.hpp"

#include "get_browser_info_json.hpp"
#include "saved_folder.hpp"

namespace dish2 {

// makes JSON that describes the software, configuration, and browser that
// made the saved data
inline std::string make_metadata() {

  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer{ buffer };

  const auto write_string = [&writer]( const std::string& str ){
    writer.String( str.c_str(), str.size() );
  };

  writer.StartObject();

  writer.Key( "software" );
  write_string( "dishtiny" );
  writer.Key( "software_version" );
  write_string( EMP_STRINGIFY(DISHTINY_HASH_) );
  writer.Key( "nlev" );
  writer.Uint( DISH2_NLEV );

  writer.Key( "config" );
  writer.StartObject();
  const std::map<
    std::string, decltype( dish2::cfg.begin()->second )
  > alph( dish2::cfg.begin(), dish2::cfg.end() );
  for ( const auto& [name, entry] : alph ) {
    writer.Key( name.c_str() );
    write_string( entry->GetValue() );
  }
  writer.EndObject();

  writer.Key( "browser" );
  const std::string browser = dish2::get_browser_info_json();
  writer.RawValue( browser.c_str(), browser.size(), rapidjson::kObjectType );

  writer.Key( "epoch_seconds" );
  writer.Int64( std::chrono::duration_cast<std::chrono::seconds>(
    std::chrono::system_clock::now().time_since_epoch()
  ).count() );

  writer.Key( "data_file" );
  write_string( dish2::saved_folder::data_name() );
  writer.Key( "schema_file" );
  write_string( dish2::saved_folder::schema_name() );

  writer.EndObject();

  return buffer.GetString();

}

} // namespace dish2

#endif // #ifndef DISH2_WEB_MAKE_METADATA_HPP_INCLUDE
