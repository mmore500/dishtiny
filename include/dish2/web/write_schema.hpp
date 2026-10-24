#pragma once
#ifndef DISH2_WEB_WRITE_SCHEMA_HPP_INCLUDE
#define DISH2_WEB_WRITE_SCHEMA_HPP_INCLUDE

#include <ostream>
#include <string>

#include "../../../third-party/cereal/include/cereal/external/rapidjson/ostreamwrapper.h"
#include "../../../third-party/cereal/include/cereal/external/rapidjson/prettywriter.h"

#include "get_view_keys_json.hpp"
#include "saved_folder.hpp"

namespace dish2 {

// describes the records in the saved data file and the views they come from
inline void write_schema( std::ostream& out ) {

  using writer_t = rapidjson::PrettyWriter<rapidjson::OStreamWrapper>;

  rapidjson::OStreamWrapper wrapper{ out };
  writer_t writer{ wrapper };

  const auto write_string = [&writer]( const std::string& str ){
    writer.String( str.c_str(), str.size() );
  };

  const auto write_field = [&](
    const std::string& name, const std::string& description
  ){
    writer.Key( name.c_str() );
    writer.StartObject();
    writer.Key( "description" );
    write_string( description );
    writer.EndObject();
  };

  writer.StartObject();

  writer.Key( "data_file" );
  write_string( dish2::saved_folder::data_name() );
  writer.Key( "format" );
  write_string( "gzip-compressed JSON Lines, with one record per line" );

  // each record has these fields
  writer.Key( "fields" );
  writer.StartObject();
  write_field( "update", "Simulation update the view was saved at." );
  write_field(
    "series",
    "Index of the subview within a viewer that has several, "
    "e.g., a register address or a module."
  );
  write_field(
    "artist", "Name of the viewer, whose slug is a key in view_keys."
  );
  write_field( "cell", "Index of the cell the value is from." );
  write_field(
    "cardinal",
    "Index of the cardinal the value is from, for per-cardinal values."
  );
  write_field( "attribute", "What the value is a measurement of." );
  write_field( "value", "The value that was measured." );
  writer.EndObject();

  // views are described by their view keys, which are keyed by the slug of
  // the viewer name given in the artist field
  writer.Key( "view_keys" );
  const std::string view_keys = dish2::get_view_keys_json();
  writer.RawValue( view_keys.c_str(), view_keys.size(), rapidjson::kObjectType );

  writer.EndObject();

}

} // namespace dish2

#endif // #ifndef DISH2_WEB_WRITE_SCHEMA_HPP_INCLUDE
