#pragma once
#ifndef UITSL_ALGORITHM_UPPER_UNIQUIFY_HPP_INCLUDE
#define UITSL_ALGORITHM_UPPER_UNIQUIFY_HPP_INCLUDE

#include <map>
#include <utility>

namespace uitsl {

template<typename Key, typename T>
void upper_uniquify(std::multimap<Key, T>& map) {

  if (map.size() < 2) return;

  auto prev = std::begin( map );
  auto cur = std::next( prev );

  while ( cur != std::end( map ) ) {

    if ( prev->first == cur->first ) map.erase( prev );

    prev = std::exchange(
      cur,
      std::next(cur)
    );

  }

}

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_UPPER_UNIQUIFY_HPP_INCLUDE
