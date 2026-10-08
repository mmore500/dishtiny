#pragma once
#ifndef DISH2_VIZ_UTIL_WRITE_JSONL_RECORD_HPP_INCLUDE
#define DISH2_VIZ_UTIL_WRITE_JSONL_RECORD_HPP_INCLUDE

#include <algorithm>
#include <cstddef>
#include <functional>
#include <optional>
#include <ostream>
#include <string>
#include <tuple>
#include <type_traits>

#include "../../../../third-party/cereal/include/cereal/archives/json.hpp"
#include "../../../../third-party/cereal/include/cereal/external/rapidjson/document.h"
#include "../../../../third-party/cereal/include/cereal/external/rapidjson/stringbuffer.h"
#include "../../../../third-party/cereal/include/cereal/external/rapidjson/writer.h"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../genome/Genome.hpp"
#include "../../genome/KinGroupID.hpp"

namespace dish2 {

namespace internal {

template< typename Value, typename = void >
struct is_leaf_node : std::false_type {};

// detect uitsl::PodLeafNode and types derived from it
template< typename Value >
struct is_leaf_node<
  Value, std::void_t< decltype( Value::IsLeaf() ) >
> : std::bool_constant< Value::IsLeaf() > {};

// forward declarations
template< typename Writer, typename Value >
void write_jsonl_record_value(
  Writer& writer, const Value& value
);

template< typename Writer, typename... Args >
void write_jsonl_record_value(
  Writer& writer, const std::tuple<Args...>& value
);

template< typename Writer, typename Spec >
void write_jsonl_record_value(
  Writer& writer, const dish2::KinGroupID<Spec>& value
);

template< typename Writer, typename Spec >
void write_jsonl_record_value(
  Writer& writer, const dish2::Genome<Spec>& value
);

// definitions
template< typename Writer, typename Value >
void write_jsonl_record_value(
  Writer& writer, const Value& value
) {

  if constexpr ( std::is_same_v< Value, bool > ) {
    writer.Bool( value );
  } else if constexpr ( std::is_integral_v< Value > ) {
    if constexpr ( std::is_signed_v< Value > ) writer.Int64( value );
    else writer.Uint64( value );
  } else if constexpr ( std::is_floating_point_v< Value > ) {
    writer.Double( value );
  } else if constexpr ( is_leaf_node<Value>::value ) {
    if constexpr ( Value::GetSize() == 1 ) {
      dish2::internal::write_jsonl_record_value( writer, value.Get( 0 ) );
    } else {
      writer.StartArray();
      std::for_each(
        std::begin( value ), std::end( value ),
        [&writer]( const auto& element ){
          dish2::internal::write_jsonl_record_value( writer, element );
        }
      );
      writer.EndArray();
    }
  } else {
    writer.String( emp::to_string( value ).c_str() );
  }

}

template< typename Writer, typename... Args >
void write_jsonl_record_value(
  Writer& writer, const std::tuple<Args...>& value
) {

  writer.StartArray();
  std::apply(
    [&writer]( const auto&... elements ){
      ( dish2::internal::write_jsonl_record_value( writer, elements ), ... );
    },
    value
  );
  writer.EndArray();

}

template< typename Writer, typename Spec >
void write_jsonl_record_value(
  Writer& writer, const dish2::KinGroupID<Spec>& value
) {

  writer.StartArray();
  for ( const auto id : value.data ) writer.Uint64( id );
  writer.EndArray();

}

template< typename Writer, typename Spec >
void write_jsonl_record_value(
  Writer& writer, const dish2::Genome<Spec>& value
) {

  writer.Uint64( std::hash< dish2::Genome<Spec> >{}( value ) );

}

} // namespace internal

/**
 * Writes one EAV entry, for viewer tabulations.
 *
 * The provided fields (e.g., the update) are written at the start of the entry.
 */
template< typename Value >
void write_jsonl_record(
  std::ostream& out,
  const rapidjson::Document& label_fields,
  const size_t cell,
  const std::optional<size_t> cardinal,
  const std::string& attribute,
  const Value& value
) {

  // written to a buffer because writing straight to the stream flushes it
  // after every record, which would make a gzip stream of tiny blocks
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<
    rapidjson::StringBuffer,
    rapidjson::UTF8<>,
    rapidjson::UTF8<>,
    rapidjson::CrtAllocator,
    rapidjson::kWriteNanAndInfFlag
  > writer{ buffer };

  writer.StartObject();

  for ( const auto& field : label_fields.GetObject() ) {
    writer.Key( field.name.GetString(), field.name.GetStringLength() );
    field.value.Accept( writer );
  }

  writer.Key( "cell" );
  writer.Uint64( cell );

  if ( cardinal ) {
    writer.Key( "cardinal" );
    writer.Uint64( *cardinal );
  }

  writer.Key( "attribute" );
  writer.String( attribute.c_str() );

  writer.Key( "value" );
  dish2::internal::write_jsonl_record_value( writer, value );

  writer.EndObject();

  out << buffer.GetString() << '\n';

}

} // namespace dish2

#endif // #ifndef DISH2_VIZ_UTIL_WRITE_JSONL_RECORD_HPP_INCLUDE
