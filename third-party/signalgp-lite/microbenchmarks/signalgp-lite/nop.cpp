#include "sgpl/library/prefab/prefab.hpp"

#include "_BenchSpec.hpp"

#define OP_LIBRARY Nop

constexpr bool fill_cores = false;

using spec_t = BenchSpec<sgpl::NopOpLibrary>;

#include "benchmark.hpp"
