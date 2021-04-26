#pragma once
#ifndef DISH2_LOAD_GET_INNOCULUM_SLUG_HPP_INCLUDE
#define DISH2_LOAD_GET_INNOCULUM_SLUG_HPP_INCLUDE

#include "get_innoculum_filename.hpp"
#include "make_genome_slug.hpp"

namespace dish2 {

std::string get_innoculum_slug( const size_t root_id ) {

  return dish2::make_genome_slug( dish2::get_innoculum_filename( root_id ) );

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_GET_INNOCULUM_SLUG_HPP_INCLUDE
