#include <string>

#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/vector.hpp"

#include "dish2/config/TemporaryConfigOverride.hpp"
#include "dish2/runninglog/RunningLog.hpp"

TEST_CASE("Test RunningLog") {

  const dish2::TemporaryConfigOverride override{ "RUNNING_LOG_DURATION", 2 };

  dish2::RunningLog< std::string > log{};

  log.Record( "duck" );
  log.Record( "waddle" );

  REQUIRE(
    emp::vector<std::string>( std::begin(log), std::end(log) )
    == (emp::vector<std::string>{"waddle", "duck"})
  );

  log.Purge();

  REQUIRE(
    emp::vector<std::string>( std::begin(log), std::end(log) )
    == (emp::vector<std::string>{"waddle", "duck"})
  );

  log.Record( "quack" );

  REQUIRE(
    emp::vector<std::string>( std::begin(log), std::end(log) )
    == (emp::vector<std::string>{"quack", "waddle", "duck"})
  );

  log.Purge();

  REQUIRE(
    emp::vector<std::string>( std::begin(log), std::end(log) )
    == (emp::vector<std::string>{"quack", "waddle", "duck"})
  );

  log.Purge();

  REQUIRE(
    emp::vector<std::string>( std::begin(log), std::end(log) )
    == (emp::vector<std::string>{"quack"})
  );

  log.Record( "honk" );

  REQUIRE(
    emp::vector<std::string>( std::begin(log), std::end(log) )
    == (emp::vector<std::string>{"honk", "quack"})
  );

  log.Purge();

  REQUIRE(
    emp::vector<std::string>( std::begin(log), std::end(log) )
    == (emp::vector<std::string>{"honk"})
  );

  log.Purge();

  REQUIRE(
    emp::vector<std::string>( std::begin(log), std::end(log) )
    == (emp::vector<std::string>{"honk"})
  );

  log.Purge();

  REQUIRE(
    emp::vector<std::string>( std::begin(log), std::end(log) )
    == (emp::vector<std::string>{})
  );

  log.Purge();

  REQUIRE(
    emp::vector<std::string>( std::begin(log), std::end(log) )
    == (emp::vector<std::string>{})
  );


}
