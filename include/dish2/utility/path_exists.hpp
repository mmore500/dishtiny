#pragma once
#ifndef DISH2_UTILITY_PATH_EXISTS_HPP_INCLUDE
#define DISH2_UTILITY_PATH_EXISTS_HPP_INCLUDE

#include <string>

#include <sys/stat.h>

namespace dish2 {

// std::filesystem::exists is failing inside Docker container
// so use stat as a backup for now
bool path_exists(const std::string& filename) {
  struct stat buffer;
  return stat(filename.c_str(), &buffer) == 0;
}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_PATH_EXISTS_HPP_INCLUDE
