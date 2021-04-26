#pragma once
#ifndef UITSL_UTILITY_PRINT_UTILS_HPP_INCLUDE
#define UITSL_UTILITY_PRINT_UTILS_HPP_INCLUDE

#include <mutex>
#include <regex>
#include <sstream>
#include <string>

#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

// TODO be clever and use fewer overloads

namespace uitsl {

inline std::mutex error_message_mutex;

inline std::string apply_indent(std::string in) {
  return std::regex_replace(
    in,
    std::regex{"(^|\n)"},
    std::string{"$1"} + std::string(3, ' ')
  );
}

template<typename T>
inline std::string format_member(
  const std::string & name,
  const T& member
) {
  std::stringstream ss;
  ss << name << ":" << '\n';
  ss << uitsl::apply_indent(member.ToString());
  return ss.str();
}

template<>
inline std::string format_member<std::string>(
  const std::string & name,
  const std::string& member
) {
  std::stringstream ss;
  ss << name << ":" << '\n';
  ss << apply_indent(member);
  return ss.str();
}

template<>
inline std::string format_member<char>(
  const std::string & name,
  const char& member
) {
  std::stringstream ss;
  ss << name << ":" << '\n';
  ss << apply_indent(
    [member](){ std::stringstream ss; ss << member; return ss.str(); }()
  );
  return ss.str();
}

template<>
inline std::string format_member<size_t>(
  const std::string & name,
  const size_t& member
) {
  std::stringstream ss;
  ss << name << ":" << '\n';
  ss << apply_indent(
    std::to_string(member)
  );
  return ss.str();
}

template<>
inline std::string format_member<int>(
  const std::string & name,
  const int& member
) {
  std::stringstream ss;
  ss << name << ":" << '\n';
  ss << apply_indent(
    std::to_string(member)
  );
  return ss.str();
}

template<>
inline std::string format_member<bool>(
  const std::string & name,
  const bool& member
) {
  std::stringstream ss;
  ss << name << ":" << '\n';
  ss << apply_indent(
    std::to_string(member)
  );
  return ss.str();
}

template<>
inline std::string format_member<const void *>(
  const std::string & name,
  const void* const& member
) {
  std::stringstream ss;
  ss << name << ":" << '\n';
  ss << apply_indent(
    [member](){ std::stringstream ss; ss << member; return ss.str(); }()
  );
  return ss.str();
}

template<typename T>
std::string to_string(const T & container) {
  emp::vector<std::string> res;
  std::transform(
    std::begin(container),
    std::end(container),
    std::back_inserter(res),
    [](const auto & x){ return emp::to_string(x); }
  );
  return emp::join_on(res, ", ");
}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_PRINT_UTILS_HPP_INCLUDE
