#pragma once

#include "../third-party/Empirical/include/emp/tools/string_utils.hpp"

class State {

  char state;

public:

  State() : state('_') { ; }

  State(const char & x) : state(x) { ; }

  operator char() const { return state; }

  std::string ToString() const { return emp::to_string(state); }

};
