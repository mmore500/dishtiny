// This is the main function for the NATIVE version of this project.

#include <iostream>
#include "tools/Random.h"
#include "../Hello.h"

int main()
{
  emp::Random random;
  Hello h("Matthew", random);
  std::cout << h.say() << std::endl;
  return 0;
}
