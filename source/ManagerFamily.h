#pragma once

#include <unordered_set>

class ManagerFamily {

private:
  size_t parent_pos;
  std::unordered_set<size_t> child_pos;

public:
  ManagerFamily() { ; }

  void Reset() {
    child_pos.clear();
  }

  bool IsParentPos(size_t pos) const { return pos == parent_pos; }
  bool HasChildPos(size_t pos) const {
    return child_pos.find(pos) != child_pos.end();
  }

  void SetParentPos(size_t pos) { parent_pos = pos; }
  void AddChildPos(size_t pos) { child_pos.insert(pos); }

};
