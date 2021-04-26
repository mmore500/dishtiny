#pragma once
#ifndef UITSL_UTILITY_STREAM_UTILS_HPP_INCLUDE
#define UITSL_UTILITY_STREAM_UTILS_HPP_INCLUDE

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

namespace uitsl {

class Line {
  std::string str;
public:
  Line() = default;

  operator std::string() const { return str; }
  friend std::istream& operator>>(std::istream&, Line&);
};

inline std::istream& operator>>(std::istream& is, Line& line) {
  std::getline(is, line.str);
  return is;
}

template <typename Out>
void read_lines(std::istream & is, Out out) {
  using In = std::istream_iterator<uitsl::Line>;
  std::copy(In(is), In(), out);
}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_STREAM_UTILS_HPP_INCLUDE
