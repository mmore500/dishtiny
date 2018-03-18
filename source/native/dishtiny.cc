// This is the main function for the NATIVE version of this project.

#include "../CustomConfig.h"
#include "../DishtinyConfig.h"
#include "../GridSpec.h"
#include "../TinyWorld.h"

int main()
{

  DishtinyConfig dconfig;
  GridSpec spec = GridSpec(dconfig);
  CustomConfig cconfig = CustomConfig(dconfig, spec);

  TinyWorld simulation(1000, dconfig, cconfig);
  simulation.Run();
  return 0;
}
