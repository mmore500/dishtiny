#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"

#include <limits>

#include "emp/bits/BitSet.hpp"
#include "emp/math/Range.hpp"

#include "EventLibrary.h"

#include "SignalGPBase.h"
#include "impls/SignalGPToy.h"
#include "impls/SignalGPLinearProgram.h"
#include "impls/SignalGPLinearFunctionsProgram.h"
#include "utils/InstructionLibrary.h"
#include "utils/linear_program_instructions_impls.h"
#include "utils/linear_functions_program_instructions_impls.h"
#include "utils/MemoryModel.h"
#include "utils/LinearFunctionsProgram.h"
#include "../source/random_utils.h"

TEST_CASE( "Hello World", "[general]" ) {
  std::cout << "Hello tests!" << std::endl;
}

TEST_CASE("RandomBitSet<W>") {
  constexpr size_t RANDOM_SEED = 1;
  emp::Random random(RANDOM_SEED);
  std::unordered_set<uint32_t> uset; // Will be used to double-check uniqueness.

  // Generate a bunch of big random tags. No uniqueness guarantees.
  for (size_t i = 0; i < 100; ++i) auto tag = emp::RandomBitSet<1024>(random);

  // Enumerate all 2-bit tags
  emp::vector<emp::BitSet<2>> tags2;
  uset.clear();
  // std::cout << "All two-bit tags: " << std::endl;
  for (size_t i = 0; i < emp::Pow2(2); ++i) {
    tags2.emplace_back(emp::RandomBitSet<2>(random, tags2));
    uset.emplace(tags2.back().GetUInt(0));
    // std::cout << "  ";
    // tags2.back().Print();
    // std::cout << " : " << tags2.back().GetUInt(0) << std::endl;
  }
  REQUIRE(tags2.size() == emp::Pow2(2));
  REQUIRE(uset.size() == emp::Pow2(2));
  for (size_t i = 0; i < emp::Pow2(2); ++i) REQUIRE(emp::Has(uset, i));

  // Enumerate all 4-bit tags
  emp::vector<emp::BitSet<4>> tags4;
  uset.clear();
  // std::cout << "All four-bit tags: " << std::endl;
  for (size_t i = 0; i < emp::Pow2(4); ++i) {
    tags4.emplace_back(emp::RandomBitSet<4>(random, tags4));
    uset.emplace(tags4.back().GetUInt(0));
    // std::cout << "  ";
    // tags4.back().Print();
    // std::cout << " : " << tags4.back().GetUInt(0) << std::endl;
  }
  REQUIRE(tags4.size() == emp::Pow2(4));
  REQUIRE(uset.size() == emp::Pow2(4));
  for (size_t i = 0; i < emp::Pow2(4); ++i) REQUIRE(emp::Has(uset, i));

  // Generate a bunch of 16-bit tags.
  emp::vector<emp::BitSet<16>> tags16;
  for (size_t k = 0; k < 100; ++k) {
    uset.clear();
    tags16.clear();
    for (size_t i = 0; i < 1000; ++i) {
      tags16.emplace_back(emp::RandomBitSet<16>(random, tags16));
      uset.emplace(tags16.back().GetUInt(0));
    }
    REQUIRE(uset.size() == 1000);
  }
}

TEST_CASE("RandomBitSets<W>") {
  constexpr size_t RANDOM_SEED = 1;
  emp::Random random(RANDOM_SEED);

  std::unordered_set<uint32_t> uset; // Will be used to double-check uniqueness.

  // Generate lots of small tags with no guarantees on uniqueness.
  auto small_tags = emp::RandomBitSets<2>(random, 1000);
  REQUIRE(small_tags.size() == 1000);
  // Generate lots of large tags with no guarantees on uniqueness.
  auto big_tags = emp::RandomBitSets<1024>(random, 1000);
  REQUIRE(big_tags.size() == 1000);

  // Use generator to enumerate all 2-bit tags.
  auto tags2 = emp::RandomBitSets<2>(random, emp::Pow2(2), true);
  uset.clear();
  for (size_t i = 0; i < tags2.size(); ++i) {
    uset.emplace(tags2[i].GetUInt(0));
  }
  REQUIRE(tags2.size() == emp::Pow2(2));
  REQUIRE(uset.size() == emp::Pow2(2));
  for (size_t i = 0; i < emp::Pow2(2); ++i) REQUIRE(emp::Has(uset, i));

  // Use generator to enumerate all 4-bit tags.
  auto tags4 = emp::RandomBitSets<4>(random, emp::Pow2(4), true);
  uset.clear();
  for (size_t i = 0; i < tags4.size(); ++i) {
    uset.emplace(tags4[i].GetUInt(0));
  }
  REQUIRE(tags4.size() == emp::Pow2(4));
  REQUIRE(uset.size() == emp::Pow2(4));
  for (size_t i = 0; i < emp::Pow2(4); ++i) REQUIRE(emp::Has(uset, i));

  // Generate a bunch of 8-bit tags (50% of the tag-space).
  // Check for uniqueness.
  auto tags8 = emp::RandomBitSets<8>(random, 128, true);
  uset.clear();
  for (size_t i = 0; i < tags8.size(); ++i) uset.emplace(tags8[i].GetUInt(0));
  REQUIRE(tags8.size() == 128);
  REQUIRE(uset.size() == 128);

  // Generate a bunch of 8-bit tags using previously generated tag as
  // reserved tag-space. Check for proper uniqueness.
  for (size_t i = 0; i < 100; ++i) {
    std::unordered_set<uint32_t> temp_set(uset);
    auto tags = emp::RandomBitSets<8>(random, 64, true, tags8);
    REQUIRE(tags.size() == 64);
    for (size_t t = 0; t < tags.size(); ++t) temp_set.emplace(tags[t].GetUInt(0));
    REQUIRE(temp_set.size() == 128+64);
  }
}

