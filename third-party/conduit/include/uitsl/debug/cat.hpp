#pragma once
#ifndef UITSL_DEBUG_CAT_HPP_INCLUDE
#define UITSL_DEBUG_CAT_HPP_INCLUDE

#include <fstream>
#include <iostream>

#include "../polyfill/filesystem.hpp"

namespace uitsl {

void cat( const std::filesystem::path& target ) {
  std::ifstream stream( target );
  std::cout << stream.rdbuf();
}

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_CAT_HPP_INCLUDE
