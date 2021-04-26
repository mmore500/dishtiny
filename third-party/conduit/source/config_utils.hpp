#pragma once

#include <assert.h>
#include <iostream>
#include <limits>
#include <new>
#include <string>
#include <unordered_map>

#include "../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "uitsl/math/math_utils.hpp"

#include "uit/setup/ImplSpec.hpp"

#include "State.hpp"

using Spec = uit::ImplSpec<State>;

using config_t = std::unordered_map<std::string, size_t>;

// TODO refactor to Empirical config system
config_t make_config() {

  const config_t res{
    {
      "audit",
      uitsl::stoszt(
        std::getenv("PP_AUDIT") ?: "0"
      )
    },
    {
      "checkout_memory",
      uitsl::stoszt(
        std::getenv("PP_CHECKOUT_MEMORY") ?: "0"
      )
    },
    {
      "shuffle_tile_evaluation",
      uitsl::stoszt(
        std::getenv("PP_SHUFFLE_TILE_EVALUATION") ?: "1"
      )
    },
    {
      "num_chunks",
      uitsl::stoszt(
        std::getenv("PP_NUM_THREADS") ?: "1"
      )
    },
    {
      "num_threads",
      uitsl::stoszt(
        std::getenv("PP_NUM_THREADS") ?: "1"
      )
    },
    {
      "num_updates", // undefined = no limit
      uitsl::stoszt(
        std::getenv("PP_NUM_UPDATES")
        ?: emp::to_string(std::numeric_limits<size_t>::max())
      )
    },
    {
      "num_seconds", // 0 for no limit
      uitsl::stoszt(
        std::getenv("PP_NUM_SECONDS") ?: "5"
      )
    },
    {
      "grid_size",
      uitsl::stoszt(
        std::getenv("PP_GRID_SIZE") ?: "1000"
      )
    },
    {
      "synchronous",
      uitsl::stoszt(
        std::getenv("PP_SYNCHRONOUS") ?: "1"
      )
    },
    {
      "resistance",
      uitsl::stoszt(
        std::getenv("PP_RESISTANCE") ?: "0"
      )
    },
    {
      "use_omp",
      uitsl::stoszt(
        std::getenv("PP_USE_OMP") ?: "0"
      )
    },
    {
      "verbose", // printing CA grid
      uitsl::stoszt(
        std::getenv("PP_VERBOSE") ?: "0"
      )
    },
    {
      "taciturn", // RE: printing other info
      uitsl::stoszt(
        std::getenv("PP_TACITURN") ?: "0"
      )
    }
  };

  assert(res.at("grid_size") % res.at("num_threads") == 0);

  return res;

}

void print_config(const config_t & cfg) {
  std::cout << ">>> config <<<" << '\n';
  for (const auto & [name, value] : cfg) {
    std::cout << name << ": " << value << '\n';
  }
  std::cout << '\n';
}
