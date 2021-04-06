#pragma once
#ifndef DISH2_DEBUG_LOG_TEE_HPP_INCLUDE
#define DISH2_DEBUG_LOG_TEE_HPP_INCLUDE

#include "LogTee.hpp"

namespace dish2 {

thread_local dish2::LogTee log_tee;

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOG_TEE_HPP_INCLUDE
