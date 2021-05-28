#pragma once
#ifndef UITSL_DEBUG_LIST_CWD_HPP_INCLUDE
#define UITSL_DEBUG_LIST_CWD_HPP_INCLUDE

#include <filesystem>
#include <iostream>

namespace uitsl {

void list_cwd(std::ostream& os=std::cout) {
  os << std::filesystem::absolute(".") << '\n';
  for (auto& p: std::filesystem::directory_iterator(".")) {
    os << p.path() << '\n';
  }
}

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_LIST_CWD_HPP_INCLUDE
