#pragma once
#ifndef UITSL_CONTAINERS_SAFE_UNORDERED_MAP_HPP_INCLUDE
#define UITSL_CONTAINERS_SAFE_UNORDERED_MAP_HPP_INCLUDE

#include <shared_mutex>
#include <unordered_map>

namespace uitsl {
namespace safe {

template<
  class Key,
  class T,
  class Hash = std::hash<Key>,
  class Pred = std::equal_to<Key>,
  class Allocator = std::allocator<std::pair<const Key, T>>
>
class unordered_map {

  using impl_t = std::unordered_map<
    Key,
    T,
    Hash,
    Pred,
    Allocator
  >;

  mutable std::shared_mutex mutex;

  impl_t impl;

public:

  using allocator_type = typename impl_t::allocator_type;
  using const_iterator = typename impl_t::const_iterator;
  using const_local_iterator = typename impl_t::const_local_iterator;
  using insert_return_type = typename impl_t::insert_return_type;
  using iterator = typename impl_t::iterator;
  using key_type = typename impl_t::key_type;
  using local_iterator = typename impl_t::local_iterator;
  using mapped_type = typename impl_t::mapped_type;
  using node_type = typename impl_t::node_type;
  using size_type = typename impl_t::size_type;
  using value_type = typename impl_t::value_type;

  template<typename... Args>
  explicit unordered_map(Args&&... args)
  : impl(std::forward<Args>(args)...)
  { ; }


  unordered_map& operator=(const unordered_map& arg) {
    const std::unique_lock lock{ mutex };
    impl.operator=(arg);
    return *this;
  }

  unordered_map& operator=(unordered_map&& arg) noexcept(
    std::allocator_traits<Allocator>::is_always_equal::value
    && std::is_nothrow_move_assignable_v<Hash>
    && std::is_nothrow_move_assignable_v<Pred>
  ) {
    const std::unique_lock lock{ mutex };
    impl.operator=(std::move(arg));
    return *this;
  }

  unordered_map& operator=(std::initializer_list<value_type> arg) {
    const std::unique_lock lock{ mutex };
    impl.operator=(arg);
    return *this;
  }

  allocator_type get_allocator() const noexcept {
    return impl.get_allocator();
  }

  // iterators
  iterator begin() noexcept { return impl.begin(); }

  const_iterator begin() const noexcept { return impl.begin(); }

  iterator end() noexcept { return impl.end(); }

  const_iterator end() const noexcept { return impl.end(); }

  const_iterator cbegin() const noexcept { return impl.cbegin(); }

  const_iterator cend() const noexcept { return impl.cend(); }

  // capacity
  [[nodiscard]] bool empty() const noexcept { return impl.empty(); }

  size_type size() const noexcept { return impl.size(); }

  size_type max_size() const noexcept { return impl.max_size(); }

  // modifiers
  template<class... Args>
  std::pair<iterator, bool> emplace(Args&&... args) {
    const std::unique_lock lock{ mutex };
    return impl.emplace(std::forward<Args>(args)...);
  }

  template<class... Args>
  iterator emplace_hint(
    const_iterator position,
    Args&&... args
  ) {
    const std::unique_lock lock{ mutex };
    return impl.emplace_hint(
      position,
      std::forward<Args>(args)...
    );
  }

  std::pair<iterator, bool> insert(const value_type& obj) {
    const std::unique_lock lock{ mutex };
    return impl.insert(obj);
  }

  std::pair<iterator, bool> insert(value_type&& obj) {
    const std::unique_lock lock{ mutex };
    return impl.insert(std::move(obj));
  }

  template<class P>
  std::pair<iterator, bool> insert(P&& obj) {
    const std::unique_lock lock{ mutex };
    return impl.insert(std::move(obj));
  }

  iterator insert(const_iterator hint, const value_type& obj) {
    const std::unique_lock lock{ mutex };
    return impl.insert(hint, obj);
  }

  iterator insert(const_iterator hint, value_type&& obj) {
    const std::unique_lock lock{ mutex };
    return impl.insert(hint, std::move(obj));
  }

