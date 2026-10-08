#pragma once
#ifndef DISH2_VIZ_UTIL_PARSE_LABEL_FIELDS_JSON_HPP_INCLUDE
#define DISH2_VIZ_UTIL_PARSE_LABEL_FIELDS_JSON_HPP_INCLUDE

#include <string>

#include "../../../../third-party/cereal/include/cereal/archives/json.hpp"
#include "../../../../third-party/cereal/include/cereal/external/rapidjson/document.h"
#include "../../../../third-party/Empirical/include/emp/base/assert.hpp"

namespace dish2 {

/**
 * Parses a json dict of fields (e.g., {"update": 7}) for write_jsonl_record.
 */
inline rapidjson::Document parse_label_fields_json(
  const std::string& label_fields_json
) {

  rapidjson::Document label_fields;
  label_fields.Parse( label_fields_json.c_str() );
  emp_assert( label_fields.IsObject(), label_fields_json );

  return label_fields;

}

} // namespace dish2

#endif // #ifndef DISH2_VIZ_UTIL_PARSE_LABEL_FIELDS_JSON_HPP_INCLUDE
