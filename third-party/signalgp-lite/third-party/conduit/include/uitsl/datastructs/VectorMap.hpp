#pragma once
#ifndef UITSL_DATASTRUCTS_VECTORMAP_HPP_INCLUDE
#define UITSL_DATASTRUCTS_VECTORMAP_HPP_INCLUDE

#include <functional>
#include <memory>
#include <utility>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

namespace uitsl {

namespace internal {

template<typename Key, typename T>
class VectorMapIterator
: public emp::vector<std::reference_wrapper<
  std::pair<const Key, T>
>>::iterator {

public:
  using value_type = std::pair<const Key, T>;

private:
  using parent_t = typename emp::vector<std::reference_wrapper<
    value_type
  >>::iterator;

public:

  VectorMapIterator() = default;
  VectorMapIterator(const parent_t& val) : parent_t(val) { ; }

  value_type& operator*() { return parent_t::operator*().get(); }

  const value_type& operator*() const { return parent_t::operator*().get(); }

  value_type* operator->() { return &operator*(); }

  const value_type* operator->() const { return &operator*(); }


};

template<typename Key, typename T>
class VectorMapConstIterator
: public emp::vector<std::reference_wrapper<
  const std::pair<const Key, T>
>>::const_iterator {

public:
  using value_type = std::pair<const Key, T>;

private:
  using parent_t = typename emp::vector<std::reference_wrapper<
    const value_type
  >>::const_iterator;

public:

  VectorMapConstIterator() = default;
  VectorMapConstIterator(const parent_t& val) : parent_t(val) { ; }

  const value_type& operator*() const { return parent_t::operator*().get(); }

  const value_type* operator->() const { return &operator*(); }

};

} // namespace internal

template<typename Key, typename T>
class VectorMap {

public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
private:
  using container_t = emp::vector< emp::optional< value_type > >;
public:
  using difference_type = typename container_t::difference_type;
  using size_type = typename container_t::size_type;
  using reference = value_type&;
  using const_reference = const value_type&;
  using allocator_type = typename container_t::allocator_type;
  using pointer = typename std::allocator_traits<allocator_type>::pointer;
  using const_pointer = typename std::allocator_traits<
    allocator_type
  >::const_pointer;
  using iterator = internal::VectorMapIterator<Key, T>;
  using const_iterator = internal::VectorMapConstIterator<Key, T>;

private:

  container_t data;

  size_t num_items{};

  emp::optional<emp::vector<std::reference_wrapper<
    value_type
  >>> view;
  mutable emp::optional<emp::vector<std::reference_wrapper<
    const value_type
  >>> const_view;

  void PrepareView() {
    if (!view.has_value()) {
      view.emplace();
      for (auto& item : data) {
        if ( item.has_value() ) view->emplace_back( *item );
      }
    }
  }

  void PrepareConstView() const {
    if (!const_view.has_value()) {
      const_view.emplace();
      for (const auto& item : data) {
        if ( item.has_value() ) const_view->emplace_back( *item );
      }
    }
  }

  void InvalidateViews() { view.reset(); const_view.reset(); }

public:

  T& operator[](const Key i) {
    emp_assert( i >= 0 );

    // if needed, make space
    if (static_cast<size_t>(i) >= data.size()) data.resize(i + 1);

    // if needed, set up new value
    if ( !data[i].has_value() ) {
      data[i].emplace(
        std::piecewise_construct,
        std::forward_as_tuple(i),
        std::forward_as_tuple()
      );
      ++num_items;
    }

    // clear cached views
    InvalidateViews();

    return at(i);
  }

  T& at(const Key i) { return data[i]->second; }

  const T& at(const Key i) const { return data[i]->second; }

  iterator begin() {
    PrepareView();
    return iterator( std::begin( *view ) );
  }

  iterator end() {
    PrepareView();
    return iterator( std::end( *view ) );
  }

  const_iterator begin() const {
    PrepareConstView();
    return const_iterator( std::cbegin( *const_view ) );
  }

  const_iterator end() const {
    PrepareConstView();
    return const_iterator( std::cend( *const_view ) );
  }

  const_iterator cbegin() const {
    PrepareConstView();
    return const_iterator( std::cbegin( *const_view ) );
  }

  const_iterator cend() const {
    PrepareConstView();
    return const_iterator( std::cend( *const_view ) );
  }

  size_t size() const { return num_items; }

  size_t empty() const { return !size(); }

  void erase(const Key i) { data[i].reset(); --num_items; InvalidateViews(); }

  void clear() { data.clear(); num_items = 0; InvalidateViews(); }

  size_t count(const Key i) const {
    return i < data.size() && data[i].has_value();
  }

  bool contains(const Key i) const { return count(i); }

};

} // namespace uitsl

#endif // #ifndef UITSL_DATASTRUCTS_VECTORMAP_HPP_INCLUDE
