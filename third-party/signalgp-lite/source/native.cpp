//  This file is part of SignalGP Lite
//  Copyright (C) Matthew Andres Moreno, 2020.
//  Released under MIT license; see LICENSE

#include <iostream>

#include "base/vector.hpp"
#include "config/command_line.hpp"

// This is the main function for the NATIVE version of SignalGP Lite.

int main(int argc, char* argv[])
{
  emp::vector<std::string> args = emp::cl::args_to_strings(argc, argv);

  std::cout << "Hello, world!" << '\n';

  return 0;
}
