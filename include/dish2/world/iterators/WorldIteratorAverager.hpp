#pragma once
#ifndef DISH2_WORLD_ITERATORS_WORLDITERATORAVERAGER_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_WORLDITERATORAVERAGER_HPP_INCLUDE

#include <iterator>
#include <numeric>
#include <tuple>

#include "../../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template< typename CellIterator, typename CardinalIterator >
class WorldIteratorAverager : public CellIterator {

public:

  using value_type = double;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;

  WorldIteratorAverager(const CellIterator& cell_iterator)
  : CellIterator( cell_iterator ) { }

  const double operator*() {
    return std::accumulate(
      CellIterator::operator*().template begin<CardinalIterator>(),
      CellIterator::operator*().template end<CardinalIterator>(),
      double{}
    ) / std::distance(
      CellIterator::operator*().template begin<CardinalIterator>(),
      CellIterator::operator*().template end<CardinalIterator>()
    );
  }

  WorldIteratorAverager& operator++() {
    CellIterator::operator++();
    return *this;
  }

  WorldIteratorAverager operator++(int) {
    const auto res = *this;
    CellIterator::operator++();
    return res;
  }


  const value_type* operator->() = delete;

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_WORLDITERATORAVERAGER_HPP_INCLUDE