  template<class P>
  iterator insert(const_iterator hint, P&& obj) {
    const std::unique_lock lock{ mutex };
    return impl.insert(hint, std::move(obj));
  }

  template<class InputIt>
  void insert(InputIt first, InputIt last) {
    const std::unique_lock lock{ mutex };
    return impl.insert(first, last);
  }

  void insert(std::initializer_list<value_type> arg) {
    const std::unique_lock lock{ mutex };
    return impl.insert(arg);
  }

  node_type extract(const_iterator position) {
    const std::unique_lock lock{ mutex };
    return impl.insert(position);
  }

  node_type extract(const key_type& x) {
    const std::unique_lock lock{ mutex };
    return impl.extract(x);
  }

  insert_return_type insert(node_type&& nh) {
    const std::unique_lock lock{ mutex };
    return impl.insert(std::move(nh));
  }

  iterator insert(const_iterator hint, node_type&& nh) {
    const std::unique_lock lock{ mutex };
    return impl.insert(hint, std::move(nh));
  }

  template<class... Args>
  std::pair<iterator, bool> try_emplace(const key_type& k, Args&&... args) {
    const std::unique_lock lock{ mutex };
    return impl.try_emplace(
      k,
      std::forward<Args>(args)...
    );
  }

  template<class... Args>
  std::pair<iterator, bool> try_emplace(key_type&& k, Args&&... args) {
    const std::unique_lock lock{ mutex };
    return impl.try_emplace(
      k,
      std::forward<Args>(args)...
    );
  }

  template<class... Args>
  iterator try_emplace(const_iterator hint, const key_type& k, Args&&... args) {
    const std::unique_lock lock{ mutex };
    return impl.try_emplace(
      hint,
      k,
      std::forward<Args>(args)...
    );
  }

  template<class... Args>
  iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args) {
    const std::unique_lock lock{ mutex };
    return impl.try_emplace(
      hint,
      std::move(k),
      std::forward<Args>(args)...
    );
  }

  template<class M>
  std::pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj) {
    const std::unique_lock lock{ mutex };
    return impl.insert_or_assign(
      k,
      std::move(obj)
    );
  }

