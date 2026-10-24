#pragma once
#ifndef DISH2_UTILITY_SAVE_TEXT_HPP_INCLUDE
#define DISH2_UTILITY_SAVE_TEXT_HPP_INCLUDE

#include <filesystem>
#include <fstream>
#include <string>

namespace dish2 {

// (over)writes a file with text, making its parent folders if needed
inline void save_text(
  const std::filesystem::path& path, const std::string& text
) {

  std::filesystem::create_directories( path.parent_path() );

  std::ofstream( path ) << text;

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_SAVE_TEXT_HPP_INCLUDE
