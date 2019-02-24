#include <string>

#include "web/web.h"
#include "web/Animate.h"
#include "base/vector.h"
#include "tools/Random.h"

#include "../Config.h"
#include "../DishWorld.h"
#include "../WebInterface.h"

#include "../Config.cpp"
#include "../DishWorld.cpp"
#include "../FrameHardware.cpp"
#include "../FrameCell.cpp"
#include "../Manager.cpp"

namespace UI = emp::web;

WebInterface i;

int main(int argc, char* argv[])
{

  i.Redraw();

  return 0;
}
