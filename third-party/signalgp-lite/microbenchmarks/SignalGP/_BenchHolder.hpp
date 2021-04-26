#pragma once

#include "_SignalGP.hpp"

template<size_t NUM_AGENTS>
struct BenchHolder {

  std::function<program_t()> make_program;
  inst_lib_t& inst_lib_;
  event_lib_t& event_lib_;

  void Run(benchmark::State& state) {

    emp::Random rand{ 1 };
    emp::vector<signalgp_t> collection;
    collection.reserve( NUM_AGENTS );

    for (size_t i = 0; i < NUM_AGENTS; ++i) {
      collection.emplace_back(rand, inst_lib_, event_lib_);
      collection.back().SetActiveThreadLimit(16);
      collection.back().SetProgram(make_program());
      collection.back().SpawnThreadWithID(0);
      assert( collection.back().GetActiveThreadIDs().size() == 1 );
    }

    size_t agent{};

    emp::vector< emp::BitSet<64> > tags;
    tags.reserve( 20 );
    std::generate_n(
      std::back_inserter(tags),
      20,
      [&]{ return emp::BitSet<64>(rand); }
    );
    size_t tag{};

    // Perform setup here
    for (auto _ : state) {
      // This code gets timed

      auto& cpu = collection[agent];

      if constexpr ( fill_cores ) {

        while ( cpu.GetUnusedThreadIDs().size() ) {
          cpu.SpawnThreadWithTag( tags[tag] );
          ++tag %= tags.size();
        }
        cpu.SingleProcess();
      } else {
        for (size_t i{}; i < 16; ++i) cpu.SingleProcess();
      }

      ++agent %= NUM_AGENTS;

    }

    for (auto& hardware : collection) {
      assert( hardware.GetActiveThreadIDs().size() == 1 );
    }

    state.counters["num agents"] = NUM_AGENTS;

  }

};
