#pragma once
#ifndef DISH2_DEBUG_POPULATIONEXTINCTIONEXCEPTION_HPP_INCLUDE
#define DISH2_DEBUG_POPULATIONEXTINCTIONEXCEPTION_HPP_INCLUDE

#include <exception>
#include <sstream>
#include <string>
#include <unordered_map>

#include "../../../third-party/conduit/include/uitsl/utility/unindent_raw_string_literal.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

namespace dish2 {

class PopulationExtinctionException : public std::exception {

public:
  size_t update;

  size_t population_size;

  std::unordered_map< std::string, size_t > cause_of_death_counts;

private:
  std::string what_;

public:

  PopulationExtinctionException(
    const size_t update_, const size_t population_size_,
    const std::unordered_map< std::string, size_t >& cause_of_death_counts_
  ) : update( update_ )
  , population_size( population_size_ )
  , cause_of_death_counts( cause_of_death_counts_ ) {
    std::stringstream ss;

    ss << "Population Extinction Exception" << '\n';
    ss << "  update " << update << '\n';
    ss << "  population size " << population_size << '\n';
    for ( const auto& [k, v] : cause_of_death_counts ) {
      ss << "  " << k << " " << v << '\n';
    }


    what_ = ss.str();

  }

  const char* what () const noexcept {
    return what_.c_str();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_POPULATIONEXTINCTIONEXCEPTION_HPP_INCLUDE
