#pragma once
#ifndef DISH2_PREFAB_MODALGUARD_HPP_INCLUDE
#define DISH2_PREFAB_MODALGUARD_HPP_INCLUDE

#include <string>

#include <emscripten.h>

namespace dish2 {

struct ModalGuard {

  std::string target_id;

  ModalGuard( const std::string target_id_ )
  : target_id( target_id_ ) {

    MAIN_THREAD_EM_ASM({
      $(`#${UTF8ToString($0)}`).modal('show');
    }, target_id.c_str());

  }

  ~ModalGuard() {
    MAIN_THREAD_EM_ASM({
      $(`#${UTF8ToString($0)}`).modal('hide');
    }, target_id.c_str());
  }

};

} // namespace dish2



#endif // #ifndef DISH2_PREFAB_MODALGUARD_HPP_INCLUDE
