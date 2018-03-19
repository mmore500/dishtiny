// This is the main function for the NATIVE version of this project.

#include "tools/Random.h"

#include "../CustomConfig.h"
#include "../DishtinyConfig.h"
#include "../GridSpec.h"
#include "../TinyWorld.h"

int main()
{

  DishtinyConfig dconfig;
  GridSpec spec(dconfig);
  CustomConfig cconfig(dconfig, spec);
  emp::Random rand(dconfig.SEED());

  TinyWorld world(100000, rand, dconfig, cconfig);
  world.Run();
  return 0;
}