TEST_CASE("LinearProgram<emp::BitSet<W>,int> - GenRandInst") {
  constexpr size_t TAG_WIDTH = 16;
  using mem_model_t = sgp::SimpleMemoryModel;
  using tag_t = emp::BitSet<TAG_WIDTH>;
  using arg_t = int;
  using matchbin_t = emp::MatchBin< size_t,
                                    emp::HammingMetric<TAG_WIDTH>,
                                    emp::RankedSelector<>,
                                    emp::AdditiveCountdownRegulator<>
                                  >;
  using hardware_t = sgp::LinearFunctionsProgramSignalGP<mem_model_t,
                                                                   tag_t,
                                                                   arg_t,
                                                                   matchbin_t>;
  using inst_lib_t = typename hardware_t::inst_lib_t;
  using inst_t = typename hardware_t::inst_t;
  using inst_prop_t = typename hardware_t::InstProperty;

  constexpr int RANDOM_SEED = 1;
  constexpr size_t NUM_TAGS = 10;
  constexpr size_t NUM_ARGS = 3;
  constexpr size_t MIN_ARG_VAL = 0;
  constexpr size_t MAX_ARG_VAL = 15;
  emp::Random random(RANDOM_SEED);

  // Build a limited instruction library.
  inst_lib_t inst_lib;
  inst_lib.AddInst("Nop", [](hardware_t & hw, const inst_t & inst) { ; }, "No operation!");
  inst_lib.AddInst("Inc", sgp::inst_impl::Inst_Inc<hardware_t, inst_t>, "Increment!");
  inst_lib.AddInst("Dec", sgp::inst_impl::Inst_Dec<hardware_t, inst_t>, "Decrement!");
  inst_lib.AddInst("Not", sgp::inst_impl::Inst_Not<hardware_t, inst_t>, "Logical not of ARG[0]");
  inst_lib.AddInst("Add", sgp::inst_impl::Inst_Add<hardware_t, inst_t>, "");
  inst_lib.AddInst("Sub", sgp::inst_impl::Inst_Sub<hardware_t, inst_t>, "");
  inst_lib.AddInst("Mult", sgp::inst_impl::Inst_Mult<hardware_t, inst_t>, "");
  inst_lib.AddInst("Div", sgp::inst_impl::Inst_Div<hardware_t, inst_t>, "");
  inst_lib.AddInst("Mod", sgp::inst_impl::Inst_Mod<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestEqu", sgp::inst_impl::Inst_TestEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestNEqu", sgp::inst_impl::Inst_TestNEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestLess", sgp::inst_impl::Inst_TestLess<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestLessEqu", sgp::inst_impl::Inst_TestLessEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestGreater", sgp::inst_impl::Inst_TestGreater<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestGreaterEqu", sgp::inst_impl::Inst_TestGreaterEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("SetMem", sgp::inst_impl::Inst_SetMem<hardware_t, inst_t>, "");
  inst_lib.AddInst("Close", sgp::inst_impl::Inst_Close<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_CLOSE});
  inst_lib.AddInst("Break", sgp::inst_impl::Inst_Break<hardware_t, inst_t>, "");
  inst_lib.AddInst("Call", sgp::inst_impl::Inst_Call<hardware_t, inst_t>, "");
  inst_lib.AddInst("Return", sgp::inst_impl::Inst_Return<hardware_t, inst_t>, "");
  inst_lib.AddInst("CopyMem", sgp::inst_impl::Inst_CopyMem<hardware_t, inst_t>, "");
  inst_lib.AddInst("SwapMem", sgp::inst_impl::Inst_SwapMem<hardware_t, inst_t>, "");
  inst_lib.AddInst("InputToWorking", sgp::inst_impl::Inst_InputToWorking<hardware_t, inst_t>, "");
  inst_lib.AddInst("WorkingToOutput", sgp::inst_impl::Inst_WorkingToOutput<hardware_t, inst_t>, "");
  inst_lib.AddInst("WorkingToGlobal", sgp::inst_impl::Inst_WorkingToGlobal<hardware_t, inst_t>, "");
  inst_lib.AddInst("GlobalToWorking", sgp::inst_impl::Inst_GlobalToWorking<hardware_t, inst_t>, "");
  inst_lib.AddInst("Fork", sgp::inst_impl::Inst_Fork<hardware_t, inst_t>, "");
  inst_lib.AddInst("Terminate", sgp::inst_impl::Inst_Terminate<hardware_t, inst_t>, "");
  inst_lib.AddInst("If", sgp::lfp_inst_impl::Inst_If<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("While", sgp::lfp_inst_impl::Inst_While<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("Countdown", sgp::lfp_inst_impl::Inst_Countdown<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("Routine", sgp::lfp_inst_impl::Inst_Routine<hardware_t, inst_t>, "");

  // Generate a bunch of random instructions, check that they conform with requested bounds.
  for (size_t i = 0; i < 10000; ++i) {
    inst_t inst(sgp::GenRandInst<hardware_t, TAG_WIDTH>(random, inst_lib, NUM_TAGS, NUM_ARGS, {MIN_ARG_VAL, MAX_ARG_VAL}));
    REQUIRE(inst.id < inst_lib.GetSize());
    REQUIRE(inst.GetTags().size() == NUM_TAGS);
    REQUIRE(inst.GetArgs().size() == NUM_ARGS);
    for (auto & arg : inst.GetArgs()) {
      REQUIRE(arg >= MIN_ARG_VAL);
      REQUIRE(arg <= MAX_ARG_VAL);
    }
  }
}

TEST_CASE("LinearProgram<emp::BitSet<W>,int> - GenRandLinearProgram") {
  constexpr size_t TAG_WIDTH = 16;
  using mem_model_t = sgp::SimpleMemoryModel;
  using tag_t = emp::BitSet<TAG_WIDTH>;
  using arg_t = int;
  using matchbin_t = emp::MatchBin< size_t, emp::HammingMetric<TAG_WIDTH>, emp::RankedSelector<>, emp::AdditiveCountdownRegulator<>>;
  using hardware_t = sgp::LinearProgramSignalGP<mem_model_t,
                                                          tag_t,
                                                          arg_t,
                                                          matchbin_t>;
  using inst_lib_t = typename hardware_t::inst_lib_t;
  using inst_t = typename hardware_t::inst_t;
  using inst_prop_t = typename hardware_t::InstProperty;
  using program_t = typename hardware_t::program_t;

  constexpr int RANDOM_SEED = 1;
  constexpr size_t NUM_TAGS = 10;
  constexpr size_t NUM_ARGS = 3;
  constexpr size_t MIN_ARG_VAL = 0;
  constexpr size_t MAX_ARG_VAL = 15;
  constexpr size_t MIN_INST_CNT = 1;
  constexpr size_t MAX_INST_CNT = 512;

  emp::Random random(RANDOM_SEED);
  // Build a limited instruction library.
  inst_lib_t inst_lib;
  inst_lib.AddInst("Nop", [](hardware_t & hw, const inst_t & inst) { ; }, "No operation!");
  inst_lib.AddInst("ModuleDef", [](hardware_t & hw, const inst_t & inst) { ; }, "Module definition", {inst_prop_t::MODULE});
  inst_lib.AddInst("Inc", sgp::inst_impl::Inst_Inc<hardware_t, inst_t>, "Increment!");
  inst_lib.AddInst("Dec", sgp::inst_impl::Inst_Dec<hardware_t, inst_t>, "Decrement!");
  inst_lib.AddInst("Not", sgp::inst_impl::Inst_Not<hardware_t, inst_t>, "Logical not of ARG[0]");
  inst_lib.AddInst("Add", sgp::inst_impl::Inst_Add<hardware_t, inst_t>, "");
  inst_lib.AddInst("Sub", sgp::inst_impl::Inst_Sub<hardware_t, inst_t>, "");
  inst_lib.AddInst("Mult", sgp::inst_impl::Inst_Mult<hardware_t, inst_t>, "");
  inst_lib.AddInst("Div", sgp::inst_impl::Inst_Div<hardware_t, inst_t>, "");
  inst_lib.AddInst("Mod", sgp::inst_impl::Inst_Mod<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestEqu", sgp::inst_impl::Inst_TestEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestNEqu", sgp::inst_impl::Inst_TestNEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestLess", sgp::inst_impl::Inst_TestLess<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestLessEqu", sgp::inst_impl::Inst_TestLessEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestGreater", sgp::inst_impl::Inst_TestGreater<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestGreaterEqu", sgp::inst_impl::Inst_TestGreaterEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("SetMem", sgp::inst_impl::Inst_SetMem<hardware_t, inst_t>, "");
  inst_lib.AddInst("Close", sgp::inst_impl::Inst_Close<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_CLOSE});
  inst_lib.AddInst("Break", sgp::inst_impl::Inst_Break<hardware_t, inst_t>, "");
  inst_lib.AddInst("Call", sgp::inst_impl::Inst_Call<hardware_t, inst_t>, "");
  inst_lib.AddInst("Return", sgp::inst_impl::Inst_Return<hardware_t, inst_t>, "");
  inst_lib.AddInst("CopyMem", sgp::inst_impl::Inst_CopyMem<hardware_t, inst_t>, "");
  inst_lib.AddInst("SwapMem", sgp::inst_impl::Inst_SwapMem<hardware_t, inst_t>, "");
  inst_lib.AddInst("InputToWorking", sgp::inst_impl::Inst_InputToWorking<hardware_t, inst_t>, "");
  inst_lib.AddInst("WorkingToOutput", sgp::inst_impl::Inst_WorkingToOutput<hardware_t, inst_t>, "");
  inst_lib.AddInst("WorkingToGlobal", sgp::inst_impl::Inst_WorkingToGlobal<hardware_t, inst_t>, "");
  inst_lib.AddInst("GlobalToWorking", sgp::inst_impl::Inst_GlobalToWorking<hardware_t, inst_t>, "");
  inst_lib.AddInst("Fork", sgp::inst_impl::Inst_Fork<hardware_t, inst_t>, "");
  inst_lib.AddInst("Terminate", sgp::inst_impl::Inst_Terminate<hardware_t, inst_t>, "");
  inst_lib.AddInst("If", sgp::inst_impl::Inst_If<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("While", sgp::inst_impl::Inst_While<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("Countdown", sgp::inst_impl::Inst_Countdown<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("Routine", sgp::inst_impl::Inst_Routine<hardware_t, inst_t>, "");

  // Generate a bunch of random instructions, check that they conform with requested bounds.
  for (size_t i = 0; i < 10000; ++i) {
    program_t program(sgp::GenRandLinearProgram<hardware_t, TAG_WIDTH>(random, inst_lib, {MIN_INST_CNT, MAX_INST_CNT}, NUM_TAGS, NUM_ARGS, {MIN_ARG_VAL, MAX_ARG_VAL}));
    REQUIRE(program.GetSize() >= MIN_INST_CNT);
    REQUIRE(program.GetSize() <= MAX_INST_CNT);
    for (size_t pID = 0; pID < program.GetSize(); ++pID) {
      auto & inst = program[pID];
      REQUIRE(inst.id < inst_lib.GetSize());
      REQUIRE(inst.GetTags().size() == NUM_TAGS);
      REQUIRE(inst.GetArgs().size() == NUM_ARGS);
      for (auto & arg : inst.GetArgs()) {
        REQUIRE(arg >= MIN_ARG_VAL);
        REQUIRE(arg <= MAX_ARG_VAL);
      }
    }
  }
}

TEST_CASE("LinearFunction<emp::BitSet<W>, int> - GenRandLinearFunction") {
  constexpr size_t TAG_WIDTH = 16;
  using mem_model_t = sgp::SimpleMemoryModel;
  using tag_t = emp::BitSet<TAG_WIDTH>;
  using arg_t = int;
  using matchbin_t = emp::MatchBin< size_t, emp::HammingMetric<TAG_WIDTH>, emp::RankedSelector<>, emp::AdditiveCountdownRegulator<> >;
  using hardware_t = sgp::LinearFunctionsProgramSignalGP<mem_model_t,
                                                                   tag_t,
                                                                   arg_t,
                                                                   matchbin_t>;
  using inst_lib_t = typename hardware_t::inst_lib_t;
  using inst_t = typename hardware_t::inst_t;
  using inst_prop_t = typename hardware_t::InstProperty;
  using program_t = typename hardware_t::program_t;
  using function_t = typename program_t::function_t;

  constexpr int RANDOM_SEED = 1;
  constexpr size_t NUM_FUNC_TAGS = 3;
  // constexpr size_t MIN_NUM_FUNC = 1;
  // constexpr size_t MAX_NUM_FUNC = 32;
  constexpr size_t NUM_INST_TAGS = 10;
  constexpr size_t NUM_INST_ARGS = 3;
  constexpr size_t MIN_ARG_VAL = 0;
  constexpr size_t MAX_ARG_VAL = 15;
  constexpr size_t MIN_INST_CNT = 1;
  constexpr size_t MAX_INST_CNT = 512;

  emp::Random random(RANDOM_SEED);
  // Build a limited instruction library.
  inst_lib_t inst_lib;
  inst_lib.AddInst("Nop", [](hardware_t & hw, const inst_t & inst) { ; }, "No operation!");
  inst_lib.AddInst("Inc", sgp::inst_impl::Inst_Inc<hardware_t, inst_t>, "Increment!");
  inst_lib.AddInst("Dec", sgp::inst_impl::Inst_Dec<hardware_t, inst_t>, "Decrement!");
  inst_lib.AddInst("Not", sgp::inst_impl::Inst_Not<hardware_t, inst_t>, "Logical not of ARG[0]");
  inst_lib.AddInst("Add", sgp::inst_impl::Inst_Add<hardware_t, inst_t>, "");
  inst_lib.AddInst("Sub", sgp::inst_impl::Inst_Sub<hardware_t, inst_t>, "");
  inst_lib.AddInst("Mult", sgp::inst_impl::Inst_Mult<hardware_t, inst_t>, "");
  inst_lib.AddInst("Div", sgp::inst_impl::Inst_Div<hardware_t, inst_t>, "");
  inst_lib.AddInst("Mod", sgp::inst_impl::Inst_Mod<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestEqu", sgp::inst_impl::Inst_TestEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestNEqu", sgp::inst_impl::Inst_TestNEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestLess", sgp::inst_impl::Inst_TestLess<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestLessEqu", sgp::inst_impl::Inst_TestLessEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestGreater", sgp::inst_impl::Inst_TestGreater<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestGreaterEqu", sgp::inst_impl::Inst_TestGreaterEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("SetMem", sgp::inst_impl::Inst_SetMem<hardware_t, inst_t>, "");
  inst_lib.AddInst("Close", sgp::inst_impl::Inst_Close<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_CLOSE});
  inst_lib.AddInst("Break", sgp::inst_impl::Inst_Break<hardware_t, inst_t>, "");
  inst_lib.AddInst("Call", sgp::inst_impl::Inst_Call<hardware_t, inst_t>, "");
  inst_lib.AddInst("Return", sgp::inst_impl::Inst_Return<hardware_t, inst_t>, "");
  inst_lib.AddInst("CopyMem", sgp::inst_impl::Inst_CopyMem<hardware_t, inst_t>, "");
  inst_lib.AddInst("SwapMem", sgp::inst_impl::Inst_SwapMem<hardware_t, inst_t>, "");
  inst_lib.AddInst("InputToWorking", sgp::inst_impl::Inst_InputToWorking<hardware_t, inst_t>, "");
  inst_lib.AddInst("WorkingToOutput", sgp::inst_impl::Inst_WorkingToOutput<hardware_t, inst_t>, "");
  inst_lib.AddInst("WorkingToGlobal", sgp::inst_impl::Inst_WorkingToGlobal<hardware_t, inst_t>, "");
  inst_lib.AddInst("GlobalToWorking", sgp::inst_impl::Inst_GlobalToWorking<hardware_t, inst_t>, "");
  inst_lib.AddInst("Fork", sgp::inst_impl::Inst_Fork<hardware_t, inst_t>, "");
  inst_lib.AddInst("Terminate", sgp::inst_impl::Inst_Terminate<hardware_t, inst_t>, "");
  inst_lib.AddInst("If", sgp::lfp_inst_impl::Inst_If<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("While", sgp::lfp_inst_impl::Inst_While<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("Countdown", sgp::lfp_inst_impl::Inst_Countdown<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("Routine", sgp::lfp_inst_impl::Inst_Routine<hardware_t, inst_t>, "");

  // Generate a bunch of random instructions, check that they conform with requested bounds.
  for (size_t i = 0; i < 1000; ++i) {
    function_t function(sgp::GenRandLinearFunction<hardware_t, TAG_WIDTH>(random, inst_lib, NUM_FUNC_TAGS, {MIN_INST_CNT, MAX_INST_CNT}, NUM_INST_TAGS, NUM_INST_ARGS, {MIN_ARG_VAL, MAX_ARG_VAL}));
    REQUIRE(function.GetSize() >= MIN_INST_CNT);
    REQUIRE(function.GetSize() <= MAX_INST_CNT);
    REQUIRE(function.GetTags().size() == NUM_FUNC_TAGS);
    for (size_t pID = 0; pID < function.GetSize(); ++pID) {
      auto & inst = function[pID];
      REQUIRE(inst.id < inst_lib.GetSize());
      REQUIRE(inst.GetTags().size() == NUM_INST_TAGS);
      REQUIRE(inst.GetArgs().size() == NUM_INST_ARGS);
      for (auto & arg : inst.GetArgs()) {
        REQUIRE(arg >= MIN_ARG_VAL);
        REQUIRE(arg <= MAX_ARG_VAL);
      }
    }
  }
}

TEST_CASE("LinearFunctionsProgram<emp::BitSet<W>, int> - GenRandLinearFunctionsProgram") {
  constexpr size_t TAG_WIDTH = 16;
  using mem_model_t = sgp::SimpleMemoryModel;
  using tag_t = emp::BitSet<TAG_WIDTH>;
  using arg_t = int;
  using matchbin_t = emp::MatchBin< size_t, emp::HammingMetric<TAG_WIDTH>, emp::RankedSelector<>, emp::AdditiveCountdownRegulator<> >;
  using hardware_t = sgp::LinearFunctionsProgramSignalGP<mem_model_t,
                                                                   tag_t,
                                                                   arg_t,
                                                                   matchbin_t>;
  using inst_lib_t = typename hardware_t::inst_lib_t;
  using inst_t = typename hardware_t::inst_t;
  using inst_prop_t = typename hardware_t::InstProperty;
  using program_t = typename hardware_t::program_t;
  using function_t = typename program_t::function_t;

  constexpr int RANDOM_SEED = 1;
  constexpr size_t NUM_FUNC_TAGS = 3;
  constexpr size_t MIN_NUM_FUNC = 1;
  constexpr size_t MAX_NUM_FUNC = 32;
  constexpr size_t NUM_INST_TAGS = 10;
  constexpr size_t NUM_INST_ARGS = 3;
  constexpr size_t MIN_ARG_VAL = 0;
  constexpr size_t MAX_ARG_VAL = 15;
  constexpr size_t MIN_INST_CNT = 1;
  constexpr size_t MAX_INST_CNT = 512;

  emp::Random random(RANDOM_SEED);
  // Build a limited instruction library.
  inst_lib_t inst_lib;
  inst_lib.AddInst("Nop", [](hardware_t & hw, const inst_t & inst) { ; }, "No operation!");
  inst_lib.AddInst("Inc", sgp::inst_impl::Inst_Inc<hardware_t, inst_t>, "Increment!");
  inst_lib.AddInst("Dec", sgp::inst_impl::Inst_Dec<hardware_t, inst_t>, "Decrement!");
  inst_lib.AddInst("Not", sgp::inst_impl::Inst_Not<hardware_t, inst_t>, "Logical not of ARG[0]");
  inst_lib.AddInst("Add", sgp::inst_impl::Inst_Add<hardware_t, inst_t>, "");
  inst_lib.AddInst("Sub", sgp::inst_impl::Inst_Sub<hardware_t, inst_t>, "");
  inst_lib.AddInst("Mult", sgp::inst_impl::Inst_Mult<hardware_t, inst_t>, "");
  inst_lib.AddInst("Div", sgp::inst_impl::Inst_Div<hardware_t, inst_t>, "");
  inst_lib.AddInst("Mod", sgp::inst_impl::Inst_Mod<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestEqu", sgp::inst_impl::Inst_TestEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestNEqu", sgp::inst_impl::Inst_TestNEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestLess", sgp::inst_impl::Inst_TestLess<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestLessEqu", sgp::inst_impl::Inst_TestLessEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestGreater", sgp::inst_impl::Inst_TestGreater<hardware_t, inst_t>, "");
  inst_lib.AddInst("TestGreaterEqu", sgp::inst_impl::Inst_TestGreaterEqu<hardware_t, inst_t>, "");
  inst_lib.AddInst("SetMem", sgp::inst_impl::Inst_SetMem<hardware_t, inst_t>, "");
  inst_lib.AddInst("Close", sgp::inst_impl::Inst_Close<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_CLOSE});
  inst_lib.AddInst("Break", sgp::inst_impl::Inst_Break<hardware_t, inst_t>, "");
  inst_lib.AddInst("Call", sgp::inst_impl::Inst_Call<hardware_t, inst_t>, "");
  inst_lib.AddInst("Return", sgp::inst_impl::Inst_Return<hardware_t, inst_t>, "");
  inst_lib.AddInst("CopyMem", sgp::inst_impl::Inst_CopyMem<hardware_t, inst_t>, "");
  inst_lib.AddInst("SwapMem", sgp::inst_impl::Inst_SwapMem<hardware_t, inst_t>, "");
  inst_lib.AddInst("InputToWorking", sgp::inst_impl::Inst_InputToWorking<hardware_t, inst_t>, "");
  inst_lib.AddInst("WorkingToOutput", sgp::inst_impl::Inst_WorkingToOutput<hardware_t, inst_t>, "");
  inst_lib.AddInst("WorkingToGlobal", sgp::inst_impl::Inst_WorkingToGlobal<hardware_t, inst_t>, "");
  inst_lib.AddInst("GlobalToWorking", sgp::inst_impl::Inst_GlobalToWorking<hardware_t, inst_t>, "");
  inst_lib.AddInst("Fork", sgp::inst_impl::Inst_Fork<hardware_t, inst_t>, "");
  inst_lib.AddInst("Terminate", sgp::inst_impl::Inst_Terminate<hardware_t, inst_t>, "");
  inst_lib.AddInst("If", sgp::lfp_inst_impl::Inst_If<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("While", sgp::lfp_inst_impl::Inst_While<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("Countdown", sgp::lfp_inst_impl::Inst_Countdown<hardware_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("Routine", sgp::lfp_inst_impl::Inst_Routine<hardware_t, inst_t>, "");

  // Generate a bunch of random instructions, check that they conform with requested bounds.
  for (size_t i = 0; i < 1000; ++i) {
    // std::cout << "i="<<i<<std::endl;
    program_t program(sgp::GenRandLinearFunctionsProgram<hardware_t, TAG_WIDTH>(random, inst_lib, {MIN_NUM_FUNC, MAX_NUM_FUNC}, NUM_FUNC_TAGS, {MIN_INST_CNT, MAX_INST_CNT}, NUM_INST_TAGS, NUM_INST_ARGS, {MIN_ARG_VAL, MAX_ARG_VAL}));
    REQUIRE(program.GetSize() >= MIN_NUM_FUNC);
    REQUIRE(program.GetSize() <= MAX_NUM_FUNC);
    for (size_t fID = 0; fID < program.GetSize(); ++fID) {
      function_t & function = program[fID];
      REQUIRE(function.GetSize() >= MIN_INST_CNT);
      REQUIRE(function.GetSize() <= MAX_INST_CNT);
      REQUIRE(function.GetTags().size() == NUM_FUNC_TAGS);
      for (size_t pID = 0; pID < function.GetSize(); ++pID) {
        auto & inst = function[pID];
        REQUIRE(inst.id < inst_lib.GetSize());
        REQUIRE(inst.GetTags().size() == NUM_INST_TAGS);
        REQUIRE(inst.GetArgs().size() == NUM_INST_ARGS);
        for (auto & arg : inst.GetArgs()) {
          REQUIRE(arg >= MIN_ARG_VAL);
          REQUIRE(arg <= MAX_ARG_VAL);
        }
      }
    }
  }
}

TEST_CASE("Toy SignalGP", "[general]") {
  using signalgp_t = ToySignalGP<size_t>;
  using event_lib_t = typename signalgp_t::event_lib_t;

  event_lib_t event_lib;
  emp::Random random(2);
  signalgp_t hardware(event_lib);

  // Configure hardware
  hardware.SetActiveThreadLimit(16);
  // Make a toy program.
  emp::vector<size_t> prog1({0,1,2,3,4,5});
  // Load program onto hardware.
  hardware.SetProgram(prog1);
  hardware.SetPrintHardwareStateFun([](const signalgp_t & hw, std::ostream & os) {
    os << "Thread states: [";
    for (size_t ti = 0; ti < hw.GetThreadExecOrder().size(); ++ti) {
      const size_t thread_id = hw.GetThreadExecOrder()[ti];
      if (!hw.GetThread(thread_id).IsRunning()) continue;
      if (ti) os << ", ";
      os << "{" << thread_id << ": " << hw.GetThread(thread_id).GetExecState().value << "}";
    }
    os << "]\n";
  });

  std::cout << "--- Initial hardware state ---" << std::endl;
  hardware.PrintHardwareState();
  // Spawn a few threads.
  hardware.SpawnThreadWithID(0);
  hardware.SpawnThreads(1, 2);
  hardware.SpawnThreads(6, 1);
  // print hardware state.
  std::cout << "--- after spawning threads ---" << std::endl;
  hardware.PrintHardwareState();
  std::cout << "--- single process ---" << std::endl;
  hardware.SingleProcess();
  hardware.PrintHardwareState();
  std::cout << "--- single process ---" << std::endl;
  hardware.SingleProcess();
  hardware.PrintHardwareState();
  std::cout << "--- single process ---" << std::endl;
  hardware.SingleProcess();
  hardware.PrintHardwareState();
  std::cout << "!!" << std::endl;
}

// Test SignalGP thread management (using Toy virtual hardware)
// todo - test priority
TEST_CASE("Thread Management (Toy SignalGP)") {
  using signalgp_t = ToySignalGP<size_t>;
  using event_lib_t = typename signalgp_t::event_lib_t;

  event_lib_t event_lib;
  emp::Random random(2);
  signalgp_t hardware(event_lib);

  // Configure hardware
  hardware.SetActiveThreadLimit(8);
  REQUIRE(hardware.ValidateThreadState());
  hardware.SetThreadCapacity(16);
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetThreads().size() >= 8);
  REQUIRE(hardware.GetThreads().size() <= 16);

  // Make a toy program.
  emp::vector<size_t> prog1({1, 5, 10, 20, 50, 100});
  // Load program onto hardware.
  hardware.SetProgram(prog1);
  hardware.SetPrintHardwareStateFun([](const signalgp_t & hw, std::ostream & os) {
    os << "Thread states: [";
    for (size_t ti = 0; ti < hw.GetThreadExecOrder().size(); ++ti) {
      const size_t thread_id = hw.GetThreadExecOrder()[ti];
      if (!hw.GetThread(thread_id).IsRunning()) continue;
      if (ti) os << ", ";
      os << "{" << thread_id << ": " << hw.GetThread(thread_id).GetExecState().value << "}";
    }
    os << "]\n";
  });

  //////////////////////////////////////////////////////////////////////////////
  // Test that things start empty
  REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetUnusedThreadIDs().size() >= 8);
  REQUIRE(hardware.GetUnusedThreadIDs().size() <= 16);
  REQUIRE(hardware.GetThreadExecOrder().size() == 0);

  //////////////////////////////////////////////////////////////////////////////
  // Test - spawn thread while active & pending < limit
  std::cout << "Test - Spawn thread while active & pending < limit" << std::endl;
  hardware.SpawnThreadWithID(0); // Thread should be pending.
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
  REQUIRE(hardware.GetThreadExecOrder().size() == 0);
  hardware.SingleProcess(); // Thread will get activated and run 1 step (1=>0).
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 1);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 1);
  hardware.SingleProcess(); // Thread will be marked as dead (and cleaned up).
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 0);

  //////////////////////////////////////////////////////////////////////////////
  // Test - spawn thread while active == limit
  std::cout << "Test - Spawn thread while active == limit" << std::endl;
  hardware.ResetHardware();
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 0);
  for (size_t i = 0; i < 8; ++i) {
    hardware.SpawnThreadWithID(1);
    REQUIRE(hardware.ValidateThreadState());
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    REQUIRE(hardware.GetPendingThreadIDs().size() == i+1);
    REQUIRE(hardware.GetThreadExecOrder().size() == 0);
  }
  hardware.SingleProcess();
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 8);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 8);
  hardware.SpawnThreadWithID(1);
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 8);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
  REQUIRE(hardware.GetThreadExecOrder().size() == 8);
  hardware.SingleProcess();
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 8);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 8);

  //////////////////////////////////////////////////////////////////////////////
  // Test - spawn thread while active & pending == limit
  std::cout << "Test - Spawn thread while active & pending == limit" << std::endl;
  hardware.ResetHardware();
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 0);
  for (size_t i = 0; i < 32; ++i) {
    hardware.SpawnThreadWithID(1);
    size_t expected_pending = (i+1 > 16) ? 16 : i + 1;
    REQUIRE(hardware.ValidateThreadState());
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    REQUIRE(hardware.GetPendingThreadIDs().size() == expected_pending);
    REQUIRE(hardware.GetThreadExecOrder().size() == 0);
  }
  hardware.SingleProcess();
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 8);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 8);
  hardware.SpawnThreadWithID(1);
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 8);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
  REQUIRE(hardware.GetThreadExecOrder().size() == 8);
  hardware.SingleProcess();
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 8);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 8);
  for (size_t i = 0; i < 4; ++i) hardware.SingleProcess();
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 0);

  //////////////////////////////////////////////////////////////////////////////
  // Test - spawn thread while active & pending == limit
  hardware.ResetHardware();
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 0);
  for (size_t i = 0; i < 32; ++i) {
    hardware.SpawnThreadWithID(1, 1);
    size_t expected_pending = (i+1 > 16) ? 16 : i + 1;
    REQUIRE(hardware.ValidateThreadState());
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    REQUIRE(hardware.GetPendingThreadIDs().size() == expected_pending);
    REQUIRE(hardware.GetThreadExecOrder().size() == 0);
  }
  hardware.SingleProcess();
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 8);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 8);

  std::cout << "Test - Spawn different priority threads while active == limit" << std::endl;
  hardware.SpawnThreadWithID(2, 100);
  hardware.SpawnThreadWithID(2, -1);
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 8);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 2);
  REQUIRE(hardware.GetThreadExecOrder().size() == 8);
  hardware.SingleProcess();
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 8);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 8);
  for (size_t i = 0; i < 4; ++i) hardware.SingleProcess();
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 1);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 1);

  //////////////////////////////////////////////////////////////////////////////
  // Test - spawn thread while active & pending == limit
  hardware.ResetHardware();
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 0);
  for (size_t i = 0; i < 32; ++i) {
    hardware.SpawnThreadWithID(1, 1);
    size_t expected_pending = (i+1 > 16) ? 16 : i + 1;
    REQUIRE(hardware.ValidateThreadState());
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    REQUIRE(hardware.GetPendingThreadIDs().size() == expected_pending);
    REQUIRE(hardware.GetThreadExecOrder().size() == 0);
  }
  hardware.SingleProcess();
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 8);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 8);
  // mark all active threads as dead
  for (size_t id : hardware.GetActiveThreadIDs()) {
    hardware.GetThread(id).SetDead();
  }
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 8);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 8);
  hardware.SingleProcess();
  REQUIRE(hardware.ValidateThreadState());
  REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
  REQUIRE(hardware.GetThreadExecOrder().size() == 0);
}

