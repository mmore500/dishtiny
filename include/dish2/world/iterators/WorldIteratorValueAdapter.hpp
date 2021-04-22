#pragma once
#ifndef DISH2_WORLD_ITERATORS_WORLDITERATORVALUEADAPTER_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_WORLDITERATORVALUEADAPTER_HPP_INCLUDE

#include <tuple>

#include "../../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template< typename CellIterator, typename CardinalIterator >
class WorldIteratorValueAdapter {


  CardinalIterator cardinal_iterator;
  CellIterator cell_iterator;

  void Rectify() {
    if (
      cardinal_iterator == cell_iterator->template end<CardinalIterator>()
    ) {
      ++cell_iterator;
      cardinal_iterator = cell_iterator->template begin<CardinalIterator>();
    }
  }

  WorldIteratorValueAdapter(
    CardinalIterator cardinal_iterator_, CellIterator cell_iterator_
  ) : cardinal_iterator( cardinal_iterator_ )
  , cell_iterator( cell_iterator_ )
  {}

public:

  static WorldIteratorValueAdapter make_begin( CellIterator cell_iterator ) {
    return WorldIteratorValueAdapter{
      cell_iterator->template begin< CardinalIterator >(),
      cell_iterator
    };
  }

  static WorldIteratorValueAdapter make_end( CellIterator cell_iterator ) {
    // ideally, just use std::prev()
    // but for some reason that's calling operator++ instead of operator--
    // see backtrace at https://pastebin.com/4nGpEcCy
    --cell_iterator;
    return WorldIteratorValueAdapter{
      cell_iterator->template end< CardinalIterator >(),
      cell_iterator
    };
  }

  using value_type = typename CardinalIterator::value_type;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;

  WorldIteratorValueAdapter() = default;

  const value_type operator*() { Rectify(); return *cardinal_iterator; }

  WorldIteratorValueAdapter& operator++() {

    Rectify();

    ++cardinal_iterator;

    return *this;
  }

  WorldIteratorValueAdapter operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  bool operator==(const WorldIteratorValueAdapter& other) const {
    return std::tuple{
      cell_iterator,
      cardinal_iterator
    } == std::tuple{
      other.cell_iterator,
      other.cardinal_iterator
    };
  }

  bool operator!=(const WorldIteratorValueAdapter& other) const {
    return !operator==( other );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_WORLDITERATORVALUEADAPTER_HPP_INCLUDE
