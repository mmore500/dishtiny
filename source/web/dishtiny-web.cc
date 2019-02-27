#include <string>

#include "../Config.h"
#include "../DishWorld.h"
#include "../WebInterface.h"

#include "../Config.cpp"
#include "../DishWorld.cpp"
#include "../FrameHardware.cpp"
#include "../FrameCell.cpp"
#include "../Manager.cpp"

WebInterface i;

int main(int argc, char* argv[])
{

  i.Redraw();

  return 0;
}