TEST_CASE("Linear Functions Program") {
  using tag_t = emp::BitSet<8>;
  using arg_t = int;
  using program_t = sgp::LinearFunctionsProgram<tag_t, arg_t>;
  program_t program1;
}


TEST_CASE("SignalGP - Linear Functions Program") {
  using mem_model_t = sgp::SimpleMemoryModel;
  using signalgp_t = sgp::LinearFunctionsProgramSignalGP<mem_model_t,
                                                         emp::BitSet<16>,
                                                         int,
                                                         emp::MatchBin< size_t,
                                                                        emp::HammingMetric<16>,
                                                                        emp::RankedSelector<std::ratio<16+8, 16>>,
                                                                        emp::AdditiveCountdownRegulator<>
                                                                      >,
                                                         sgp::DefaultCustomComponent>;
  using inst_lib_t = typename signalgp_t::inst_lib_t;
  using inst_t = typename signalgp_t::inst_t;
  using inst_prop_t = typename signalgp_t::InstProperty;

  using event_lib_t = typename signalgp_t::event_lib_t;
  using program_t = typename signalgp_t::program_t;
  using tag_t = typename signalgp_t::tag_t;

  using mem_buffer_t = typename mem_model_t::mem_buffer_t;

  inst_lib_t inst_lib;
  event_lib_t event_lib;

  // Add some instructions to the instruction library.
  inst_lib.AddInst("Nop", [](signalgp_t & hw, const inst_t & inst) { ; }, "No operation!");
  inst_lib.AddInst("Inc", sgp::inst_impl::Inst_Inc<signalgp_t, inst_t>, "Increment!");
  inst_lib.AddInst("Dec", sgp::inst_impl::Inst_Dec<signalgp_t, inst_t>, "Decrement!");
  inst_lib.AddInst("Not", sgp::inst_impl::Inst_Not<signalgp_t, inst_t>, "Logical not of ARG[0]");
  inst_lib.AddInst("Add", sgp::inst_impl::Inst_Add<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Sub", sgp::inst_impl::Inst_Sub<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Mult", sgp::inst_impl::Inst_Mult<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Div", sgp::inst_impl::Inst_Div<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Mod", sgp::inst_impl::Inst_Mod<signalgp_t, inst_t>, "");
  inst_lib.AddInst("TestEqu", sgp::inst_impl::Inst_TestEqu<signalgp_t, inst_t>, "");
  inst_lib.AddInst("TestNEqu", sgp::inst_impl::Inst_TestNEqu<signalgp_t, inst_t>, "");
  inst_lib.AddInst("TestLess", sgp::inst_impl::Inst_TestLess<signalgp_t, inst_t>, "");
  inst_lib.AddInst("TestLessEqu", sgp::inst_impl::Inst_TestLessEqu<signalgp_t, inst_t>, "");
  inst_lib.AddInst("TestGreater", sgp::inst_impl::Inst_TestGreater<signalgp_t, inst_t>, "");
  inst_lib.AddInst("TestGreaterEqu", sgp::inst_impl::Inst_TestGreaterEqu<signalgp_t, inst_t>, "");
  inst_lib.AddInst("SetMem", sgp::inst_impl::Inst_SetMem<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Close", sgp::inst_impl::Inst_Close<signalgp_t, inst_t>, "", {inst_prop_t::BLOCK_CLOSE});
  inst_lib.AddInst("Break", sgp::inst_impl::Inst_Break<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Call", sgp::inst_impl::Inst_Call<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Return", sgp::inst_impl::Inst_Return<signalgp_t, inst_t>, "");
  inst_lib.AddInst("CopyMem", sgp::inst_impl::Inst_CopyMem<signalgp_t, inst_t>, "");
  inst_lib.AddInst("SwapMem", sgp::inst_impl::Inst_SwapMem<signalgp_t, inst_t>, "");
  inst_lib.AddInst("InputToWorking", sgp::inst_impl::Inst_InputToWorking<signalgp_t, inst_t>, "");
  inst_lib.AddInst("WorkingToOutput", sgp::inst_impl::Inst_WorkingToOutput<signalgp_t, inst_t>, "");
  inst_lib.AddInst("WorkingToGlobal", sgp::inst_impl::Inst_WorkingToGlobal<signalgp_t, inst_t>, "");
  inst_lib.AddInst("GlobalToWorking", sgp::inst_impl::Inst_GlobalToWorking<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Fork", sgp::inst_impl::Inst_Fork<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Terminate", sgp::inst_impl::Inst_Terminate<signalgp_t, inst_t>, "");
  inst_lib.AddInst("If", sgp::lfp_inst_impl::Inst_If<signalgp_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("While", sgp::lfp_inst_impl::Inst_While<signalgp_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("Countdown", sgp::lfp_inst_impl::Inst_Countdown<signalgp_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("Routine", sgp::lfp_inst_impl::Inst_Routine<signalgp_t, inst_t>, "");

  emp::Random random(2);
  signalgp_t hardware(random, inst_lib, event_lib);
  hardware.SetActiveThreadLimit(16);
  REQUIRE(hardware.ValidateThreadState());
  program_t program;

  SECTION ("Inst_Inc") {
    std::cout << "-- Testing Inst_Inc --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    emp_assert(hardware.ValidateThreadState());

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0});
    program.PushInst(inst_lib, "Inc", {1, 0, 0});
    program.PushInst(inst_lib, "Inc", {1, 0, 0});
    program.PushInst(inst_lib, "Inc", {2, 0, 0});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // Inc(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Inc(1)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 1.0}}));
    hardware.SingleProcess(); // Inc(1)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}}));
    hardware.SingleProcess(); // Inc(2)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 1.0}}));
    hardware.SingleProcess(); // Inc(3)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 1.0}, {3, 1.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_Dec") {
    std::cout << "-- Testing Inst_Dec --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0});
    program.PushInst(inst_lib, "Dec", {0, 0, 0});
    program.PushInst(inst_lib, "Dec", {0, 0, 0});
    program.PushInst(inst_lib, "Dec", {1, 0, 0});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // Inc(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Dec(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // Dec(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, -1.0}}));
    hardware.SingleProcess(); // Dec(1)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, -1.0}, {1, -1.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_Not") {
    std::cout << "-- Testing Inst_Not --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0});
    program.PushInst(inst_lib, "Not", {0, 0, 0});
    program.PushInst(inst_lib, "Not", {1, 0, 0});
    program.PushInst(inst_lib, "Dec", {0, 0, 0});
    program.PushInst(inst_lib, "Not", {0, 0, 0});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // Inc(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Not(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // Not(1)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 1.0}}));
    hardware.SingleProcess(); // Dec(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, -1.0}, {1, 1.0}}));
    hardware.SingleProcess(); // Not(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 1.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_Add") {
    std::cout << "-- Testing Inst_Add --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0}); // [0] = 1
    program.PushInst(inst_lib, "Add", {0, 0, 1}); // [1] = 2
    program.PushInst(inst_lib, "Add", {1, 1, 2}); // [2] = 4
    program.PushInst(inst_lib, "Add", {1, 2, 3}); // [3] = 6

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // Inc(0, 0, 0) // [0] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Add(0, 0, 1) // [1] = 2
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}}));
    hardware.SingleProcess(); // Add(1, 1, 2) // [2] = 4
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 4.0}}));
    hardware.SingleProcess(); // Add(1, 2, 3) // [3] = 6
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 4.0}, {3, 6.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_Sub") {
    std::cout << "-- Testing Inst_Sub --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0}); // [0] = 1
    program.PushInst(inst_lib, "Add", {0, 0, 1}); // [1] = 2
    program.PushInst(inst_lib, "Add", {1, 1, 2}); // [2] = 4
    program.PushInst(inst_lib, "Add", {1, 2, 3}); // [3] = 6

    program.PushInst(inst_lib, "Sub", {3, 4, 3}); // [4] = 0; [3] = 6
    program.PushInst(inst_lib, "Sub", {3, 2, 4}); // [4] = 2
    program.PushInst(inst_lib, "Sub", {2, 3, 4}); // [4] = -2

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    // hardware.SingleProcess(); // Inc(0)
    // REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));

    hardware.SingleProcess(); // Inc(0, 0, 0) // [0] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Add(0, 0, 1) // [1] = 2
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}}));
    hardware.SingleProcess(); // Add(1, 1, 2) // [2] = 4
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 4.0}}));
    hardware.SingleProcess(); // Add(1, 2, 3) // [3] = 6
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 4.0}, {3, 6.0}}));
    hardware.SingleProcess(); // Sub(3, 4, 3) // [4] = 0; [3] = 6
    REQUIRE((mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 4.0}, {3, 6.0}, {4, 0.0}})));
    hardware.SingleProcess(); // Sub(3, 2, 4) // [4] = 2
    REQUIRE((mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 4.0}, {3, 6.0}, {4, 2.0}})));
    hardware.SingleProcess(); // Sub(2, 3, 4) // [4] = -2
    REQUIRE((mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 4.0}, {3, 6.0}, {4, -2.0}})));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_Mult") {
    std::cout << "-- Testing Inst_Mult --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0}); // [0] = 1
    program.PushInst(inst_lib, "Add", {0, 0, 1}); // [1] = 2
    program.PushInst(inst_lib, "Dec", {-1, 0, 0}); // [-1] = -1
    program.PushInst(inst_lib, "Mult", {0, 0, 2}); // [2] = 1
    program.PushInst(inst_lib, "Mult", {-1, -1, 2}); // [2] = 1
    program.PushInst(inst_lib, "Mult", {1, 1, 2}); // [2] = 4
    program.PushInst(inst_lib, "Mult", {-1, 1, 2}); // [2] = -2

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // Inc(0, 0, 0) // [0] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Add(0, 0, 1) // [1] = 2
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}}));
    hardware.SingleProcess(); // Dec(-1, 0, 0) // [-1] = -1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{-1, -1.0}, {0, 1.0}, {1, 2.0}}));
    hardware.SingleProcess(); // Mult(0, 0, 2) // [2] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{-1, -1.0}, {0, 1.0}, {1, 2.0}, {2, 1.0}}));
    hardware.SingleProcess(); // Mult(-1, -1, 2) // [2] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{-1, -1.0}, {0, 1.0}, {1, 2.0}, {2, 1.0}}));
    hardware.SingleProcess(); // Mult(1, 1, 2) // [2] = 4
    REQUIRE(mem_state.working_mem == mem_buffer_t({{-1, -1.0}, {0, 1.0}, {1, 2.0}, {2, 4.0}}));
    hardware.SingleProcess(); // Mult(-1, 1, 2) // [2] = -2
    REQUIRE(mem_state.working_mem == mem_buffer_t({{-1, -1.0}, {0, 1.0}, {1, 2.0}, {2, -2.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_Div") {
    std::cout << "-- Testing Inst_Div --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0}); // [0] = 1
    program.PushInst(inst_lib, "Add", {0, 0, 1}); // [1] = 2
    program.PushInst(inst_lib, "Div", {2, 2, 2}); // [2] = 0   Do nothing.
    program.PushInst(inst_lib, "Div", {2, 2, 2}); // [2] = 0   Do nothing.
    program.PushInst(inst_lib, "Div", {0, 1, 3}); // [3] = 0.5 Do nothing.

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // Inc(0, 0, 0) // [0] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Add(0, 0, 1) // [1] = 2
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}}));
    hardware.SingleProcess(); // Div(2, 2, 2); // [2] = 0   Do nothing.
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 0.0}}));
    hardware.SingleProcess(); // Div(2, 2, 2); // [2] = 0   Do nothing.
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 0.0}}));
    hardware.SingleProcess(); // Div(0, 1, 3); // [3] = 0.5 Do nothing.
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 0.0}, {3, 0.5}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_Mod") {
    std::cout << "-- Testing Inst_Mod --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0}); // [0] = 1
    program.PushInst(inst_lib, "Add", {0, 0, 1}); // [1] = 2
    program.PushInst(inst_lib, "Mod", {2, 2, 2}); // [2] = 0   Do nothing.
    program.PushInst(inst_lib, "Mod", {2, 2, 2}); // [2] = 0   Do nothing.
    program.PushInst(inst_lib, "Mod", {0, 1, 3}); // [3] = 1

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // Inc(0, 0, 0) // [0] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Add(0, 0, 1) // [1] = 2
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}}));
    hardware.SingleProcess(); // Mod(2, 2, 2); // [2] = 0   Do nothing.
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 0.0}}));
    hardware.SingleProcess(); // Mod(2, 2, 2); // [2] = 0   Do nothing.
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 0.0}}));
    hardware.SingleProcess(); // Mod(0, 1, 3); // [3] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 0.0}, {3, 1.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_SetMem") {
    std::cout << "-- Testing Inst_SetMem --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "SetMem", {0, 0, 0});
    program.PushInst(inst_lib, "SetMem", {1, 1, 0});
    program.PushInst(inst_lib, "SetMem", {2, 2, 0});
    program.PushInst(inst_lib, "SetMem", {3, -128, 0});
    program.PushInst(inst_lib, "SetMem", {4, 256, 0});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // SetMem(0, 0, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // SetMem(1, 1, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 1.0}}));
    hardware.SingleProcess(); // SetMem(2, 2, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 2.0}}));
    hardware.SingleProcess(); // SetMem(3, -128, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 2.0}, {3, -128.0}}));
    hardware.SingleProcess(); // SetMem(4, 256, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 2.0}, {3, -128.0}, {4, 256}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_TestEqu") {
    std::cout << "-- Testing Inst_TestEqu --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "SetMem", {0, 0, 0});
    program.PushInst(inst_lib, "SetMem", {1, 0, 0});
    program.PushInst(inst_lib, "SetMem", {2, 2, 0});
    program.PushInst(inst_lib, "TestEqu", {0, 1, 3});
    program.PushInst(inst_lib, "TestEqu", {0, 2, 3});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // SetMem(0, 0, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // SetMem{1, 0, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}}));
    hardware.SingleProcess(); // SetMem{2, 2, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}}));
    hardware.SingleProcess(); // TestEqu{0, 1, 3}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));
    hardware.SingleProcess(); // TestEqu{0, 2, 3}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_TestNEqu") {
    std::cout << "-- Testing Inst_TestNEqu --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "SetMem", {0, 0, 0});
    program.PushInst(inst_lib, "SetMem", {1, 0, 0});
    program.PushInst(inst_lib, "SetMem", {2, 2, 0});
    program.PushInst(inst_lib, "TestNEqu", {0, 1, 3});
    program.PushInst(inst_lib, "TestNEqu", {0, 2, 3});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // SetMem(0, 0, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // SetMem{1, 0, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}}));
    hardware.SingleProcess(); // SetMem{2, 2, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_TestLess") {
    std::cout << "-- Testing Inst_TestLess --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "SetMem", {0, 0, 0});
    program.PushInst(inst_lib, "SetMem", {1, 0, 0});
    program.PushInst(inst_lib, "SetMem", {2, 2, 0});
    program.PushInst(inst_lib, "TestLess", {0, 1, 3});
    program.PushInst(inst_lib, "TestLess", {0, 2, 3});
    program.PushInst(inst_lib, "TestLess", {2, 0, 3});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // SetMem(0, 0, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // SetMem{1, 0, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}}));
    hardware.SingleProcess(); // SetMem{2, 2, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_TestLessEqu") {
    std::cout << "-- Testing Inst_TestLessEqu --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "SetMem", {0, 0, 0});
    program.PushInst(inst_lib, "SetMem", {1, 0, 0});
    program.PushInst(inst_lib, "SetMem", {2, 2, 0});
    program.PushInst(inst_lib, "TestLessEqu", {0, 1, 3});
    program.PushInst(inst_lib, "TestLessEqu", {0, 2, 3});
    program.PushInst(inst_lib, "TestLessEqu", {2, 0, 3});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // SetMem(0, 0, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // SetMem{1, 0, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}}));
    hardware.SingleProcess(); // SetMem{2, 2, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_TestGreater") {
    std::cout << "-- Testing Inst_TestGreater --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "SetMem", {0, 0, 0});
    program.PushInst(inst_lib, "SetMem", {1, 0, 0});
    program.PushInst(inst_lib, "SetMem", {2, 2, 0});
    program.PushInst(inst_lib, "TestGreater", {0, 1, 3});
    program.PushInst(inst_lib, "TestGreater", {0, 2, 3});
    program.PushInst(inst_lib, "TestGreater", {2, 0, 3});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // SetMem(0, 0, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // SetMem{1, 0, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}}));
    hardware.SingleProcess(); // SetMem{2, 2, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_TestGreaterEqu") {
    std::cout << "-- Testing Inst_TestGreaterEqu --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "SetMem", {0, 0, 0});
    program.PushInst(inst_lib, "SetMem", {1, 0, 0});
    program.PushInst(inst_lib, "SetMem", {2, 2, 0});
    program.PushInst(inst_lib, "TestGreaterEqu", {0, 1, 3});
    program.PushInst(inst_lib, "TestGreaterEqu", {0, 2, 3});
    program.PushInst(inst_lib, "TestGreaterEqu", {2, 0, 3});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // SetMem(0, 0, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // SetMem{1, 0, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}}));
    hardware.SingleProcess(); // SetMem{2, 2, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_If") {
    std::cout << "-- Testing Inst_TestIf --" << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    // If(true), ____, ____, ____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "If", {1, 0, 0});
    program.PushInst(inst_lib, "Inc", {2, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess(); // If
    hardware.SingleProcess(); // Inc
    hardware.SingleProcess(); // Nop
    hardware.SingleProcess(); // Nop
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 1.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , If(true), ____, ____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {1, 0, 0});
    program.PushInst(inst_lib, "Inc", {2, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 1.0}, {3, 1.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , ____ , ____, If(true)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {3, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 3.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // if(false) , ____ , ____, ____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "If",  {0, 0, 0});
    program.PushInst(inst_lib, "Inc", {2, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , if(false) , ____, ____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {0, 0, 0});
    program.PushInst(inst_lib, "Inc", {2, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , ____ , ____, If(false)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 3.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , ____ , DEF, _____, if(true) , _____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {4, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "Nop", {5, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {4, 1.0}, {5, 2.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, if(true) , _____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {4, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "Close", {5, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {4, 1.0}, {5, 2.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, if(true)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "Close", {5, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {5, 2.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, if(false) , _____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {0, 0, 0});
    program.PushInst(inst_lib, "Inc", {4, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "Close", {5, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {5, 1.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, if(false)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {0, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "Close", {5, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {5, 1.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////
  }

  SECTION ("Inst_While") {
    std::cout << "-- Testing Inst_While --" << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    // While(true), ____, ____, ____  => Infinite loop.
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "While", {1, 0, 0});
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "Inc",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc",   {4, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 1.0}, {3, 1.0}, {4, 1.0}}));
    for (size_t i = 0; i < 4; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 2.0}, {3, 2.0}, {4, 2.0}}));
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , While(true), ____, ____   => Infinite loop.
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "While", {1, 0, 0});
    program.PushInst(inst_lib, "Inc",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc",   {4, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 1.0}, {3, 1.0}, {4, 1.0}}));
    for (size_t i = 0; i < 3; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 1.0}, {3, 2.0}, {4, 2.0}}));
    for (size_t i = 0; i < 32; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetActiveThreadIDs().size());
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , ____ , ____, While(true)   => Infinite loop.
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc",   {3, 0, 0});
    program.PushInst(inst_lib, "While", {3, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 3.0}}));
    for (size_t i = 0; i < 32; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetActiveThreadIDs().size());
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 3.0}}));
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // While(false) , ____ , ____, ____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "While", {0, 0, 0});
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "Inc",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc",   {4, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , While(false) , ____, ____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "While", {0, 0, 0});
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , ____ , ____, While(false)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "While", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 3.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , ____ , DEF, _____, While(true) , _____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Inc",   {3, 0, 0});
    program.PushInst(inst_lib, "While", {3, 0, 0});
    program.PushInst(inst_lib, "Inc",   {4, 0, 0});
    program.PushInst(inst_lib, "Inc",   {5, 0, 0});
    program.PushInst(inst_lib, "Nop",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",   {5, 0, 0});
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess(); // Inc
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // Inc
    hardware.SingleProcess(); // Inc
    hardware.SingleProcess(); // Nop
    hardware.SingleProcess(); // Inc
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {4, 1.0}, {5, 2.0}}));
    for (size_t i = 0; i < 5; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {4, 2.0}, {5, 4.0}}));
    for (size_t i = 0; i < 32; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetActiveThreadIDs().size());
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, While(true) , _____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc",     {4, 0, 0});
    program.PushInst(inst_lib, "Inc",     {5, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess(); // Inc
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // Inc(4)
    hardware.SingleProcess(); // Inc(5)
    hardware.SingleProcess(); // Close
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {4, 1.0}, {5, 1.0}}));
    for (size_t i = 0; i < 4; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {4, 2.0}, {5, 2.0}}));
    for (size_t i = 0; i < 32; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetActiveThreadIDs().size());
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, While(true)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc",     {5, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // Inc(5)
    hardware.SingleProcess(); // Close
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {5, 1.0}}));
    for (size_t i = 0; i < 3; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {5, 2.0}}));
    for (size_t i = 0; i < 32; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetActiveThreadIDs().size());
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, While(false) , _____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {0, 0, 0});
    program.PushInst(inst_lib, "Inc",     {4, 0, 0});
    program.PushInst(inst_lib, "Inc",     {5, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // Inc(6)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {6, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, While(false)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {0, 0, 0});
    program.PushInst(inst_lib, "Inc",     {5, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // Inc(6)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {6, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // CLOSE, ____ , DEF, _____, While(false)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {0, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // Inc(6)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {6, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // if(false), INC, CLOSE, INC, CLOSE, ____ , DEF, _____, While(True)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {1, 0, 0});
    program.PushInst(inst_lib, "If",      {0, 0, 0});
    program.PushInst(inst_lib, "Inc",     {7, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushInst(inst_lib, "Close",   {0, 0, 0});
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // If(false)
    hardware.SingleProcess(); // Inc(6)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {6, 1.0}}));
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // if(false), INC, CLOSE, INC, CLOSE, ____ , DEF, _____, While(false)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {0, 0, 0});
    program.PushInst(inst_lib, "If",      {0, 0, 0});
    program.PushInst(inst_lib, "Inc",     {7, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushInst(inst_lib, "Close",   {0, 0, 0});
    program.PushFunction(tag_t());
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    program.PushInst(inst_lib, "Nop",   {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(false)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}}));
    ////////////////////////////////////////////////////////////////////////////
  }

  SECTION ("Inst_Countdown") {
    std::cout << "-- Testing Inst_Countdown --" << std::endl;
    // Countdown instruction is pretty much the same as the While.
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "Countdown",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // Countdown(true)
    hardware.SingleProcess(); // Inc(6)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}}));
    for (size_t i = 0; i < 10; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}}));
    REQUIRE(hardware.GetActiveThreadIDs().size());
  }

  SECTION ("Inst_Break") {
    std::cout << "-- Testing Inst_Break --" << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {1, 0, 0});
    program.PushInst(inst_lib, "If",      {1, 0, 0});
    program.PushInst(inst_lib, "Break",   {0, 0, 0});
    program.PushInst(inst_lib, "Inc",     {7, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushInst(inst_lib, "Close",   {0, 0, 0});
    program.PushInst(inst_lib, "Inc",     {8, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // If(true)
    hardware.SingleProcess(); // Break
    hardware.SingleProcess(); // Inc(8)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {8, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc",       {3, 0, 0});
    program.PushInst(inst_lib, "If",        {1, 0, 0});
    program.PushInst(inst_lib, "If",        {1, 0, 0});
    program.PushInst(inst_lib, "Break",     {0, 0, 0});
    program.PushInst(inst_lib, "Inc",       {7, 0, 0});
    program.PushInst(inst_lib, "Close",     {5, 0, 0});
    program.PushInst(inst_lib, "Inc",       {6, 0, 0});
    program.PushInst(inst_lib, "Close",     {0, 0, 0});
    program.PushInst(inst_lib, "Inc",       {8, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);
    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // If(true)
    hardware.SingleProcess(); // If(true)
    hardware.SingleProcess(); // Break
    hardware.SingleProcess(); // Inc(7)
    hardware.SingleProcess(); // Close
    hardware.SingleProcess(); // Inc(6)
    hardware.SingleProcess(); // Close
    hardware.SingleProcess(); // Inc(8)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {6, 1.0}, {7, 1.0}, {8, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////
  }

  SECTION ("Inst_Call") {
    std::cout << "-- Testing Inst_Call --" << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    tag_t zeros, ones;
    ones.SetUInt(0, (uint16_t)-1);
    program.PushFunction(zeros);
    program.PushInst(inst_lib,   "SetMem", {2, 2});
    program.PushInst(inst_lib,   "SetMem", {3, 3});
    program.PushInst(inst_lib,   "Call", {0, 0, 0}, {ones});
    program.PushFunction(ones);
    program.PushInst(inst_lib,   "InputToWorking", {2, 1, 0});
    program.PushInst(inst_lib,   "InputToWorking", {3, 2, 0});
    program.PushInst(inst_lib,   "Inc", {1, 0, 0});
    program.PushInst(inst_lib,   "Inc", {2, 0, 0});
    program.PushInst(inst_lib,   "WorkingToOutput", {1, 4, 0});
    program.PushInst(inst_lib,   "WorkingToOutput", {2, 5, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    size_t thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    for (size_t i = 0; i < 10; ++i) hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{4, 3.0}, {5, 4.0}, {2, 2.0}, {3, 3.0}}));
    ////////////////////////////////////////////////////////////////////////////
  }

  SECTION ("Inst_Routine") {
    std::cout << "-- Testing Inst_Routine --" << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    tag_t zeros, ones;
    ones.SetUInt(0, (uint16_t)-1);
    program.PushFunction(zeros);
    program.PushInst(inst_lib,   "SetMem", {2, 2});
    program.PushInst(inst_lib,   "SetMem", {3, 3});
    program.PushInst(inst_lib,   "Routine", {0, 0, 0}, {ones});
    program.PushInst(inst_lib,   "SetMem", {4, 4});
    program.PushFunction(ones);
    program.PushInst(inst_lib,   "Inc", {0, 0, 0});
    program.PushInst(inst_lib,   "Inc", {1, 0, 0});
    program.PushInst(inst_lib,   "Inc", {2, 0, 0});
    program.PushInst(inst_lib,   "Inc", {3, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    size_t thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 1);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    hardware.SingleProcess(); // SetMem
    hardware.SingleProcess(); // Routine
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 2);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    for (size_t i = 0; i < 4; ++i) hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 2);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{0, 1.0}, {1, 1.0}, {2, 3.0}, {3, 4.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{0, 1.0}, {1, 1.0}, {2, 3.0}, {3, 4.0}, {4, 4.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////
  }

  SECTION ("Inst_Return") {
    std::cout << "-- Testing Inst_Return --" << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    // Return from 1 Call (=> thread should die)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    tag_t zeros, ones;
    ones.SetUInt(0, (uint16_t)-1);
    program.PushFunction(zeros);
    program.PushInst(inst_lib,   "SetMem", {2, 2});
    program.PushInst(inst_lib,   "SetMem", {3, 3});
    program.PushInst(inst_lib,   "Return", {0, 0, 0});
    program.PushInst(inst_lib,   "SetMem", {4, 4});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    size_t thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 1);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}}));
    hardware.SingleProcess(); // Return
    REQUIRE(hardware.ValidateThreadState());
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 0);
    REQUIRE(hardware.GetThread(thread_id).IsDead());
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    REQUIRE(hardware.ValidateThreadState());
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Return from 2 Calls (=> should only return from top call)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    ones.SetUInt(0, (uint16_t)-1);
    program.PushFunction(zeros);
    program.PushInst(inst_lib,   "SetMem", {2, 2});
    program.PushInst(inst_lib,   "SetMem", {3, 3});
    program.PushInst(inst_lib,   "Call", {0, 0, 0}, {ones});
    program.PushInst(inst_lib,   "SetMem", {4, 4});
    program.PushFunction(ones);
    program.PushInst(inst_lib,   "SetMem", {2, 4});
    program.PushInst(inst_lib,   "SetMem", {5, 10});
    program.PushInst(inst_lib,   "Return", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 1);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}}));
    hardware.SingleProcess(); // Call
    REQUIRE(hardware.ValidateThreadState());
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 2);
    hardware.SingleProcess(); // SetMem
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 4.0}, {5, 10.0}}));
    hardware.SingleProcess(); // Return
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{2, 2.0}, {3, 3.0}, {4, 4.0}}));
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    REQUIRE(hardware.ValidateThreadState());
    hardware.SingleProcess(); // EOP
    REQUIRE(hardware.GetThread(thread_id).IsDead());
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    REQUIRE(hardware.ValidateThreadState());
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Return from Call from within BASIC block
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    program.PushFunction(zeros);
    program.PushInst(inst_lib,   "Inc", {0, 0, 0});
    program.PushInst(inst_lib,   "If",  {0, 0, 0});
    program.PushInst(inst_lib,   "Return", {0, 0, 0});
    program.PushInst(inst_lib,   "Close",  {0, 0, 0});
    program.PushInst(inst_lib,   "SetMem", {4, 4});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    hardware.SingleProcess(); // Inc
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 1);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    hardware.SingleProcess(); // If
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Return
    REQUIRE(hardware.ValidateThreadState());
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 0);
    REQUIRE(hardware.GetThread(thread_id).IsDead());
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    REQUIRE(hardware.ValidateThreadState());
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Return from Call from within 2 BASIC blocks
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    program.PushFunction(zeros);
    program.PushInst(inst_lib,   "Inc", {0, 0, 0});
    program.PushInst(inst_lib,   "If",  {0, 0, 0});
    program.PushInst(inst_lib,   "While",  {0, 0, 0});
    program.PushInst(inst_lib,   "Return", {0, 0, 0});
    program.PushInst(inst_lib,   "Close",  {0, 0, 0});
    program.PushInst(inst_lib,   "Close",  {0, 0, 0});
    program.PushInst(inst_lib,   "SetMem", {4, 4});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    hardware.SingleProcess(); // Inc
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 1);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    hardware.SingleProcess(); // If
    hardware.SingleProcess(); // While
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Return
    REQUIRE(hardware.ValidateThreadState());
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 0);
    REQUIRE(hardware.GetThread(thread_id).IsDead());
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    REQUIRE(hardware.ValidateThreadState());
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Return from 1 Routine (=> thread should live on)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    program.PushFunction(zeros);
    program.PushInst(inst_lib,   "SetMem", {2, 2});
    program.PushInst(inst_lib,   "SetMem", {3, 3});
    program.PushInst(inst_lib,   "Routine", {0, 0, 0}, {ones});
    program.PushInst(inst_lib,   "SetMem", {4, 4});
    program.PushFunction(ones);
    program.PushInst(inst_lib,   "SetMem", {5, 5});
    program.PushInst(inst_lib,   "Return", {0, 0, 0});
    program.PushInst(inst_lib,   "SetMem", {6, 6});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 1);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}}));
    hardware.SingleProcess(); // Routine
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 2);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    hardware.SingleProcess(); // SetMem(5,5)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}, {5, 5.0}}));
    hardware.SingleProcess(); // Return
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}, {5, 5.0}}));
    hardware.SingleProcess(); // SetMem(4, 4)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}, {5, 5.0}, {4, 4.0}}));
    hardware.SingleProcess(); // EOP
    REQUIRE(hardware.ValidateThreadState());
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 0);
    REQUIRE(hardware.GetThread(thread_id).IsDead());
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Return from 2 Routines (=> should just return from top)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    tag_t two;
    two.SetUInt(0, (uint32_t)2);
    program.PushFunction(zeros);
    program.PushInst(inst_lib,   "SetMem", {2, 2});
    program.PushInst(inst_lib,   "SetMem", {3, 3});
    program.PushInst(inst_lib,   "Routine", {0, 0, 0}, {ones});
    program.PushInst(inst_lib,   "SetMem", {4, 4});
    program.PushFunction(ones);
    program.PushInst(inst_lib,   "SetMem", {5, 5});
    program.PushInst(inst_lib,   "Routine", {0, 0, 0}, {two});
    program.PushInst(inst_lib,   "SetMem", {6, 6});
    program.PushFunction(two);
    program.PushInst(inst_lib,   "SetMem", {7, 7});
    program.PushInst(inst_lib,   "Return", {0, 0, 0});
    program.PushInst(inst_lib,   "SetMem", {8, 8});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 1);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}}));
    hardware.SingleProcess(); // Routine
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 2);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    hardware.SingleProcess(); // SetMem(5,5)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}, {5, 5.0}}));
    hardware.SingleProcess(); // Routine
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}, {5, 5.0}}));
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 3);
    hardware.SingleProcess(); // SetMem(7,7)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}, {5, 5.0}, {7, 7.0}}));
    hardware.SingleProcess(); // Return
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 2);
    hardware.SingleProcess(); // SetMem(6,6)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}, {5, 5.0}, {7, 7.0}, {6, 6.0}}));
    hardware.SingleProcess(); // EOM, SetMem(4,4)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}, {5, 5.0}, {7, 7.0}, {6, 6.0}, {4, 4.0}}));
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 1);
    hardware.SingleProcess(); // EOP
    REQUIRE(hardware.ValidateThreadState());
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 0);
    REQUIRE(hardware.GetThread(thread_id).IsDead());
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Return from Routine from within BASIC block
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    program.PushFunction(zeros);
    program.PushInst(inst_lib,   "SetMem", {1, 1});
    program.PushInst(inst_lib,   "Routine", {0, 0, 0}, {ones});
    program.PushInst(inst_lib,   "SetMem", {5, 5});
    program.PushFunction(ones);
    program.PushInst(inst_lib,   "If",  {1});
    program.PushInst(inst_lib,   "Return", {0, 0, 0});
    program.PushInst(inst_lib,   "Close",  {0, 0, 0});
    program.PushInst(inst_lib,   "SetMem", {4, 4});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    hardware.SingleProcess(); // SetMem
    hardware.SingleProcess(); // Routine
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 2);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    hardware.SingleProcess(); // If
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{1, 1.0}}));
    hardware.SingleProcess(); // Return
    hardware.SingleProcess(); // SetMem(5,5)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{1, 1.0}, {5, 5.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.ValidateThreadState());
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 0);
    REQUIRE(hardware.GetThread(thread_id).IsDead());
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    REQUIRE(hardware.ValidateThreadState());
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Return from Routine from within BASIC block
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    program.PushFunction(zeros);
    program.PushInst(inst_lib,   "SetMem", {1, 1});
    program.PushInst(inst_lib,   "Routine", {0, 0, 0}, {ones});
    program.PushInst(inst_lib,   "SetMem", {5, 5});
    program.PushFunction(ones);
    program.PushInst(inst_lib,   "If",  {1});
    program.PushInst(inst_lib,   "While",  {1});
    program.PushInst(inst_lib,   "Return", {0, 0, 0});
    program.PushInst(inst_lib,   "Close",  {0, 0, 0});
    program.PushInst(inst_lib,   "Close",  {0, 0, 0});
    program.PushInst(inst_lib,   "SetMem", {4, 4});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    hardware.SingleProcess(); // SetMem
    hardware.SingleProcess(); // Routine
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 2);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    hardware.SingleProcess(); // If
    hardware.SingleProcess(); // While
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{1, 1.0}}));
    hardware.SingleProcess(); // Return
    hardware.SingleProcess(); // SetMem(5,5)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{1, 1.0}, {5, 5.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.ValidateThreadState());
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 0);
    REQUIRE(hardware.GetThread(thread_id).IsDead());
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    REQUIRE(hardware.ValidateThreadState());
    ////////////////////////////////////////////////////////////////////////////
  }

  SECTION ("Inst_CopyMem") {
    std::cout << "-- Testing Inst_CopyMem --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    program.PushInst(inst_lib, "SetMem", {1, 1, 0});
    program.PushInst(inst_lib, "CopyMem", {1, 2, 0});
    program.PushInst(inst_lib, "CopyMem", {2, 3, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    size_t thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);
    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();
    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());
    hardware.SingleProcess(); // SetMem(1, 1, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{1, 1.0}}));
    hardware.SingleProcess(); // CopyMem(1, 2, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{1, 1.0}, {2, 1.0}}));
    hardware.SingleProcess(); // CopyMem(2, 3, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{1, 1.0}, {2, 1.0}, {3, 1.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_SwapMem") {
    std::cout << "-- Testing Inst_SwapMem --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    program.PushInst(inst_lib, "SetMem", {1, 1, 0});
    program.PushInst(inst_lib, "SetMem", {3, 3, 0});
    program.PushInst(inst_lib, "SwapMem", {1, 2, 0});
    program.PushInst(inst_lib, "SwapMem", {2, 3, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    size_t thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);
    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();
    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());
    hardware.SingleProcess(); // SetMem(1, 1, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{1, 1.0}}));
    hardware.SingleProcess(); // SetMem(3, 3, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{1, 1.0}, {3, 3.0}}));
    hardware.SingleProcess(); // SwapMem(1, 2, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{1, 0.0}, {2, 1.0}, {3, 3.0}}));
    hardware.SingleProcess(); // SwapMem(2, 3, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{1, 0.0}, {3, 1.0}, {2, 3.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_WorkingToGlobal") {
    std::cout << "-- Testing Inst_WorkingToGlobal --" << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    tag_t zeros, ones;
    ones.SetUInt(0, (uint16_t)-1);
    program.PushFunction(zeros);
    program.PushInst(inst_lib, "SetMem", {2, 2});
    program.PushInst(inst_lib, "WorkingToGlobal", {2, 1, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    size_t thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}}));
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer() == mem_buffer_t());
    hardware.SingleProcess(); // WorkingToGlobal
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}}));
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer() == mem_buffer_t({{1, 2.0}}));
  }

  SECTION ("Inst_GlobalToWorking") {
    std::cout << "-- Testing Inst_GlobalToWorking --" << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    tag_t zeros, ones;
    ones.SetUInt(0, (uint16_t)-1);
    program.PushFunction(zeros);
    program.PushInst(inst_lib, "SetMem", {2, 2});
    program.PushInst(inst_lib, "WorkingToGlobal", {2, 1, 0});
    program.PushInst(inst_lib, "GlobalToWorking", {1, 3, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    size_t thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}}));
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer() == mem_buffer_t());
    hardware.SingleProcess(); // WorkingToGlobal
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}}));
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer() == mem_buffer_t({{1, 2.0}}));
    hardware.SingleProcess(); // GlobalToWorking
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 2.0}}));
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer() == mem_buffer_t({{1, 2.0}}));
  }

  SECTION ("Inst_Fork") {
    std::cout << "-- Testing Inst_Fork --" << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    tag_t zeros, ones, two;
    ones.SetUInt(0, (uint16_t)-1);
    two.SetUInt(0, (uint16_t)2);
    program.PushFunction(zeros);
    program.PushInst(inst_lib,   "SetMem", {2, 2});
    program.PushInst(inst_lib,   "SetMem", {3, 3});
    program.PushInst(inst_lib,   "Fork", {0, 0, 0}, {ones});
    program.PushInst(inst_lib,   "SetMem", {4, 4});
    program.PushFunction(ones);
    program.PushInst(inst_lib,   "Nop", {0, 0, 0});
    program.PushInst(inst_lib,   "Fork", {0, 0, 0}, {two});
    program.PushInst(inst_lib,   "Nop", {0, 0, 0});
    program.PushFunction(two);
    program.PushInst(inst_lib,   "Nop", {0, 0, 0});
    program.PushInst(inst_lib,   "Nop", {0, 0, 0});
    program.PushInst(inst_lib,   "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    size_t thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 1);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}}));
    hardware.SingleProcess(); // Fork
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    REQUIRE(hardware.GetActiveThreadIDs().size() == 1);
    REQUIRE(hardware.ValidateThreadState());
    hardware.SingleProcess(); // [0]: SetMem(4,4), [1]: Nop
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}, {4, 4.0}}));
    REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
    REQUIRE(hardware.GetActiveThreadIDs().size() == 2);
    REQUIRE(hardware.ValidateThreadState());
    hardware.SingleProcess(); // [0]: DEAD, [1]: Fork
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    REQUIRE(hardware.GetActiveThreadIDs().size() == 1);
    REQUIRE(hardware.ValidateThreadState());
    hardware.SingleProcess(); // [0]: DEAD, [1]: Nop, [2]: Nop
    REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
    REQUIRE(hardware.GetActiveThreadIDs().size() == 2);
    REQUIRE(hardware.ValidateThreadState());
    hardware.SingleProcess(); // [0]: DEAD, [1]: DEAD, [2]: Nop
    REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
    REQUIRE(hardware.GetActiveThreadIDs().size() == 1);
    REQUIRE(hardware.ValidateThreadState());
    hardware.SingleProcess(); // [0]: DEAD, [1]: DEAD, [2]: Nop
    REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
    REQUIRE(hardware.GetActiveThreadIDs().size() == 1);
    REQUIRE(hardware.ValidateThreadState());
    hardware.SingleProcess(); // [0]: DEAD, [1]: DEAD, [2]: DEAD
    REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    REQUIRE(hardware.ValidateThreadState());
    ////////////////////////////////////////////////////////////////////////////
  }

  SECTION ("Inst_Terminate") {
    std::cout << "-- Testing Inst_Terminate --" << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    tag_t zeros, ones, two;
    ones.SetUInt(0, (uint16_t)-1);
    two.SetUInt(0, (uint16_t)2);
    program.PushFunction(zeros);
    program.PushInst(inst_lib,   "SetMem", {2, 2});
    program.PushInst(inst_lib,   "SetMem", {3, 3});
    program.PushInst(inst_lib,   "Fork", {0, 0, 0}, {ones});
    program.PushInst(inst_lib,   "SetMem", {4, 4});
    program.PushInst(inst_lib,   "Nop", {0, 0, 0});
    program.PushInst(inst_lib,   "Nop", {0, 0, 0});
    program.PushInst(inst_lib,   "Nop", {0, 0, 0});
    program.PushInst(inst_lib,   "Nop", {0, 0, 0});
    program.PushFunction(ones);
    program.PushInst(inst_lib,   "Nop", {0, 0, 0});
    program.PushInst(inst_lib,   "Fork", {0, 0, 0}, {two});
    program.PushInst(inst_lib,   "Terminate", {0, 0, 0});
    program.PushInst(inst_lib,   "Nop", {0, 0, 0});
    program.PushFunction(two);
    program.PushInst(inst_lib,   "Nop", {0, 0, 0});
    program.PushInst(inst_lib,   "Terminate", {0, 0, 0});
    program.PushInst(inst_lib,   "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    REQUIRE(spawned);
    size_t thread_id = spawned.value();
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetFlowStack().size() == 1);
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetCallStack().size() == 1);
    hardware.SingleProcess(); // SetMem
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}}));
    hardware.SingleProcess(); // Fork
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    REQUIRE(hardware.GetActiveThreadIDs().size() == 1);
    REQUIRE(hardware.ValidateThreadState());
    hardware.SingleProcess(); // [0]: SetMem(4,4), [1]: Nop
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{2, 2.0}, {3, 3.0}, {4, 4.0}}));
    REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
    REQUIRE(hardware.GetActiveThreadIDs().size() == 2);
    REQUIRE(hardware.ValidateThreadState());

    hardware.SingleProcess(); // [0]: Nop, [1]: Fork
    REQUIRE(hardware.GetPendingThreadIDs().size() == 1);
    REQUIRE(hardware.GetActiveThreadIDs().size() == 2);
    REQUIRE(hardware.ValidateThreadState());

    hardware.SingleProcess(); // [0]: Nop, [1]: Terminate - DEAD, [2]: Nop
    REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
    REQUIRE(hardware.GetActiveThreadIDs().size() == 2);
    REQUIRE(hardware.ValidateThreadState());

    hardware.SingleProcess(); // [0]: Nop, [1]: DEAD, [2]: Terminate - DEAD
    REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
    REQUIRE(hardware.GetActiveThreadIDs().size() == 1);
    REQUIRE(hardware.ValidateThreadState());

    hardware.SingleProcess(); // [0]: Nop, [1]: DEAD, [2]: Terminate - DEAD
    REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
    REQUIRE(hardware.GetActiveThreadIDs().size() == 1);
    REQUIRE(hardware.ValidateThreadState());

    hardware.SingleProcess(); // [0]: EOM, [1]: DEAD, [2]: Terminate - DEAD
    REQUIRE(hardware.GetPendingThreadIDs().size() == 0);
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    REQUIRE(hardware.ValidateThreadState());
    ////////////////////////////////////////////////////////////////////////////
  }
}

