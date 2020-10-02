#pragma once
#ifndef DISH_MANAGER_MANAGERAPOPTOSIS_HPP_INCLUDE
#define DISH_MANAGER_MANAGERAPOPTOSIS_HPP_INCLUDE

namespace dish {

class ManagerApoptosis {

private:

  size_t state;

public:

  static const size_t complete = 2;
  static const size_t partial = 1;
  static const size_t unmarked = 0;

  ManagerApoptosis() : state(unmarked) { Reset(); }

  void Reset() { state = unmarked; }

  bool IsMarked() const { return state; }

  bool IsMarkedPartial() const { return state == partial; }

  bool IsMarkedComplete() const { return state == complete; }

  void MarkComplete() { state = complete; }

  void MarkPartial() { state = partial; }

  int GetState() { return state; }

};

} // namespace dish

#endif // #ifndef DISH_MANAGER_MANAGERAPOPTOSIS_HPP_INCLUDE
