#pragma once
#ifndef UITSL_CONTAINERS_SAFE_DEQUE_HPP_INCLUDE
#define UITSL_CONTAINERS_SAFE_DEQUE_HPP_INCLUDE

#include <deque>
#include <shared_mutex>

namespace uitsl {
namespace safe {

template<class T, class Allocator = std::allocator<T>>
class deque {

  using impl_t = std::deque<T, Allocator>;

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

  // construct/copy/destroy
  template<typename... Args>
  deque(Args&&... args)
  : impl(std::forward<Args>(args)...)
  { ; }

  deque& operator=(const deque& x) {
    const std::unique_lock lock{ mutex };
    impl.operator=(x);
    return *this;
  }

  deque& operator=(deque&& x)  noexcept (
    std::allocator_traits<Allocator>::is_always_equal::value
  ) {
    const std::unique_lock lock{ mutex };
    impl.operator=(std::move(x));
    return *this;
  }

  deque& operator=(std::initializer_list<T> arg) {
    const std::unique_lock lock{ mutex };
    impl.operator=(arg);
    return *this;
  }

  template<class InputIt>
  void assign(InputIt first, InputIt last) {
    const std::unique_lock lock{ mutex };
    impl.assign(first, last);
  }

  void assign(size_type n, const T& t) {
    const std::unique_lock lock{ mutex };
    impl.assign(n, t);
  }

  void assign(std::initializer_list<T> arg) {
    const std::unique_lock lock{ mutex };
    impl.assign(arg);
  }

  allocator_type get_allocator() const noexcept {
    return impl.get_allocator();
  }


  // iterators
  iterator begin() noexcept { return impl.begin(); }

  const_iterator begin() const noexcept { return impl.begin(); }

  iterator end() noexcept { return impl.end(); }

  const_iterator end() const noexcept { return impl.end(); }

  reverse_iterator rbegin() noexcept { return impl.rbegin(); }

  const_reverse_iterator rbegin() const noexcept { return impl.rbegin(); }

  reverse_iterator rend() noexcept { return impl.rend(); }

  const_reverse_iterator rend() const noexcept { return impl.rend(); }


  const_iterator cbegin() const noexcept { return impl.cbegin(); }

  const_iterator cend() const noexcept { return impl.cend(); }

  const_reverse_iterator crbegin() const noexcept { return impl.crbegin(); }

  const_reverse_iterator crend() const noexcept { return impl.crend(); }


  // capacity
  [[nodiscard]] bool empty() const noexcept { return impl.empty(); }

  size_type size() const noexcept { return impl.size(); }

  size_type max_size() const noexcept { return impl.max_size(); }

  void resize(size_type sz) {
    const std::unique_lock lock{ mutex };
    return impl.resize(sz);
  }

  void resize(size_type sz, const T& c) {
    const std::unique_lock lock{ mutex };
    return impl.resize(sz, c);
  }

  void shrink_to_fit() {
    const std::unique_lock lock{ mutex };
    impl.shrink_to_fit();
  }


  // element access
  reference operator[](size_type n) {
    return impl.operator[](n);
  }

  const_reference operator[](size_type n) const {
    return impl.operator[](n);
  }

  reference at(size_type n) {
    return impl.at(n);
  }

  const_reference at(size_type n) const {
    return impl.at(n);
  }

  reference front() {
    return impl.front();
  }

  const_reference front() const {
    return impl.front();
  }

  reference back() {
    return impl.back();
  }

  const_reference back() const {
    return impl.back();
  }


  // modifiers
  template<class... Args>
  reference emplace_front(Args&&... args) {
    const std::unique_lock lock{ mutex };
    return impl.emplace_front(std::forward<Args>(args)...);
  }

  template<class... Args>
  reference emplace_back(Args&&... args) {
    const std::unique_lock lock{ mutex };
    return impl.emplace_back(std::forward<Args>(args)...);
  }

  template<class... Args>
  iterator emplace(const_iterator position, Args&&... args) {
    const std::unique_lock lock{ mutex };
    return impl.emplace(position, std::forward<Args>(args)...);
  }

  void push_front(const T& x) {
    const std::unique_lock lock{ mutex };
    impl.push_front(x);
  }

  void push_front(T&& x) {
    const std::unique_lock lock{ mutex };
    impl.push_front(std::move(x));
  }

  void push_back(const T& x) {
    const std::unique_lock lock{ mutex };
    impl.push_back(x);
  }

  void push_back(T&& x) {
    const std::unique_lock lock{ mutex };
    impl.push_back(std::move(x));
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
  iterator insert(const_iterator position, InputIt first, InputIt last) {
    const std::unique_lock lock{ mutex };
    return impl.insert(position, first, last);
  }

  iterator insert(const_iterator position, std::initializer_list<T> arg) {
    const std::unique_lock lock{ mutex };
    return impl.insert(position, arg);
  }


  void pop_front() {
    const std::unique_lock lock{ mutex };
    return impl.pop_front();
  }

  void pop_back() {
    const std::unique_lock lock{ mutex };
    return impl.pop_back();
  }

  iterator erase(const_iterator position) {
    const std::unique_lock lock{ mutex };
    return impl.erase(position);
  }

  iterator erase(const_iterator first, const_iterator last) {
    const std::unique_lock lock{ mutex };
    return impl.erase(first, last);
  }

  void swap(deque& arg) noexcept (
    std::allocator_traits<Allocator>::is_always_equal::value
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

#endif // #ifndef UITSL_CONTAINERS_SAFE_DEQUE_HPP_INCLUDE
