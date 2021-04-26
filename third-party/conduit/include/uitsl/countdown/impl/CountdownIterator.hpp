#pragma once
#ifndef UITSL_COUNTDOWN_IMPL_COUNTDOWNITERATOR_HPP_INCLUDE
#define UITSL_COUNTDOWN_IMPL_COUNTDOWNITERATOR_HPP_INCLUDE

namespace uitsl {

template<typename CountdownType>
class CountdownIterator {

  // non-owning pointer
  CountdownType* impl;

public:

  CountdownIterator() = default;

  explicit CountdownIterator(CountdownType& countdown) : impl( &countdown ) {}

  bool operator!=(const CountdownIterator other) {
    return ! impl->IsComplete();
  }

  CountdownIterator& operator++() { ++*impl; return *this; }

  using elapsed_t = typename CountdownType::elapsed_t;
  elapsed_t operator*() const { return impl->GetElapsed(); }

};

} // namespace uitsl

#endif // #ifndef UITSL_COUNTDOWN_IMPL_COUNTDOWNITERATOR_HPP_INCLUDE
