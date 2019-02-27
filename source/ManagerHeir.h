#pragma once

#include <algorithm>

#include "base/vector.h"

class ManagerHeir {

private:
  emp::vector<bool> is_heir;

public:
  ManagerHeir() : is_heir(Cardi::Dir::NumDirs) { Reset(); }

  void Reset() {
    std::fill(is_heir.begin(), is_heir.end(), false);
  }

  bool IsHeir(const size_t dir) const { return is_heir[dir]; }

  void SetHeir(const size_t dir, const bool set) { is_heir[dir] = set; }

  size_t HeirCount() const {
    return std::count(is_heir.cbegin(), is_heir.cend(), true);
  }

};
