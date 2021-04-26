#pragma once
#ifndef UITSL_UTILITY_UIDMAP_HPP_INCLUDE
#define UITSL_UTILITY_UIDMAP_HPP_INCLUDE

#include <unordered_map>

namespace uitsl {

/*
 * Helper class for toroidal topology factory that sifts for complementary
 * from/to and to/from requests and links them to the same edge ID.
 */
template <typename T>
class UIDMap {
  using node_id_t = size_t;
  using node_tuple = std::tuple<bool, T, T>;

  size_t counter{};

  std::unordered_multimap<
    node_tuple,
    size_t,
    emp::TupleHash<bool, T, T>
  > map;

public:
  size_t operator[](const node_tuple& a) {
    const auto& [is_output, from_node, to_node] = a;
    const node_tuple complement{
      !is_output,
      from_node,
      to_node
    };

    if (map.count(complement)) {
      const auto it = map.find(complement);
      const auto [key, val] = *it;
      map.erase(it);
      return val;
    } else {
      return map.insert({a, counter++})->second;
    }
  }
};

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_UIDMAP_HPP_INCLUDE
