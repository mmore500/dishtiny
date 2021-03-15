#pragma once
#ifndef DISH2_UTILITY_GET_HOSTNAME_HPP_INCLUDE
#define DISH2_UTILITY_GET_HOSTNAME_HPP_INCLUDE

#include <string>

#include <unistd.h>

#include "../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"

namespace dish2 {

std::string get_hostname() {

  char res[HOST_NAME_MAX + 1];
  uitsl::err_verify( gethostname(res, HOST_NAME_MAX + 1) );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_GET_HOSTNAME_HPP_INCLUDE
