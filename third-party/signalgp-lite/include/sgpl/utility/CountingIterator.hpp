#pragma once
#ifndef SGPL_UTILITY_COUNTINGITERATOR_HPP_INCLUDE
#define SGPL_UTILITY_COUNTINGITERATOR_HPP_INCLUDE

namespace sgpl {

template<typename T=size_t>
class CountingIterator {

  size_t idx{};

public:
  using value_type = T;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = int;

  CountingIterator() = default;
  CountingIterator(const T& t) : idx( t ) {}

  value_type operator*() const { return idx; }

  CountingIterator operator++(int) {
    const auto res = *this;
    ++idx;
    return res;
  }

  CountingIterator& operator++() {
    ++idx;
    return *this;
  }

  CountingIterator operator+(const size_t rhs) {
    CountingIterator res{ *this };
    res.idx += rhs;
    return res;
  }

  bool operator==(const CountingIterator& other) const {
    return operator*() == other.operator*();
  }

  bool operator!=(const CountingIterator& other) const {
    return operator*() != other.operator*();
  }

};

} // namespace sgpl

#endif // #ifndef SGPL_UTILITY_COUNTINGITERATOR_HPP_INCLUDE
