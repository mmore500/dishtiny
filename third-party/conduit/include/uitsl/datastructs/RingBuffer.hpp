#pragma once
#ifndef UITSL_DATASTRUCTS_RINGBUFFER_HPP_INCLUDE
#define UITSL_DATASTRUCTS_RINGBUFFER_HPP_INCLUDE

#include <algorithm>
#include <stddef.h>
#include <utility>

#include "../../../third-party/Empirical/include/emp/base/array.hpp"

#include "../nonce/CircularIndex.hpp"

namespace uitsl {

template<typename T, size_t N>
class RingBuffer {

  emp::array<T, N> buffer{};
  uitsl::CircularIndex<N> tail{};
  size_t num_items{};

public:

  size_t GetSize() const { return num_items; }

  constexpr size_t GetCapacity() const { return N; }

  size_t GetAvailableCapacity() const { return GetSize() - GetCapacity(); }

  bool IsEmpty() const { return GetSize() == 0; }

  bool IsFull() const { return GetSize() == N; }

  void DoPushHead() { emp_assert( !IsFull() ); ++num_items; }

  bool PushHead() {
    if ( IsFull() ) return false;
    else {
      DoPushHead();
      return true;
    }
  }

  bool PushHead(const T& t) {
    if ( IsFull() ) return false;
    else {
      Get(num_items) = t;
      DoPushHead();
      return true;
    }
  }

  template<typename P>
  bool PushHead(P&& p) {
    if ( IsFull() ) return false;
    else {
      Get(num_items) = std::forward<P>( p );
      DoPushHead();
      return true;
    }
  }

  void DoPopHead(const size_t num_requested=1) {
    emp_assert( num_requested <= GetSize() );
    num_items -= num_requested;
  }

  size_t PopHead(const size_t num_requested=1) {
    const size_t num_popped = std::min(num_requested, GetSize());
    DoPopHead( num_popped );
    return num_popped;
  }

  void DoPopTail(const size_t num_requested=1) {
    emp_assert( num_requested <= GetSize() );
    tail += num_requested;
    num_items -= num_requested;
  }

  size_t PopTail(const size_t num_requested=1) {
    const size_t num_popped = std::min(num_requested, GetSize());
    DoPopTail( num_popped );
    return num_popped;
  }

  void Fill(const T& t=T{}) { while( PushHead(t) ); }

  const T& Get(const size_t i) const { return buffer[tail + i]; }

  T& Get(const size_t i) { return buffer[tail + i]; }

  T& GetHead() { return Get( GetSize() - 1 ); }

  const T& GetHead() const { return Get( GetSize() - 1 ); }

  T& GetTail() { return Get(0); }

  T GetPopTail() { const T res = GetTail(); DoPopTail(); return res; }

  const T& GetTail() const { return Get(0); }

  void Clear() { num_items = 0; }

  auto& GetBuffer() { return buffer; }

  bool IsHead(const size_t pos) const { return pos == GetSize() - 1; }

  bool IsTail(const size_t pos) const { return pos == 0; }

};

} // namespace uitsl

#endif // #ifndef UITSL_DATASTRUCTS_RINGBUFFER_HPP_INCLUDE
