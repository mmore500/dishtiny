#pragma once
#ifndef UITSL_DEBUG_BENCHMARK_UTILS_HPP_INCLUDE
#define UITSL_DEBUG_BENCHMARK_UTILS_HPP_INCLUDE

#include <random>

#include <benchmark/benchmark.h>

#include "../math/stats_utils.hpp"

namespace uitsl {

// adapted from Google Benchmark
// https://github.com/google/benchmark/blob/37177a84b7e8d33696ea1e1854513cb0de3b4dc3/include/benchmark/benchmark.h#L307

// GCC string: https://quick-bench.com/q/xjx4ugn7uGsYkYn6kdO9ew6NQOg
// Clang string: https://quick-bench.com/q/4jocMN_o8PmTPdJuPN7tJMHsa3c

// GCC int: https://quick-bench.com/q/Kg4Wxh_b1fFXLwEoq5147890pSM
// Clang int: https://quick-bench.com/q/iD8r--H2CSObahy_y-D_j3JXwGU

template <typename T>
inline void do_not_optimize(const T& value) {
  asm volatile("" : : "r,m"(value) : "memory");
}

template <typename T>
inline void do_not_optimize(T& value) {
#if defined(__clang__)
  asm volatile("" : "+r,m"(value) : : "memory");
#else
  asm volatile("" : "+m,r"(value) : : "memory");
#endif
}

inline void do_compute_work(const size_t amt=1) {

  thread_local std::uniform_int_distribution<size_t> distribution{
    0,
    std::numeric_limits<size_t>::max()
  };
  thread_local std::mt19937 random_number_engine;
  thread_local auto rand = std::bind(
    distribution,
    random_number_engine
  );

  for (size_t i = 0; i < amt; ++i) {
    const auto res = rand();
    uitsl::do_not_optimize( res );
  }

}

template<size_t NumReps=10>
benchmark::internal::Benchmark* report_confidence(
  benchmark::internal::Benchmark* bench
) {
  return bench->Repetitions(
    NumReps
  )->ComputeStatistics(
    "ci5",
    [](const std::vector<double>& v) -> double {
      emp::Random rand;
      return std::get<0>(
        bootstrap(rand, v)
      );
    }
  )->ComputeStatistics(
    "ci95",
    [](const std::vector<double>& v) -> double {
      emp::Random rand;
      return std::get<1>(
        bootstrap(rand, v)
      );
    }
  );
}

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_BENCHMARK_UTILS_HPP_INCLUDE
