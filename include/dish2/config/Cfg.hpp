#pragma once
#ifndef DISH2_CONFIG_CFG_HPP_INCLUDE
#define DISH2_CONFIG_CFG_HPP_INCLUDE

namespace dish2 {

struct Cfg {

  static size_t N_THREADS() { return 1; }

  static size_t N_PROCS() { return 1; }

  static size_t N_CELLS() { return 100; }

};

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_CFG_HPP_INCLUDE
