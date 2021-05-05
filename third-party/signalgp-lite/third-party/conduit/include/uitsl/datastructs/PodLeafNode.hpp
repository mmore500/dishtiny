#pragma once
#ifndef UITSL_DATASTRUCTS_PODLEAFNODE_HPP_INCLUDE
#define UITSL_DATASTRUCTS_PODLEAFNODE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/include/emp/base/array.hpp"

namespace uitsl {

template<typename T, size_t N=1>
class PodLeafNode {

  // TODO swtich back to emp::array after https://github.com/devosoft/Empirical/issues/408 is resolved
  std::array<T, N> data{};

public:

  PodLeafNode() = default;

  PodLeafNode(const T& other) {
    emp_assert( N == 1 );
    data[0] = other;
  }

  static constexpr bool IsLeaf() noexcept { return true; }

  static constexpr size_t GetSize() noexcept { return N; }

  /*
   * Access value by index at compile time.
   */
  template<size_t Query>
  constexpr T& GetByIndex() noexcept { return data.at(Query); }

  /*
   * Access value by index at compile time.
   */
  template<size_t Query>
  constexpr T& Get() noexcept{ return data.at(Query); }

  /*
   * Access value by index at compile time.
   */
  template<size_t Query>
  constexpr const T& Get() const noexcept { return data.at(Query); }

  /*
   * Access value by index at runtime.
   */
  constexpr T& Get(const size_t query=0) noexcept { return data.at(query); }

  /*
   * Access value by index at runtime.
   */
  constexpr const T& Get(const size_t query=0) const noexcept {
    return data.at(query);
  }

  /*
   * Access value.
   * Template oncluded for compatibility with other components
   */
  template<typename Query=T>
  constexpr T& Get() noexcept {
    static_assert( std::is_same<Query, T>::value );
    return data.at(0);
  }

  /*
   * Access value.
   * Template oncluded for compatibility with other components
   */
  template<typename Query=T>
  constexpr const T& Get() const noexcept {
    static_assert( std::is_same<Query, T>::value );
    return data.at(0);
  }


  /*
   * Does leaf array contain type?
   */
  template<typename Query>
  static constexpr bool HasType() noexcept { return std::is_same<T, Query>(); }


  /*
   * Equality operator overload.
   */
  bool operator==(const PodLeafNode& other) const noexcept {
    return data == other.data;
  }

  /*
   * Equality operator overload.
   */
  bool operator<(const PodLeafNode& other) const noexcept {
    return std::lexicographical_compare(
      std::begin( data ),
      std::end( data ),
      std::begin( other.data ),
      std::end( other.data )
    );
  }

  /*
   * Set data to value-initialized state.
   */
  void Reset() noexcept { data.fill( T{} ); }

  operator T&() noexcept { emp_assert( N == 1 ); return Get(); }

  operator const T&() const noexcept { emp_assert( N == 1 ); return Get(); }

  PodLeafNode& operator=(const T& other) noexcept {
    emp_assert( N == 1 );
    data[0] = other;
    return *this;
  }

  auto begin() noexcept { return std::begin( data ); }

  auto end()noexcept  { return std::end( data ); }

  auto begin() const noexcept { return std::begin( data ); }

  auto end() const noexcept { return std::end( data ); }

  // TODO swtich back to emp::array after https://github.com/devosoft/Empirical/issues/408 is resolved
  const std::array<T, N>& GetBuffer() const noexcept { return data; }

};

} // namespace uitsl

#endif // #ifndef UITSL_DATASTRUCTS_PODLEAFNODE_HPP_INCLUDE
