#include "sgpl/library/prefab/prefab.hpp"

#include "_BenchSpec.hpp"

#define OP_LIBRARY SansRegulation

constexpr bool fill_cores = true;

struct spec_t : public BenchSpec<sgpl::SansRegulationOpLibrary> {

  static constexpr inline size_t switch_steps{ 1 };

};

#include "benchmark.hpp"
