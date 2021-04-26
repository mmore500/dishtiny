#include "sgpl/library/prefab/prefab.hpp"

#include "_BenchSpec.hpp"

#define OP_LIBRARY Complete

constexpr bool fill_cores = true;

struct spec_t : public BenchSpec<sgpl::CompleteOpLibrary> {

  static constexpr inline size_t switch_steps{ 1 };

};

#include "benchmark.hpp"
