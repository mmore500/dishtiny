#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/parallel/ThreadIbarrierFactory.hpp"
#include "uitsl/parallel/ThreadIbarrier.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"

TEST_CASE("ThreadIbarrier satisfied serial") {

  uitsl::ThreadIbarrierFactory factory{1};

  uitsl::ThreadIbarrier first{ factory.MakeBarrier() };
  uitsl::ThreadIbarrier second{ factory.MakeBarrier() };
  REQUIRE(first.IsComplete());
  REQUIRE(second.IsComplete());

}

TEST_CASE("ThreadIbarrier unsatisfied") {

  uitsl::ThreadIbarrierFactory factory{2};
  uitsl::ThreadIbarrier first{ factory.MakeBarrier() };
  uitsl::ThreadIbarrier second{ factory.MakeBarrier() };
  REQUIRE(!first.IsComplete());
  REQUIRE(!second.IsComplete());

}

TEST_CASE("ThreadIbarrier satisfied parallel") {

  for (size_t thread_count = 1; thread_count <= 8; ++thread_count) {
    std::cout << "thread_count: " << thread_count << '\n';
    uitsl::ThreadIbarrierFactory factory{thread_count};
    uitsl::ThreadTeam team;
    for (size_t thread = 0; thread < thread_count; ++thread) {
      team.Add([&factory](){
        for (size_t rep = 0; rep < 100; ++rep) {
          uitsl::ThreadIbarrier first{ factory.MakeBarrier() };
          uitsl::ThreadIbarrier second{ factory.MakeBarrier() };
          while(!first.IsComplete());
          REQUIRE( first.IsComplete() );
          while(!second.IsComplete());
          REQUIRE( second.IsComplete() );
        }
      });
    }
    team.Join();
  }
}
