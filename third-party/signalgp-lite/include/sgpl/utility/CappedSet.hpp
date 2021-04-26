#pragma once
#ifndef SGPL_UTILITY_CAPPEDSET_HPP_INCLUDE
#define SGPL_UTILITY_CAPPEDSET_HPP_INCLUDE

#include <utility>

#include "../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

namespace sgpl {

template<typename T, size_t N>
class CappedSet {

  emp::array<T, N> buffer;

  size_t size_{};

public:

  T& operator[]( const size_t pos ) { return buffer[pos]; }

  const T& operator[]( const size_t pos ) const { return buffer[pos]; }

  bool try_push_back( const T& value ) {
    if ( !full() ) {
      push_back( value );
      return true;
    } else return false;
  }

  bool try_push_back( T&& value ) {
    if ( !full() ) {
      push_back( std::move( value ) );
      return true;
    } else return false;
  }

  void push_back( const T& value ) { buffer[size()] = value; ++size_; }

  void push_back( T&& value ) { buffer[size()] = std::move( value ); ++size_; }

  void push_back() { ++size_; }

  void clear() { size_ = 0; }

  size_t size() const { return size_; }

  constexpr size_t max_size() const { return N; }

  bool empty() const { return size(); }

  bool full() const { return size() == N; }

  void pop_back() { emp_assert( size() ); --size_; }

  T& back() { emp_assert( size() ); return operator[](size() - 1); };

  const T& back() const {
   emp_assert( size() );
   return operator[](size() - 1);
 };

  T& front() { return buffer.front(); };

  const T& front()  const { return buffer.front(); };

  void erase( const size_t pos ) {
    operator[](pos) = std::move( back() );
    pop_back();
  }

  T* begin() { return &front(); }

  const T* begin() const { return &front(); }

  T* end() { return begin() + size(); }

  const T* end() const { return begin() + size(); }

};

} // namespace sgpl

#endif // #ifndef SGPL_UTILITY_CAPPEDSET_HPP_INCLUDE
