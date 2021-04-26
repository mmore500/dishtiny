//  This file is part of SignalGP Lite
//  Copyright (C) Matthew Andres Moreno, 2020.
//  Released under MIT license; see LICENSE

#include <iostream>

#include "web/web.hpp"

namespace UI = emp::web;

UI::Document doc("emp_base");

int main()
{
  doc << "<h1>Hello, browser!</h1>";
  std::cout << "Hello, console!" << '\n';
  return 0;
}
