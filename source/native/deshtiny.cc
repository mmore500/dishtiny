// This is the main function for the NATIVE version of this project.

#include "../Simulation.h"

int main()
{
  Simulation simulation = Simulation(100000, 0);
  while(simulation.Step()){}
  return 0;
}
