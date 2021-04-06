#pragma once
#ifndef DISH2_WORLD_ITERATORS_LIVECELLIDXITERATOR_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_LIVECELLIDXITERATOR_HPP_INCLUDE

#include <algorithm>
#include <iterator>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../cell/Cell.hpp"

namespace dish2 {

template<typename Spec>
class LiveCellIdxIterator
: protected emp::vector<dish2::Cell<Spec>>::const_iterator {

  using parent_t
    = typename emp::vector<dish2::Cell<Spec>>::const_iterator;

  parent_t begin;
  parent_t end;

  LiveCellIdxIterator(
    const parent_t& init,
    const parent_t& begin_,
    const parent_t& end_
  ) : parent_t(init)
  , begin(begin_)
  , end(end_)
  {}

public:

  LiveCellIdxIterator(const LiveCellIdxIterator &) = default;

  static LiveCellIdxIterator make_begin(
    const emp::vector<dish2::Cell<Spec>>& population
  ) {
    return dish2::LiveCellIdxIterator<Spec>{
      std::find_if(
        std::begin( population ), std::end( population ),
        []( const auto& cell ){ return cell.IsAlive(); }
      ),
      std::begin( population ),
      std::end( population )
    };
  }

  static LiveCellIdxIterator make_end(
    const emp::vector<dish2::Cell<Spec>>& population
  ) {
    return LiveCellIdxIterator<Spec>{
      std::end(population),
      std::begin(population),
      std::end(population)
    };
  }

  using value_type = size_t;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = typename parent_t::difference_type;

  const value_type operator*() {
    return std::distance( begin, static_cast<parent_t>( *this ) );
  }

  LiveCellIdxIterator& operator++() {

    do {
      parent_t::operator++();
    } while (
      *this != end
      && ! parent_t::operator*().IsAlive()
    );

    emp_assert( *this == end || parent_t::operator*().IsAlive() );

    return *this;
  }

  LiveCellIdxIterator operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  LiveCellIdxIterator& operator--() {

    // might not quite be right, but should work in practice
    do {
      parent_t::operator--();
    } while (
      *this != std::prev( begin )
      && !parent_t::operator*().IsAlive()
    );

    emp_assert(
      *this != std::prev( begin ) || parent_t::operator*().IsAlive()
    );

    return *this;
  }

  LiveCellIdxIterator operator--(int) {
    const auto res = *this;
    operator--();
    return res;
  }

  bool operator==(const LiveCellIdxIterator& other) const {
    return static_cast<parent_t>( *this ) == static_cast<parent_t>( other );
  }

  bool operator!=(const LiveCellIdxIterator& other) const {
    return static_cast<parent_t>( *this ) != static_cast<parent_t>( other );
  }


};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_LIVECELLIDXITERATOR_HPP_INCLUDE
