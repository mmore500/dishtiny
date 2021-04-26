//  This file is part of Header-Only PCA
//  Copyright (C) Santiago Rodriguez Papa, 2020.
//  Released under MIT license; see LICENSE

#include <iostream>

#include "emp/web/web.hpp"

#include "header-only-pca/example.hpp"

namespace UI = emp::web;

UI::Document doc("emp_base");

int main()
{
  doc << "<h1>Hello, browser!</h1>";
  std::cout << "Hello, console!" << std::endl;
  return example();
}
