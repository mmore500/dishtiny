#pragma once
#ifndef DISH2_UTILITY_POPULATIONEXTINCTIONEXCEPTION_HPP_INCLUDE
#define DISH2_UTILITY_POPULATIONEXTINCTIONEXCEPTION_HPP_INCLUDE

#include <exception>
#include <sstream>

#include "../../../third-party/conduit/include/uitsl/utility/unindent_raw_string_literal.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

namespace dish2 {

class PopulationExtinctionException : public std::exception {

public:
  size_t update;

  size_t population_size;

private:
  std::string what_;

public:

  PopulationExtinctionException(
    const size_t update_, const size_t population_size_
  ) : update( update_ )
  , population_size( population_size_ ) {
    std::stringstream ss;

    ss << "Population Extinction Exception" << std::endl;
    ss << "  update " << update << std::endl;
    ss << "  population size " << population_size << std::endl;

    what_ = ss.str();

  }

	const char* what () const noexcept {
    return what_.c_str();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_POPULATIONEXTINCTIONEXCEPTION_HPP_INCLUDE