TEST_CASE("SignalGP - Linear Program", "[general]") {
  using mem_model_t = sgp::SimpleMemoryModel;
  using signalgp_t = sgp::LinearProgramSignalGP<mem_model_t,
                                                emp::BitSet<16>,
                                                int,
                                                emp::MatchBin< size_t,
                                                               emp::HammingMetric<16>,
                                                               emp::RankedSelector<std::ratio<16+8, 16>>,
                                                               emp::AdditiveCountdownRegulator<>
                                                              >,
                                                sgp::DefaultCustomComponent>;
  using inst_lib_t = typename signalgp_t::inst_lib_t;
  using inst_t = typename signalgp_t::inst_t;
  using inst_prop_t = typename signalgp_t::InstProperty;

  using event_lib_t = typename signalgp_t::event_lib_t;
  using program_t = typename signalgp_t::program_t;
  using tag_t = typename signalgp_t::tag_t;

  using mem_buffer_t = typename mem_model_t::mem_buffer_t;

  inst_lib_t inst_lib;
  event_lib_t event_lib;

  // Add some instructions to the instruction library.
  inst_lib.AddInst("Nop", [](signalgp_t & hw, const inst_t & inst) { ; }, "No operation!");
  inst_lib.AddInst("ModuleDef", [](signalgp_t & hw, const inst_t & inst) { ; }, "Module definition", {inst_prop_t::MODULE});
  inst_lib.AddInst("Inc", sgp::inst_impl::Inst_Inc<signalgp_t, inst_t>, "Increment!");
  inst_lib.AddInst("Dec", sgp::inst_impl::Inst_Dec<signalgp_t, inst_t>, "Decrement!");
  inst_lib.AddInst("Not", sgp::inst_impl::Inst_Not<signalgp_t, inst_t>, "Logical not of ARG[0]");
  inst_lib.AddInst("Add", sgp::inst_impl::Inst_Add<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Sub", sgp::inst_impl::Inst_Sub<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Mult", sgp::inst_impl::Inst_Mult<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Div", sgp::inst_impl::Inst_Div<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Mod", sgp::inst_impl::Inst_Mod<signalgp_t, inst_t>, "");
  inst_lib.AddInst("TestEqu", sgp::inst_impl::Inst_TestEqu<signalgp_t, inst_t>, "");
  inst_lib.AddInst("TestNEqu", sgp::inst_impl::Inst_TestNEqu<signalgp_t, inst_t>, "");
  inst_lib.AddInst("TestLess", sgp::inst_impl::Inst_TestLess<signalgp_t, inst_t>, "");
  inst_lib.AddInst("TestLessEqu", sgp::inst_impl::Inst_TestLessEqu<signalgp_t, inst_t>, "");
  inst_lib.AddInst("TestGreater", sgp::inst_impl::Inst_TestGreater<signalgp_t, inst_t>, "");
  inst_lib.AddInst("TestGreaterEqu", sgp::inst_impl::Inst_TestGreaterEqu<signalgp_t, inst_t>, "");
  inst_lib.AddInst("SetMem", sgp::inst_impl::Inst_SetMem<signalgp_t, inst_t>, "");
  inst_lib.AddInst("If", sgp::inst_impl::Inst_If<signalgp_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("While", sgp::inst_impl::Inst_While<signalgp_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("Countdown", sgp::inst_impl::Inst_Countdown<signalgp_t, inst_t>, "", {inst_prop_t::BLOCK_DEF});
  inst_lib.AddInst("Break", sgp::inst_impl::Inst_Break<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Close", sgp::inst_impl::Inst_Close<signalgp_t, inst_t>, "", {inst_prop_t::BLOCK_CLOSE});
  inst_lib.AddInst("Call", sgp::inst_impl::Inst_Call<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Routine", sgp::inst_impl::Inst_Routine<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Return", sgp::inst_impl::Inst_Return<signalgp_t, inst_t>, "");
  inst_lib.AddInst("CopyMem", sgp::inst_impl::Inst_CopyMem<signalgp_t, inst_t>, "");
  inst_lib.AddInst("SwapMem", sgp::inst_impl::Inst_SwapMem<signalgp_t, inst_t>, "");
  inst_lib.AddInst("InputToWorking", sgp::inst_impl::Inst_InputToWorking<signalgp_t, inst_t>, "");
  inst_lib.AddInst("WorkingToOutput", sgp::inst_impl::Inst_WorkingToOutput<signalgp_t, inst_t>, "");
  inst_lib.AddInst("WorkingToGlobal", sgp::inst_impl::Inst_WorkingToGlobal<signalgp_t, inst_t>, "");
  inst_lib.AddInst("GlobalToWorking", sgp::inst_impl::Inst_GlobalToWorking<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Fork", sgp::inst_impl::Inst_Fork<signalgp_t, inst_t>, "");
  inst_lib.AddInst("Terminate", sgp::inst_impl::Inst_Terminate<signalgp_t, inst_t>, "");

  emp::Random random(2);

  signalgp_t hardware(random, inst_lib, event_lib);
  hardware.SetActiveThreadLimit(16);
  emp_assert(hardware.ValidateThreadState(), "Bad thread initial state.");

  program_t program;

  SECTION ("Inst_Inc") {
    std::cout << "-- Testing Inst_Inc --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    emp_assert(hardware.ValidateThreadState());

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0});
    program.PushInst(inst_lib, "Inc", {1, 0, 0});
    program.PushInst(inst_lib, "Inc", {1, 0, 0});
    program.PushInst(inst_lib, "Inc", {2, 0, 0});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // Inc(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Inc(1)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 1.0}}));
    hardware.SingleProcess(); // Inc(1)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}}));
    hardware.SingleProcess(); // Inc(2)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 1.0}}));
    hardware.SingleProcess(); // Inc(3)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 1.0}, {3, 1.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_Dec") {
    std::cout << "-- Testing Inst_Dec --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0});
    program.PushInst(inst_lib, "Dec", {0, 0, 0});
    program.PushInst(inst_lib, "Dec", {0, 0, 0});
    program.PushInst(inst_lib, "Dec", {1, 0, 0});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // Inc(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Dec(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // Dec(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, -1.0}}));
    hardware.SingleProcess(); // Dec(1)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, -1.0}, {1, -1.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_Not") {
    std::cout << "-- Testing Inst_Not --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0});
    program.PushInst(inst_lib, "Not", {0, 0, 0});
    program.PushInst(inst_lib, "Not", {1, 0, 0});
    program.PushInst(inst_lib, "Dec", {0, 0, 0});
    program.PushInst(inst_lib, "Not", {0, 0, 0});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // Inc(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Not(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // Not(1)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 1.0}}));
    hardware.SingleProcess(); // Dec(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, -1.0}, {1, 1.0}}));
    hardware.SingleProcess(); // Not(0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 1.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_Add") {
    std::cout << "-- Testing Inst_Add --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0}); // [0] = 1
    program.PushInst(inst_lib, "Add", {0, 0, 1}); // [1] = 2
    program.PushInst(inst_lib, "Add", {1, 1, 2}); // [2] = 4
    program.PushInst(inst_lib, "Add", {1, 2, 3}); // [3] = 6

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // Inc(0, 0, 0) // [0] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Add(0, 0, 1) // [1] = 2
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}}));
    hardware.SingleProcess(); // Add(1, 1, 2) // [2] = 4
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 4.0}}));
    hardware.SingleProcess(); // Add(1, 2, 3) // [3] = 6
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 4.0}, {3, 6.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_Sub") {
    std::cout << "-- Testing Inst_Sub --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0}); // [0] = 1
    program.PushInst(inst_lib, "Add", {0, 0, 1}); // [1] = 2
    program.PushInst(inst_lib, "Add", {1, 1, 2}); // [2] = 4
    program.PushInst(inst_lib, "Add", {1, 2, 3}); // [3] = 6

    program.PushInst(inst_lib, "Sub", {3, 4, 3}); // [4] = 0; [3] = 6
    program.PushInst(inst_lib, "Sub", {3, 2, 4}); // [4] = 2
    program.PushInst(inst_lib, "Sub", {2, 3, 4}); // [4] = -2

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    // hardware.SingleProcess(); // Inc(0)
    // REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));

    hardware.SingleProcess(); // Inc(0, 0, 0) // [0] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Add(0, 0, 1) // [1] = 2
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}}));
    hardware.SingleProcess(); // Add(1, 1, 2) // [2] = 4
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 4.0}}));
    hardware.SingleProcess(); // Add(1, 2, 3) // [3] = 6
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 4.0}, {3, 6.0}}));
    hardware.SingleProcess(); // Sub(3, 4, 3) // [4] = 0; [3] = 6
    REQUIRE((mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 4.0}, {3, 6.0}, {4, 0.0}})));
    hardware.SingleProcess(); // Sub(3, 2, 4) // [4] = 2
    REQUIRE((mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 4.0}, {3, 6.0}, {4, 2.0}})));
    hardware.SingleProcess(); // Sub(2, 3, 4) // [4] = -2
    REQUIRE((mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 4.0}, {3, 6.0}, {4, -2.0}})));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_Mult") {
    std::cout << "-- Testing Inst_Mult --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0}); // [0] = 1
    program.PushInst(inst_lib, "Add", {0, 0, 1}); // [1] = 2
    program.PushInst(inst_lib, "Dec", {-1, 0, 0}); // [-1] = -1
    program.PushInst(inst_lib, "Mult", {0, 0, 2}); // [2] = 1
    program.PushInst(inst_lib, "Mult", {-1, -1, 2}); // [2] = 1
    program.PushInst(inst_lib, "Mult", {1, 1, 2}); // [2] = 4
    program.PushInst(inst_lib, "Mult", {-1, 1, 2}); // [2] = -2

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // Inc(0, 0, 0) // [0] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Add(0, 0, 1) // [1] = 2
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}}));
    hardware.SingleProcess(); // Dec(-1, 0, 0) // [-1] = -1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{-1, -1.0}, {0, 1.0}, {1, 2.0}}));
    hardware.SingleProcess(); // Mult(0, 0, 2) // [2] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{-1, -1.0}, {0, 1.0}, {1, 2.0}, {2, 1.0}}));
    hardware.SingleProcess(); // Mult(-1, -1, 2) // [2] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{-1, -1.0}, {0, 1.0}, {1, 2.0}, {2, 1.0}}));
    hardware.SingleProcess(); // Mult(1, 1, 2) // [2] = 4
    REQUIRE(mem_state.working_mem == mem_buffer_t({{-1, -1.0}, {0, 1.0}, {1, 2.0}, {2, 4.0}}));
    hardware.SingleProcess(); // Mult(-1, 1, 2) // [2] = -2
    REQUIRE(mem_state.working_mem == mem_buffer_t({{-1, -1.0}, {0, 1.0}, {1, 2.0}, {2, -2.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_Div") {
    std::cout << "-- Testing Inst_Div --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0}); // [0] = 1
    program.PushInst(inst_lib, "Add", {0, 0, 1}); // [1] = 2
    program.PushInst(inst_lib, "Div", {2, 2, 2}); // [2] = 0   Do nothing.
    program.PushInst(inst_lib, "Div", {2, 2, 2}); // [2] = 0   Do nothing.
    program.PushInst(inst_lib, "Div", {0, 1, 3}); // [3] = 0.5 Do nothing.

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // Inc(0, 0, 0) // [0] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Add(0, 0, 1) // [1] = 2
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}}));
    hardware.SingleProcess(); // Div(2, 2, 2); // [2] = 0   Do nothing.
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 0.0}}));
    hardware.SingleProcess(); // Div(2, 2, 2); // [2] = 0   Do nothing.
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 0.0}}));
    hardware.SingleProcess(); // Div(0, 1, 3); // [3] = 0.5 Do nothing.
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 0.0}, {3, 0.5}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_Mod") {
    std::cout << "-- Testing Inst_Mod --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {0, 0, 0}); // [0] = 1
    program.PushInst(inst_lib, "Add", {0, 0, 1}); // [1] = 2
    program.PushInst(inst_lib, "Mod", {2, 2, 2}); // [2] = 0   Do nothing.
    program.PushInst(inst_lib, "Mod", {2, 2, 2}); // [2] = 0   Do nothing.
    program.PushInst(inst_lib, "Mod", {0, 1, 3}); // [3] = 1

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // Inc(0, 0, 0) // [0] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}}));
    hardware.SingleProcess(); // Add(0, 0, 1) // [1] = 2
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}}));
    hardware.SingleProcess(); // Mod(2, 2, 2); // [2] = 0   Do nothing.
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 0.0}}));
    hardware.SingleProcess(); // Mod(2, 2, 2); // [2] = 0   Do nothing.
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 0.0}}));
    hardware.SingleProcess(); // Mod(0, 1, 3); // [3] = 1
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 1.0}, {1, 2.0}, {2, 0.0}, {3, 1.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_SetMem") {
    std::cout << "-- Testing Inst_SetMem --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "SetMem", {0, 0, 0});
    program.PushInst(inst_lib, "SetMem", {1, 1, 0});
    program.PushInst(inst_lib, "SetMem", {2, 2, 0});
    program.PushInst(inst_lib, "SetMem", {3, -128, 0});
    program.PushInst(inst_lib, "SetMem", {4, 256, 0});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // SetMem(0, 0, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // SetMem(1, 1, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 1.0}}));
    hardware.SingleProcess(); // SetMem(2, 2, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 2.0}}));
    hardware.SingleProcess(); // SetMem(3, -128, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 2.0}, {3, -128.0}}));
    hardware.SingleProcess(); // SetMem(4, 256, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 2.0}, {3, -128.0}, {4, 256}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_TestEqu") {
    std::cout << "-- Testing Inst_TestEqu --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "SetMem", {0, 0, 0});
    program.PushInst(inst_lib, "SetMem", {1, 0, 0});
    program.PushInst(inst_lib, "SetMem", {2, 2, 0});
    program.PushInst(inst_lib, "TestEqu", {0, 1, 3});
    program.PushInst(inst_lib, "TestEqu", {0, 2, 3});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // SetMem(0, 0, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // SetMem{1, 0, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}}));
    hardware.SingleProcess(); // SetMem{2, 2, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}}));
    hardware.SingleProcess(); // TestEqu{0, 1, 3}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));
    hardware.SingleProcess(); // TestEqu{0, 2, 3}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_TestNEqu") {
    std::cout << "-- Testing Inst_TestNEqu --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "SetMem", {0, 0, 0});
    program.PushInst(inst_lib, "SetMem", {1, 0, 0});
    program.PushInst(inst_lib, "SetMem", {2, 2, 0});
    program.PushInst(inst_lib, "TestNEqu", {0, 1, 3});
    program.PushInst(inst_lib, "TestNEqu", {0, 2, 3});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // SetMem(0, 0, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // SetMem{1, 0, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}}));
    hardware.SingleProcess(); // SetMem{2, 2, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_TestLess") {
    std::cout << "-- Testing Inst_TestLess --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "SetMem", {0, 0, 0});
    program.PushInst(inst_lib, "SetMem", {1, 0, 0});
    program.PushInst(inst_lib, "SetMem", {2, 2, 0});
    program.PushInst(inst_lib, "TestLess", {0, 1, 3});
    program.PushInst(inst_lib, "TestLess", {0, 2, 3});
    program.PushInst(inst_lib, "TestLess", {2, 0, 3});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // SetMem(0, 0, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // SetMem{1, 0, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}}));
    hardware.SingleProcess(); // SetMem{2, 2, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_TestLessEqu") {
    std::cout << "-- Testing Inst_TestLessEqu --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "SetMem", {0, 0, 0});
    program.PushInst(inst_lib, "SetMem", {1, 0, 0});
    program.PushInst(inst_lib, "SetMem", {2, 2, 0});
    program.PushInst(inst_lib, "TestLessEqu", {0, 1, 3});
    program.PushInst(inst_lib, "TestLessEqu", {0, 2, 3});
    program.PushInst(inst_lib, "TestLessEqu", {2, 0, 3});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // SetMem(0, 0, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // SetMem{1, 0, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}}));
    hardware.SingleProcess(); // SetMem{2, 2, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_TestGreater") {
    std::cout << "-- Testing Inst_TestGreater --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "SetMem", {0, 0, 0});
    program.PushInst(inst_lib, "SetMem", {1, 0, 0});
    program.PushInst(inst_lib, "SetMem", {2, 2, 0});
    program.PushInst(inst_lib, "TestGreater", {0, 1, 3});
    program.PushInst(inst_lib, "TestGreater", {0, 2, 3});
    program.PushInst(inst_lib, "TestGreater", {2, 0, 3});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // SetMem(0, 0, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // SetMem{1, 0, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}}));
    hardware.SingleProcess(); // SetMem{2, 2, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_TestGreaterEqu") {
    std::cout << "-- Testing Inst_TestGreaterEqu --" << std::endl;
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    program.PushInst(inst_lib, "SetMem", {0, 0, 0});
    program.PushInst(inst_lib, "SetMem", {1, 0, 0});
    program.PushInst(inst_lib, "SetMem", {2, 2, 0});
    program.PushInst(inst_lib, "TestGreaterEqu", {0, 1, 3});
    program.PushInst(inst_lib, "TestGreaterEqu", {0, 2, 3});
    program.PushInst(inst_lib, "TestGreaterEqu", {2, 0, 3});

    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();

    emp_assert(hardware.GetPendingThreadIDs().size() == 1);

    // Assert call stack has only 1 call.
    auto & call_stack = hardware.GetThread(thread_id).GetExecState().GetCallStack();
    REQUIRE(call_stack.size() == 1);

    auto & call_state = call_stack.back();
    auto & mem_state = call_state.GetMemory();

    // Assert that memory is empty.
    REQUIRE(hardware.GetMemoryModel().GetGlobalBuffer().empty());
    REQUIRE(mem_state.working_mem.empty());
    REQUIRE(mem_state.input_mem.empty());
    REQUIRE(mem_state.output_mem.empty());

    hardware.SingleProcess(); // SetMem(0, 0, 0)
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}}));
    hardware.SingleProcess(); // SetMem{1, 0, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}}));
    hardware.SingleProcess(); // SetMem{2, 2, 0}
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 0.0}}));
    hardware.SingleProcess();
    REQUIRE(mem_state.working_mem == mem_buffer_t({{0, 0.0}, {1, 0.0}, {2, 2.0}, {3, 1.0}}));

    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
  }

  SECTION ("Inst_If") {
    std::cout << "-- Testing Inst_TestIf --" << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    // If(true), ____, ____, ____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "If", {1, 0, 0});
    program.PushInst(inst_lib, "Inc", {2, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);

    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 1.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , If(true), ____, ____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {1, 0, 0});
    program.PushInst(inst_lib, "Inc", {2, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 1.0}, {3, 1.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , ____ , ____, If(true)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {3, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 3.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // if(false) , ____ , ____, ____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "If",  {0, 0, 0});
    program.PushInst(inst_lib, "Inc", {2, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , if(false) , ____, ____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {0, 0, 0});
    program.PushInst(inst_lib, "Inc", {2, 0, 0});
    program.PushInst(inst_lib, "Nop", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , ____ , ____, If(false)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 3.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , ____ , DEF, _____, if(true) , _____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "Nop", {5, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {4, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {4, 1.0}, {5, 2.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, if(true) , _____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "Close", {5, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {4, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {4, 1.0}, {5, 2.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, if(true)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "Close", {5, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {3, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {5, 2.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, if(false) , _____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "Close", {5, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {0, 0, 0});
    program.PushInst(inst_lib, "Inc", {4, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {5, 1.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, if(false)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "Close", {5, 0, 0});
    program.PushInst(inst_lib, "Inc", {5, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    program.PushInst(inst_lib, "If",  {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {5, 1.0}}));
    hardware.SingleProcess(); // IP off edge of program
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////
  }

  SECTION ("Inst_While") {
    std::cout << "-- Testing Inst_While --" << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    // While(true), ____, ____, ____  => Infinite loop.
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "While", {1, 0, 0});
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "Inc",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc",   {4, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 1.0}, {3, 1.0}, {4, 1.0}}));
    for (size_t i = 0; i < 4; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 2.0}, {3, 2.0}, {4, 2.0}}));
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , While(true), ____, ____   => Infinite loop.
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "While", {1, 0, 0});
    program.PushInst(inst_lib, "Inc",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc",   {4, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 1.0}, {3, 1.0}, {4, 1.0}}));
    for (size_t i = 0; i < 3; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 1.0}, {3, 2.0}, {4, 2.0}}));
    for (size_t i = 0; i < 32; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetActiveThreadIDs().size());
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , ____ , ____, While(true)   => Infinite loop.
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc",   {3, 0, 0});
    program.PushInst(inst_lib, "While", {3, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 3.0}}));

    for (size_t i = 0; i < 32; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetActiveThreadIDs().size());
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 3.0}}));
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // While(false) , ____ , ____, ____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "While", {0, 0, 0});
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "Inc",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc",   {4, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , While(false) , ____, ____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "While", {0, 0, 0});
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , ____ , ____, While(false)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "Inc",   {2, 0, 0});
    program.PushInst(inst_lib, "While", {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{0, 0.0}, {1, 1.0}, {2, 3.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ , ____ , DEF, _____, While(true) , _____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc",   {5, 0, 0});
    program.PushInst(inst_lib, "Nop",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",   {5, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc",   {3, 0, 0});
    program.PushInst(inst_lib, "While", {3, 0, 0});
    program.PushInst(inst_lib, "Inc",   {4, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess(); // Inc
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // Inc
    hardware.SingleProcess(); // Inc
    hardware.SingleProcess(); // Nop
    hardware.SingleProcess(); // Inc
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {4, 1.0}, {5, 2.0}}));
    for (size_t i = 0; i < 5; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {4, 2.0}, {5, 4.0}}));
    for (size_t i = 0; i < 32; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetActiveThreadIDs().size());
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, While(true) , _____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc",     {5, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc",     {4, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess(); // Inc
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // Inc(4)
    hardware.SingleProcess(); // Inc(5)
    hardware.SingleProcess(); // Close
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {4, 1.0}, {5, 1.0}}));
    for (size_t i = 0; i < 4; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {4, 2.0}, {5, 2.0}}));
    for (size_t i = 0; i < 32; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetActiveThreadIDs().size());
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, While(true)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc",     {5, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {3, 0, 0});

    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // Inc(5)
    hardware.SingleProcess(); // Close
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {5, 1.0}}));
    for (size_t i = 0; i < 3; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {5, 2.0}}));
    for (size_t i = 0; i < 32; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetActiveThreadIDs().size());
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, While(false) , _____
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc",     {5, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {0, 0, 0});
    program.PushInst(inst_lib, "Inc",     {4, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // Inc(6)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {6, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // ____ ,CLOSE, ____ , DEF, _____, While(false)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Inc",     {5, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // Inc(6)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {6, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // CLOSE, ____ , DEF, _____, While(false)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // Inc(6)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {6, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // if(false), INC, CLOSE, INC, CLOSE, ____ , DEF, _____, While(True)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "If",      {0, 0, 0});
    program.PushInst(inst_lib, "Inc",     {7, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushInst(inst_lib, "Close",   {0, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {1, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // If(false)
    hardware.SingleProcess(); // Inc(6)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {6, 1.0}}));
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // if(false), INC, CLOSE, INC, CLOSE, ____ , DEF, _____, While(false)
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "If",      {0, 0, 0});
    program.PushInst(inst_lib, "Inc",     {7, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushInst(inst_lib, "Close",   {0, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {0, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(false)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}}));
    ////////////////////////////////////////////////////////////////////////////
  }

  SECTION ("Inst_Countdown") {
    std::cout << "-- Testing Inst_Countdown --" << std::endl;
    // Countdown instruction is pretty much the same as the While.
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "Countdown",   {3, 0, 0});
    program.PushInst(inst_lib, "Inc", {3, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // Inc(6)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}}));
    for (size_t i = 0; i < 10; ++i) { hardware.SingleProcess(); }
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}}));
    REQUIRE(hardware.GetActiveThreadIDs().size());
  }

  SECTION ("Inst_Break") {
    std::cout << "-- Testing Inst_Break --" << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "If",      {1, 0, 0});
    program.PushInst(inst_lib, "Break",     {0, 0, 0});
    program.PushInst(inst_lib, "Inc",     {7, 0, 0});
    program.PushInst(inst_lib, "Close",   {5, 0, 0});
    program.PushInst(inst_lib, "Inc",     {6, 0, 0});
    program.PushInst(inst_lib, "Close",   {0, 0, 0});
    program.PushInst(inst_lib, "Inc",     {8, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc",     {3, 0, 0});
    program.PushInst(inst_lib, "While",   {1, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // While(true)
    hardware.SingleProcess(); // If(true)
    hardware.SingleProcess(); // Break
    hardware.SingleProcess(); // Inc(8)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {8, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    program.Clear();
    hardware.Reset(); // Reset program & hardware.
    // Build program to test inc instruction.
    program.PushInst(inst_lib, "If",        {1, 0, 0});
    program.PushInst(inst_lib, "Break",     {0, 0, 0});
    program.PushInst(inst_lib, "Inc",       {7, 0, 0});
    program.PushInst(inst_lib, "Close",     {5, 0, 0});
    program.PushInst(inst_lib, "Inc",       {6, 0, 0});
    program.PushInst(inst_lib, "Close",     {0, 0, 0});
    program.PushInst(inst_lib, "Inc",       {8, 0, 0});
    program.PushInst(inst_lib, "ModuleDef", {0, 0, 0}, {tag_t()});
    program.PushInst(inst_lib, "Inc",       {3, 0, 0});
    program.PushInst(inst_lib, "If",        {1, 0, 0});
    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    hardware.SingleProcess(); // Inc(3)
    hardware.SingleProcess(); // If(true)
    hardware.SingleProcess(); // If(true)
    hardware.SingleProcess(); // Break
    hardware.SingleProcess(); // Inc(7)
    hardware.SingleProcess(); // Close
    hardware.SingleProcess(); // Inc(6)
    hardware.SingleProcess(); // Close
    hardware.SingleProcess(); // Inc(8)
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
            == mem_buffer_t({{0, 0.0}, {1, 1.0}, {3, 1.0}, {6, 1.0}, {7, 1.0}, {8, 1.0}}));
    hardware.SingleProcess();
    REQUIRE(hardware.GetActiveThreadIDs().size() == 0);
    ////////////////////////////////////////////////////////////////////////////
  }

  SECTION ("Inst_Call") {
    std::cout << "-- Testing Inst_Call --" << std::endl;
    ////////////////////////////////////////////////////////////////////////////
    program.Clear();
    hardware.Reset(); // Reset program & hardware.

    // Build program to test inc instruction.
    tag_t zeros, ones;
    ones.SetUInt(0, (uint16_t)-1);
    // SetUInt
    program.PushInst(inst_lib, "ModuleDef",  {0, 0, 0}, {zeros});
    program.PushInst(inst_lib,   "SetMem", {2, 2});
    program.PushInst(inst_lib,   "SetMem", {3, 3});
    program.PushInst(inst_lib,   "Call", {0, 0, 0}, {ones});

    program.PushInst(inst_lib, "ModuleDef",  {0, 0, 0}, {ones});
    program.PushInst(inst_lib,   "InputToWorking", {2, 1, 0});
    program.PushInst(inst_lib,   "InputToWorking", {3, 2, 0});
    program.PushInst(inst_lib,   "Inc", {1, 0, 0});
    program.PushInst(inst_lib,   "Inc", {2, 0, 0});
    program.PushInst(inst_lib,   "WorkingToOutput", {1, 4, 0});
    program.PushInst(inst_lib,   "WorkingToOutput", {2, 5, 0});

    // Load program on hardware.
    hardware.SetProgram(program);
    // Spawn a thread to run the program.
    auto spawned = hardware.SpawnThreadWithID(0);
    emp_assert(spawned);
    size_t thread_id = spawned.value();
    emp_assert(hardware.GetPendingThreadIDs().size() == 1);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(0, 0);
    hardware.GetThread(thread_id).GetExecState().GetTopCallState()
            .GetMemory().SetWorking(1, 1);

    for (size_t i = 0; i < 10; ++i) hardware.SingleProcess();
    REQUIRE(hardware.GetThread(thread_id).GetExecState().GetTopCallState().GetMemory().working_mem
        == mem_buffer_t({{4, 3.0}, {1, 1.0}, {0, 0.0}, {5, 4.0}, {2, 2.0}, {3, 3.0}}));
    ////////////////////////////////////////////////////////////////////////////
  }

  // SECTION ("Inst_Routine") {

  // }

  // SECTION ("Inst_Return") {

  // }

  // SECTION ("Inst_CopyMem") {

  // }

  // SECTION ("Inst_SwapMem") {

  // }

  // SECTION ("Inst_WorkingToGlobal") {

  // }

  // SECTION ("Inst_GlobalToWorking") {

  // }

  // SECTION ("Inst_Fork") {

  // }

  // SECTION ("Inst_Terminate") {

  // }

}

