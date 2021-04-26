//  This file is part of Header-Only PCA
//  Copyright (C) Santiago Rodriguez Papa, 2020.
//  Released under MIT license; see LICENSE

#include <iostream>

#include "emp/base/vector.hpp"
#include "emp/config/command_line.hpp"

#include "header-only-pca/example.hpp"

// This is the main function for the NATIVE version of Header-Only PCA.

int main(int argc, char* argv[])
{
  emp::vector<std::string> args = emp::cl::args_to_strings(argc, argv);

  std::cout << "Hello, world!" << std::endl;

  return example();
}
