#pragma once
#ifndef DISH2_WORLD_ITERATORS_LIVECELLITERATOR_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_LIVECELLITERATOR_HPP_INCLUDE

#include <iterator>

#include "../../../../third-party/Empirical/source/base/vector.h"

#include "../../cell/Cell.hpp"

namespace dish2 {

template<typename Spec>
class LiveCellIterator
: protected std::vector<dish2::Cell<Spec>>::const_iterator {

  using parent_t
    = typename std::vector<dish2::Cell<Spec>>::const_iterator;

  parent_t end;

  LiveCellIterator(
    const parent_t& init,
    const parent_t& end_
  ) : parent_t(init)
  , end(end_)
  {}

public:

  static LiveCellIterator make_begin(
    const emp::vector<dish2::Cell<Spec>>& population
  ) {
    return ++dish2::LiveCellIterator<Spec>{
      std::prev( std::begin( population ) ), std::end( population )
    };
  }

  static LiveCellIterator make_end(
    const emp::vector<dish2::Cell<Spec>>& population
  ) {
    return LiveCellIterator<Spec>{ std::end(population), std::end(population) };
  }

  using value_type = dish2::Cell<Spec>;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = typename parent_t::difference_type;

  const value_type& operator*() { return parent_t::operator*(); }

  const value_type* operator->() { return &operator*(); }

  LiveCellIterator& operator++() {

    do {
      parent_t::operator++();
    } while (
      *this != end
      && ! parent_t::operator*().IsAlive()
    );
    return *this;
  }

  LiveCellIterator operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  bool operator==(const LiveCellIterator& other) const {
    return static_cast<parent_t>( *this ) == static_cast<parent_t>( other );
  }

  bool operator!=(const LiveCellIterator& other) const {
    return static_cast<parent_t>( *this ) != static_cast<parent_t>( other );
  }

  size_t CalcDistance( const parent_t& from ) const {
    return std::distance( from, static_cast<parent_t>( *this ) );
  }


};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_LIVECELLITERATOR_HPP_INCLUDE
