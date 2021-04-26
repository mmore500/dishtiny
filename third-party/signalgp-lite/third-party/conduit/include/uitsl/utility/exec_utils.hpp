#pragma once
#ifndef UITSL_UTILITY_EXEC_UTILS_HPP_INCLUDE
#define UITSL_UTILITY_EXEC_UTILS_HPP_INCLUDE

#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "../../../third-party/Empirical/include/emp/base/array.hpp"

namespace uitsl {

// adapted from https://stackoverflow.com/a/478960
inline std::string exec(const char* cmd) {
    emp::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_EXEC_UTILS_HPP_INCLUDE
