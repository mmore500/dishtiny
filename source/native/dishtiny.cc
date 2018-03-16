// This is the main function for the NATIVE version of this project.

#include "../CustomConfig.h"
#include "../DishtinyConfig.h"
#include "../GridSpec.h"
#include "../Simulation.h"


int main()
{

  DishtinyConfig dconfig;
  GridSpec spec = GridSpec(dconfig);
  CustomConfig cconfig = CustomConfig(dconfig, spec);

  Simulation* simulation = new Simulation(1000, 0, dconfig, cconfig);
  simulation->Run();
  return 0;
}
