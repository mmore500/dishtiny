#pragma once
#ifndef UITSL_DEBUG_MULTIPROCESSREPORTER_HPP_INCLUDE
#define UITSL_DEBUG_MULTIPROCESSREPORTER_HPP_INCLUDE

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "../mpi/mpi_utils.hpp"

namespace Catch {
// to fix maybe do look into some of these https://stackoverflow.com/questions/58289895/is-it-possible-to-use-catch2-for-testing-an-mpi-code
class MultiprocessReporter : public ConsoleReporter {

  Catch::ConsoleReporter impl;

public:

  explicit MultiprocessReporter(const Catch::ReporterConfig& config)
  : Catch::ConsoleReporter(config), impl(config) { ; }

  void testRunEnded(Catch::TestRunStats const& testRunStats) override {

    if(uitsl::is_root() || !testRunStats.totals.testCases.allPassed()) {
      const std::string message{ emp::to_string(
        "\x1B[35m",
        "Processes: ",
        uitsl::get_nprocs(),
        "\033[0m\n"
      ) };
      printf("%s", message.c_str());
      impl.testRunEnded(testRunStats);
    }

  }

};

CATCH_REGISTER_REPORTER ("multiprocess", MultiprocessReporter);

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_MULTIPROCESSREPORTER_HPP_INCLUDE
