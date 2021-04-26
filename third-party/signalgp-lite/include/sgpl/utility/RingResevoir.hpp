#pragma once
#ifndef SGPL_UTILITY_RINGRESEVOIR_HPP_INCLUDE
#define SGPL_UTILITY_RINGRESEVOIR_HPP_INCLUDE

#include <numeric>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/datastructs/RingBuffer.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/safe_compare.hpp"
#include "../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"

namespace sgpl {

template<typename T, size_t N>
class RingResevoir {

  emp::array<T, N> buffer{};

  size_t tail_index{};
  size_t num_items{};

  inline void AdvanceTail() { ++tail_index %= GetCapacity(); }

  inline size_t GetBufferIndex( const size_t pos ) const {
    emp_assert( pos < GetSize() );
    emp_assert( tail_index < GetCapacity() );
    return ( tail_index + pos ) % GetCapacity();
  }

public:

  inline size_t GetSize() const { return num_items; }

  constexpr size_t GetCapacity() const { return N; }

  inline size_t GetAvailableCapacity() const {
    return GetCapacity() - GetSize();
  }

  inline bool IsEmpty() const { return GetSize() == 0; }

  inline bool IsFull() const { return GetSize() == GetCapacity(); }

  inline T& Get( const size_t pos ) { return buffer[ GetBufferIndex( pos ) ]; }

  inline const T& Get( const size_t pos ) const {
    return buffer[ GetBufferIndex( pos ) ];
  }

  inline T& GetTail() {
    emp_assert( !IsEmpty() );
    return buffer[tail_index];
  }

  inline T& GetHead() {
    emp_assert( !IsEmpty() );
    return Get( GetSize() - 1 );
  }

  T& Acquire() {
    emp_assert( !IsFull() );
    ++num_items;
    return GetHead();
  }

  inline void ReleaseHead() {
    emp_assert( !IsEmpty() );
    --num_items;
  }

  inline void ReleaseTail() {
    emp_assert( !IsEmpty() );
    AdvanceTail();
    --num_items;
  }

  inline bool IsTail(const size_t pos) const { return pos == 0; }

  inline bool IsHead(const size_t pos) const { return pos == GetSize(); }

  void Release(const size_t pos) {
    if ( IsTail( pos ) ) ReleaseTail();
    else {
      if ( !IsHead( pos ) ) std::swap( this->Get(pos), this->GetHead() );
      ReleaseHead();
    }
  }

  void Fill( const T& t ) { buffer.fill( t ); }

  void Reset() {
    tail_index = {};
    num_items = {};
  }

  emp::array<T, N>& GetBuffer() { return buffer; }

};

} // namespace sgpl

#endif // #ifndef SGPL_UTILITY_RINGRESEVOIR_HPP_INCLUDE
