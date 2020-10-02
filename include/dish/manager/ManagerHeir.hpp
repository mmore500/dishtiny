#pragma once
#ifndef DISH_MANAGERHEIR_HPP_INCLUDE
#define DISH_MANAGERHEIR_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/source/base/vector.h"

namespace dish {

class ManagerHeir {

private:
  emp::vector<size_t> is_heir;

public:
  ManagerHeir() : is_heir(Cardi::Dir::NumDirs) { Reset(); }

  void Reset() {
    std::fill(is_heir.begin(), is_heir.end(), 0);
  }

  bool IsHeir(const size_t dir) const { return is_heir[dir]; }

  void DecayHeirs() {
    for (auto & val : is_heir) if (val) --val;
  }

  void SetHeir(const size_t dir, const size_t set) { is_heir[dir] = set; }

  size_t HeirCount() const {
    return std::count_if(
      is_heir.cbegin(),
      is_heir.cend(),
      [](size_t i){ return i; }
    );
  }

};

} // namespace dish

#endif // #ifndef DISH_MANAGERHEIR_HPP_INCLUDE
