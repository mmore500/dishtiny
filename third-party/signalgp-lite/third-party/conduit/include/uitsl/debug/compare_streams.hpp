#pragma once
#ifndef UITSL_DEBUG_COMPARE_STREAMS_HPP_INCLUDE
#define UITSL_DEBUG_COMPARE_STREAMS_HPP_INCLUDE

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

namespace uitsl {

/*
 * Return true if streams are equal.
 */
inline bool compare_streams(std::istream& s1, std::istream& s2) {

  // seek back to beginning and use std::equal to compare contents
  s1.seekg(0, std::istream::beg);
  s2.seekg(0, std::istream::beg);

  if (s1.fail() || s2.fail()) throw( "stream failure" );

  // size mismatch
  if (s1.tellg() != s2.tellg()) return false;

  s1.seekg(0, std::istream::beg);
  s2.seekg(0, std::istream::beg);

  return std::equal(
    std::istreambuf_iterator<char>(s1.rdbuf()),
    std::istreambuf_iterator<char>(),
    std::istreambuf_iterator<char>(s2.rdbuf())
  );

}

/*
 * Return true if streams are equal.
 */
inline bool compare_streams(std::istream&& s1, std::istream&& s2) {

  return compare_streams(s1, s2);

}

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_COMPARE_STREAMS_HPP_INCLUDE
