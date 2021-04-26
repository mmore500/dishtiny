#pragma once
#ifndef UITSL_CONTAINERS_SAFE_VECTOR_HPP_INCLUDE
#define UITSL_CONTAINERS_SAFE_VECTOR_HPP_INCLUDE

#include <shared_mutex>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

namespace uitsl {
namespace safe {

template<class T, class Allocator = std::allocator<T>>
class vector {

  using impl_t = emp::vector<T, Allocator>;

  mutable std::shared_mutex mutex;

  impl_t impl;

public:
  // types
  using value_type = typename impl_t::value_type;
  using allocator_type = typename impl_t::allocator_type;
  using pointer = typename impl_t::pointer;
  using const_pointer = typename impl_t::const_pointer;
  using reference = typename impl_t::reference;
  using const_reference = typename impl_t::const_reference;
  using size_type = typename impl_t::size_type;
  using difference_type = typename impl_t::difference_type;
  using iterator = typename impl_t::iterator;
  using const_iterator = typename impl_t::const_iterator;
  using reverse_iterator = typename impl_t::reverse_iterator;
  using const_reverse_iterator = typename impl_t::const_reverse_iterator;

  template<typename... Args>
  explicit vector(Args&&... args)
  : impl(std::forward<Args>(args)...)
  { ; }


  vector& operator=(const vector& x) {
    const std::unique_lock lock{ mutex };
    impl.operator=(x);
    return *this;
  }

  vector& operator=(vector&& x) noexcept(
    std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value
    || std::allocator_traits<Allocator>::is_always_equal::value
  ) {
    const std::unique_lock lock{ mutex };
    impl.operator=(std::move(x));
    return *this;
  }

  vector& operator=(std::initializer_list<T> arg) {
    const std::unique_lock lock{ mutex };
    impl.operator=(arg);
    return *this;
  }

  template<class InputIt>
  void assign(InputIt first, InputIt last) {
    const std::unique_lock lock{ mutex };
    impl.assign(first, last);
  }

  void assign(size_type n, const T& u) {
    const std::unique_lock lock{ mutex };
    impl.assign(n, u);
  }

  void assign(std::initializer_list<T> arg) {
    const std::unique_lock lock{ mutex };
    impl.assign(arg);
  }

  constexpr allocator_type get_allocator() const noexcept {
    return impl.get_allocator();
  }

  // iterators
  constexpr iterator begin() noexcept {
    return impl.begin();
  }

  constexpr const_iterator begin() const noexcept {
    return impl.begin();
  }

  constexpr iterator end() noexcept {
    return impl.begin();
  }

  constexpr const_iterator end() const noexcept {
    return impl.end();
  }

  constexpr reverse_iterator rbegin() noexcept {
    return impl.rbegin();
  }

  constexpr const_reverse_iterator rbegin() const noexcept {
    return impl.rbegin();
  }

  constexpr reverse_iterator rend() noexcept {
    return impl.rend();
  }

  constexpr const_reverse_iterator rend() const noexcept {
    return impl.rend();
  }

  constexpr const_iterator cbegin() const noexcept {
    return impl.cbegin();
  }

  constexpr const_iterator cend() const noexcept {
    return impl.cend();
  }

  constexpr const_reverse_iterator crbegin() const noexcept {
    return impl.crbegin();
  }

  constexpr const_reverse_iterator crend() const noexcept {
    return impl.crend();
  }

  // capacity
  [[nodiscard]] constexpr bool empty() const noexcept {
    return impl.empty();
  }

  constexpr size_type size() const noexcept {
    return impl.size();
  }

  constexpr size_type max_size() const noexcept {
    return impl.max_size();
  }

  constexpr size_type capacity() const noexcept {
    return impl.capacity();
  }

  void resize(size_type sz) {
    const std::unique_lock lock{ mutex };
    impl.resize(sz);
  }

  void resize(size_type sz, const T& c) {
    const std::unique_lock lock{ mutex };
    impl.resize(sz, c);
  }

  void reserve(size_type n) {
    const std::unique_lock lock{ mutex };
    impl.resize(n);
  }

  void shrink_to_fit() {
    const std::unique_lock lock{ mutex };
    impl.shrink_to_fit();
  }


  // element access
  constexpr reference operator[](size_type n) {
    return impl.at(n);
  }

  constexpr const_reference operator[](size_type n) const {
    return impl.at(n);
  }

  constexpr const_reference at(size_type n) const {
    return impl.at(n);
  }

  constexpr reference at(size_type n) {
    return impl.at(n);
  }

  constexpr reference front() {
    return impl.front();
  }

  constexpr const_reference front() const {
    return impl.front();
  }

  constexpr reference back() {
    return impl.back();
  }

  constexpr const_reference back() const {
    return impl.back();
  }


  // data access
  constexpr T* data() noexcept {
    return impl.data();
  }

  constexpr const T* data() const noexcept {
    return impl.data();
  }

  // modifiers
  template<class... Args>
  reference emplace_back(Args&&... args) {
    const std::unique_lock lock{ mutex };
    return impl.emplace_back(
      std::forward<Args>(args)...
    );
  }

  void push_back(const T& x) {
    const std::unique_lock lock{ mutex };
    return impl.push_back(x);
  }

  void push_back(T&& x) {
    const std::unique_lock lock{ mutex };
    return impl.push_back(std::move(x));
  }

  void pop_back() {
    const std::unique_lock lock{ mutex };
    return impl.pop_back();
  }


  template<class... Args>
  iterator emplace(const_iterator position, Args&&... args) {
    const std::unique_lock lock{ mutex };
    return impl.emplace(
      position,
      std::forward<Args>(args)...
    );
  }

  iterator insert(const_iterator position, const T& x) {
    const std::unique_lock lock{ mutex };
    return impl.insert(position, x);
  }

  iterator insert(const_iterator position, T&& x) {
    const std::unique_lock lock{ mutex };
    return impl.insert(position, std::move(x));
  }

  iterator insert(const_iterator position, size_type n, const T& x) {
    const std::unique_lock lock{ mutex };
    return impl.insert(position, n, x);
  }

  template<class InputIt>
  iterator insert(
    const_iterator position,
    InputIt first,
    InputIt last
  ) {
    const std::unique_lock lock{ mutex };
    return impl.insert(position, first, last);
 }

  iterator insert(
    const_iterator position,
    std::initializer_list<T> il
  ) {
    const std::unique_lock lock{ mutex };
    return impl.insert(position, il);
  }

  iterator erase(const_iterator position) {
    const std::unique_lock lock{ mutex };
    return impl.erase(position);
  }

  iterator erase(const_iterator first, const_iterator last) {
    const std::unique_lock lock{ mutex };
    return impl.erase(first, last);
  }

  void swap(vector& arg) noexcept(
    std::allocator_traits<Allocator>::propagate_on_container_swap::value
    || std::allocator_traits<Allocator>::is_always_equal::value
  ) {
    const std::unique_lock lock{ mutex };
    impl.swap(arg);
  }

  void clear() noexcept {
    const std::unique_lock lock{ mutex };
    impl.clear();
  }

};

} // namespace safe
} // namespace uitsl

#endif // #ifndef UITSL_CONTAINERS_SAFE_VECTOR_HPP_INCLUDE
