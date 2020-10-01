#pragma once
#ifndef DISHTINY_MANAGERAPOPTOSIS_HPP_INCLUDE
#define DISHTINY_MANAGERAPOPTOSIS_HPP_INCLUDE

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

#endif // #ifndef DISHTINY_MANAGERAPOPTOSIS_HPP_INCLUDE
