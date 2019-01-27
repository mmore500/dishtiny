#include "tools/FunctionSet.h"
#include <functional>

int main()
{

    int t = 0;

    std::function<int&()> you;

    std::cout << you();    
    you = [&t]()-> int& {return t;};

    std::cout << you();
    t = 10;

    int & foo = you();
    foo++;

    std::cout << you();
}
