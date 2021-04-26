#pragma once
#ifndef UITSL_CONTAINERS_SAFE_LIST_HPP_INCLUDE
#define UITSL_CONTAINERS_SAFE_LIST_HPP_INCLUDE

#include <list>
#include <shared_mutex>

namespace uitsl {
namespace safe {

template<class T, class Allocator = std::allocator<T>>
class list {

  using impl_t = std::list<T, Allocator>;

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
  explicit list(Args&&... args)
  : impl(std::forward<Args>(args)...)
  { ; }


  // iterators
  iterator begin() noexcept {
    return impl.begin();
  }

  const_iterator begin() const noexcept {
    return impl.begin();
  }

  iterator end() noexcept {
    return impl.end();
  }

  const_iterator end() const noexcept {
    return impl.end();
  }

  reverse_iterator rbegin() noexcept {
    return impl.rbegin();
  }

  const_reverse_iterator rbegin() const noexcept {
    return impl.rbegin();
  }

  reverse_iterator rend() noexcept {
    return impl.rend();
  }

  const_reverse_iterator rend() const noexcept {
    return impl.rend();
  }


  const_iterator cbegin() const noexcept {
    return impl.cbegin();
  }

  const_iterator cend() const noexcept {
    return impl.cend();
  }

  const_reverse_iterator crbegin() const noexcept {
    return impl.crbegin();
  }

  const_reverse_iterator crend() const noexcept {
    return impl.crend();
  }


  // capacity
  [[nodiscard]] bool empty() const noexcept {
    return impl.empty();
  }

  size_type size() const noexcept {
    return impl.size();
  }

  size_type max_size() const noexcept {
    return impl.max_size();
  }

  void resize(size_type sz) {
    const std::unique_lock lock{ mutex };
    impl.resize(sz);
  }

  void resize(size_type sz, const T& c) {
    const std::unique_lock lock{ mutex };
    impl.resize(sz, c);
  }


  // element access
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

  void push_front(const T& x) {
    const std::unique_lock lock{ mutex };
    impl.push_front(x);
  }

  void push_front(T&& x) {
    const std::unique_lock lock{ mutex };
    impl.push_front(std::move(x));
  }

  void pop_front() {
    const std::unique_lock lock{ mutex };
    impl.pop_front();
  }

  void push_back(const T& x) {
    const std::unique_lock lock{ mutex };
    impl.push_back(x);
  }

  void push_back(T&& x) {
    const std::unique_lock lock{ mutex };
    impl.push_back(std::move(x));
  }

  void pop_back() {
    const std::unique_lock lock{ mutex };
    impl.pop_back();
  }


  template<class... Args>
  iterator emplace(const_iterator position, Args&&... args) {
    const std::unique_lock lock{ mutex };
    return impl.emplace(position, std::forward<Args>(args)...);
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

  iterator insert(const_iterator position, std::initializer_list<T> il) {
    const std::unique_lock lock{ mutex };
    return impl.insert(position, il);
  }


  iterator erase(const_iterator position) {
    const std::unique_lock lock{ mutex };
    return impl.erase(position);
  }

  iterator erase(const_iterator position, const_iterator last) {
    const std::unique_lock lock{ mutex };
    return impl.erase(position, last);
  }

  void swap(list& arg) noexcept(
    std::allocator_traits<Allocator>::is_always_equal::value
  ) {
    const std::unique_lock lock{ mutex };
    impl.swap(arg);
  }

  void clear() noexcept {
    const std::unique_lock lock{ mutex };
    impl.clear();
  }

  // list operations
  void splice(const_iterator position, list& x) {
    const std::unique_lock lock{ mutex };
    impl.splice(position, x);
  }

  void splice(const_iterator position, list&& x) {
    const std::unique_lock lock{ mutex };
    impl.splice(position, std::move(x));
  }

  void splice(const_iterator position, list& x, const_iterator i) {
    const std::unique_lock lock{ mutex };
    impl.splice(position, x, i);
  }

  void splice(const_iterator position, list&& x, const_iterator i) {
    const std::unique_lock lock{ mutex };
    impl.splice(position, std::move(x), i);
  }

  void splice(
    const_iterator position,
    list& x,
    const_iterator first,
    const_iterator last
  ) {
    const std::unique_lock lock{ mutex };
    impl.splice(position, x, first, last);
  }

  void splice(
    const_iterator position,
    list&& x,
    const_iterator first,
    const_iterator last
  ) {
    const std::unique_lock lock{ mutex };
    impl.splice(position, std::move(x), first, last);
  }

  size_type remove(const T& value) {
    const std::unique_lock lock{ mutex };
    return impl.remove(value);
  }

  template<class Predicate>
  size_type remove_if(Predicate pred) {
    const std::unique_lock lock{ mutex };
    return impl.remove_if(pred);
  }


  size_type unique() {
    const std::unique_lock lock{ mutex };
    return impl.unique();
  }

  template<class BinaryPredicate>
  size_type unique(BinaryPredicate binary_pred) {
    const std::unique_lock lock{ mutex };
    return impl.unique(binary_pred);
  }


  void merge(list& x) {
    const std::unique_lock lock{ mutex };
    impl.merge(x);
  }

  void merge(list&& x) {
    const std::unique_lock lock{ mutex };
    impl.merge(std::move(x));
  }

  template<class Compare>
  void merge(list& x, Compare comp) {
    const std::unique_lock lock{ mutex };
    impl.merge(x, comp);
  }

  template<class Compare>
  void merge(list&& x, Compare comp) {
    const std::unique_lock lock{ mutex };
    impl.merge(std::move(x), comp);
  }


  void sort() {
    const std::unique_lock lock{ mutex };
    impl.sort();
  }

  template<class Compare>
  void sort(Compare comp) {
    const std::unique_lock lock{ mutex };
    impl.sort(comp);
  }


  void reverse() noexcept {
    const std::unique_lock lock{ mutex };
    impl.reverse();
  }

};

} // namespace safe
} // namespace uitsl

#endif // #ifndef UITSL_CONTAINERS_SAFE_LIST_HPP_INCLUDE
