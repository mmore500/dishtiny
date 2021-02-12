#pragma once
#ifndef DISH2_UTILITY_BYTEWISEITERATOR_HPP_INCLUDE
#define DISH2_UTILITY_BYTEWISEITERATOR_HPP_INCLUDE

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace dish2 {

template<typename It>
class BytewiseIterator {

  using element_t = std::decay_t< decltype( *std::declval<It>() ) >;
  static_assert( std::is_trivial< element_t >::value );

  It element_it;
  size_t byte_idx{};

public:

  using value = unsigned char;
  using reference = value&;
  using const_reference = const value&;
  using iterator_category = std::forward_iterator_tag;

  BytewiseIterator() = default;

  BytewiseIterator(const It& element_it_)
  : element_it(element_it_)
  {}

  // postfix
  BytewiseIterator operator++(int) {
    BytewiseIterator res( *this );
    operator++();
    return res;
  }

  // prefix
  BytewiseIterator& operator++() {
    ++byte_idx;
    if ( byte_idx == sizeof( *element_it ) ) {
      byte_idx = 0;
      ++element_it;
    }
    return *this;
  }

  const_reference operator*() const {
    auto& element = *element_it;
    const unsigned char* first_bye
      = reinterpret_cast<const unsigned char *>( &element );
    const unsigned char* cur_byte = first_bye + byte_idx;
    return *cur_byte;
  }

  bool operator==(const BytewiseIterator& rhs) const {
    return std::tuple{
      element_it, byte_idx
    } == std::tuple{
      rhs.element_it, rhs.byte_idx
    };
  }

  bool operator!=(const BytewiseIterator& rhs) const {
    return !operator==(rhs);
  }

};

} // namespace dish2


namespace std {

template < typename It >
struct iterator_traits< dish2::BytewiseIterator<It> > {
  using iterator_category
    = typename dish2::BytewiseIterator<It>::iterator_category;
};

} // namespace std

#endif // #ifndef DISH2_UTILITY_BYTEWISEITERATOR_HPP_INCLUDE
