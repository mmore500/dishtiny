#pragma once
#ifndef UITSL_UTILITY_UNINDENT_RAW_STRING_LITERAL_HPP_INCLUDE
#define UITSL_UTILITY_UNINDENT_RAW_STRING_LITERAL_HPP_INCLUDE


#include <cctype>
#include <string>

namespace uitsl {

// adapted from https://stackoverflow.com/a/24900770
inline std::string unindent_raw_string_literal(const char* p) {

  if (*p == '\n') ++p;

  const char* p_leading = p;
  while (std::isspace(*p) && *p != '\n') ++p;
  const size_t leading_len = p - p_leading;

  std::string result;
  while (*p) {

    result += *p;
    if (*p++ == '\n') {
      for (size_t i = 0; i < leading_len; ++i) {
        if (p[i] != p_leading[i]) goto dont_skip_leading;
      }
      p += leading_len;
    }

    dont_skip_leading: ;

  }

  return result;

}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_UNINDENT_RAW_STRING_LITERAL_HPP_INCLUDE