  template<class M>
  std::pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj) {
    const std::unique_lock lock{ mutex };
    return impl.insert_or_assign(
      std::move(k),
      std::move(obj)
    );
  }

  template<class M>
  iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj) {
    const std::unique_lock lock{ mutex };
    return impl.insert_or_assign(
      hint,
      k,
      std::move(obj)
    );
  }

  template<class M>
  iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj) {
    const std::unique_lock lock{ mutex };
    return impl.insert_or_assign(
      hint,
      std::move(k),
      std::move(obj)
    );
  }

  iterator erase(iterator position) {
    const std::unique_lock lock{ mutex };
    return impl.erase(position);
  }

  iterator erase(const_iterator position) {
    const std::unique_lock lock{ mutex };
    return impl.erase(position);
  }

  size_type erase(const key_type& k) {
    const std::unique_lock lock{ mutex };
    return impl.erase(k);
  }

  iterator erase(const_iterator first, const_iterator last) {
    const std::unique_lock lock{ mutex };
    return impl.erase(first, last);
  }

  void swap(unordered_map& arg) noexcept(
    std::allocator_traits<Allocator>::is_always_equal::value
    && std::is_nothrow_swappable_v<Hash>
    && std::is_nothrow_swappable_v<Pred>
  ) {
    const std::unique_lock lock{ mutex };
    impl.swap(arg);
  }

  void clear() noexcept {
    const std::unique_lock lock{ mutex };
    impl.clear();
  }

  template<class H2, class P2>
  void merge(std::unordered_map<Key, T, H2, P2, Allocator>& source) {
    const std::unique_lock lock{ mutex };
    impl.merge(source);
  }

  template<class H2, class P2>
  void merge(std::unordered_map<Key, T, H2, P2, Allocator>&& source) {
    const std::unique_lock lock{ mutex };
    impl.merge(std::move(source));
  }

  template<class H2, class P2>
  void merge(std::unordered_multimap<Key, T, H2, P2, Allocator>& source) {
    const std::unique_lock lock{ mutex };
    impl.merge(source);
  }

  template<class H2, class P2>
  void merge(std::unordered_multimap<Key, T, H2, P2, Allocator>&& source) {
    const std::unique_lock lock{ mutex };
    impl.merge(std::move(source));
  }

  // observers
  // no override
  // hasher hash_function() const;
  // key_equal key_eq() const;

  // map operations
  iterator find(const key_type& k) {
    const std::shared_lock lock{ mutex };
    return impl.find(k);
  }

  const_iterator find(const key_type& k) const {
    const std::shared_lock lock{ mutex };
    return impl.find(k);
  }

  template<class K>
  iterator find(const K& k) {
    const std::shared_lock lock{ mutex };
    return impl.find(k);
  }

  template<class K>
  const_iterator find(const K& k) const {
    const std::shared_lock lock{ mutex };
    return impl.find(k);
  }

  template<class K>
  size_type count(const key_type& k) const {
    const std::shared_lock lock{ mutex };
    return impl.count(k);
  }

  template<class K>
  size_type count(const K& k) const {
    const std::shared_lock lock{ mutex };
    return impl.count(k);
  }

  bool contains(const key_type& k) const {
    const std::shared_lock lock{ mutex };
    return impl.contains(k);
  }

  template<class K>
  bool contains(const K& k) const {
    const std::shared_lock lock{ mutex };
    return impl.contains(k);
  }

  std::pair<iterator, iterator> equal_range(const key_type& k) {
    const std::shared_lock lock{ mutex };
    return impl.equal_range(k);
  }

  std::pair<const_iterator, const_iterator> equal_range(
    const key_type& k
  ) const {
    const std::shared_lock lock{ mutex };
    return impl.equal_range(k);
  }

  template<class K>
  std::pair<iterator, iterator> equal_range(const K& k) {
    const std::shared_lock lock{ mutex };
    return impl.equal_range(k);
  }

  template<class K>
  std::pair<const_iterator, const_iterator> equal_range(const K& k) const {
    const std::shared_lock lock{ mutex };
    return impl.equal_range(k);
  }

  // element access
  mapped_type& operator[](const key_type& k) {
    const std::shared_lock lock{ mutex };
    return impl.operator[](k);
  }

  mapped_type& operator[](key_type&& k) {
    const std::shared_lock lock{ mutex };
    return impl.operator[](std::move(k));
  }

  mapped_type& at(const key_type& k) {
    const std::shared_lock lock{ mutex };
    return impl.at(k);
  }

  const mapped_type& at(const key_type& k) const {
    const std::shared_lock lock{ mutex };
    return impl.at(k);
  }

  // bucket interface
  size_type bucket_count() const noexcept {
    return impl.bucket_count();
  }

  size_type max_bucket_count() const noexcept {
    return impl.max_bucket_count();
  }

  size_type bucket_size(size_type n) const {
    return impl.bucket_size(n);
  }

  size_type bucket(const key_type& k) const {
    return impl.bucket(k);
  }

  local_iterator begin(size_type n) {
    return impl.begin(n);
  }

  const_local_iterator begin(size_type n) const {
    return impl.begin(n);
  }

  local_iterator end(size_type n) {
    return impl.end(n);
  }

  const_local_iterator end(size_type n) const {
    return impl.end(n);
  }

  const_local_iterator cbegin(size_type n) const {
    return impl.cbegin(n);
  }

  const_local_iterator cend(size_type n) const {
    return impl.cend(n);
  }

  // hash policy
  float load_factor() const noexcept {
    return impl.load_factor();
  }

  float max_load_factor() const noexcept {
    return impl.max_load_factor();
  }

  void max_load_factor(float z) {
    const std::unique_lock lock{ mutex };
    impl.max_load_factor(z);
  }

  void rehash(size_type n) {
    const std::unique_lock lock{ mutex };
    impl.rehash(n);
  }

  void reserve(size_type n) {
    const std::unique_lock lock{ mutex };
    impl.reserve(n);
  }

};

} // namespace safe
} // namespace uitsl

#endif // #ifndef UITSL_CONTAINERS_SAFE_UNORDERED_MAP_HPP_INCLUDE
