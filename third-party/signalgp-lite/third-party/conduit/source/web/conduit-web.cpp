//  This file is part of Pipe Profile
//  Copyright (C) Matthew Andres Moreno, 2020.
//  Released under MIT license; see LICENSE

#include <iostream>

#include "../../third-party/Empirical/include/emp/web/web.hpp"

namespace UI = emp::web;

UI::Document doc("emp_base");

int main()
{
  doc << "<h1>Hello, browser!</h1>";
  std::cout << "Hello, console!" << '\n';
}
