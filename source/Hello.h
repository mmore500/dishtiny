#include <sstream>

#include "tools/Random.h"

class Hello {
public:

  const std::string name;         ///< Who to greet?
  emp::Random& random;             ///< Random number generator

public:
  Hello(const std::string _n, emp::Random& _r)
    : name(_n)
    , random(_r) {;}

  std::string say() const {
    std::stringstream ss;
    ss << "hello " << name << " " << random.GetInt(8);
    return ss.str();
  }
};
