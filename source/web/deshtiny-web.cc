#include "tools/Random.h"
#include "web/web.h"

#include "../Hello.h"

namespace UI = emp::web;

struct Interface {
  Hello h;
  emp::Random random;

  UI::Document doc;
  UI::Canvas canvas;

  Interface()
    : h("Matthew", random)
    , doc("emp_base")
    , canvas(400,400)
  {
    doc << canvas << h.say();
  }

};

Interface interface;

int main()
{
  return 0;
}
