#include "sgpl/library/prefab/prefab.hpp"

#include "_BenchSpec.hpp"

#define OP_LIBRARY Arithmetic

constexpr bool fill_cores = false;

using spec_t = BenchSpec<sgpl::ArithmeticOpLibrary>;

#include "benchmark.hpp"
