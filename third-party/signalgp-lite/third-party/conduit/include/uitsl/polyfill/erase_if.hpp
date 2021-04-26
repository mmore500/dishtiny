#pragma once
#ifndef UITSL_POLYFILL_ERASE_IF_HPP_INCLUDE
#define UITSL_POLYFILL_ERASE_IF_HPP_INCLUDE

#include <unordered_map>

namespace std {

template< typename... Args, typename Pred >
size_t erase_if( std::unordered_map<Args...>& c, Pred pred ) {

  auto old_size = c.size();
  for (auto i = c.begin(), last = c.end(); i != last; ) {
    if (pred(*i)) {
      i = c.erase(i);
    } else {
      ++i;
    }
  }
  return old_size - c.size();

}

} // namespace std

#endif // #ifndef UITSL_POLYFILL_ERASE_IF_HPP_INCLUDE
