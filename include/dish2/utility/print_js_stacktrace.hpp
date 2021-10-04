#pragma once
#ifndef DISH2_UTILITY_PRINT_JS_STACKTRACE_HPP_INCLUDE
#define DISH2_UTILITY_PRINT_JS_STACKTRACE_HPP_INCLUDE

#include <emscripten.h>

namespace dish2 {

void print_js_stacktrace() {

  // provided via Emscripten
  // see https://emscripten.org/docs/porting/Debugging.html#manual-print-debugging
  MAIN_THREAD_EM_ASM({ stackTrace(); });

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_PRINT_JS_STACKTRACE_HPP_INCLUDE
