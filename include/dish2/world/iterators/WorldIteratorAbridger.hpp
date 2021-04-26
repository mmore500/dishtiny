#pragma once
#ifndef DISH2_WORLD_ITERATORS_WORLDITERATORABRIDGER_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_WORLDITERATORABRIDGER_HPP_INCLUDE

#include <tuple>

#include "../../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template< typename CellIterator, typename CardinalIterator >
class WorldIteratorAbridger : public CellIterator {

public:

  using value_type = typename CardinalIterator::value_type;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;

  WorldIteratorAbridger(const CellIterator& cell_iterator)
  : CellIterator( cell_iterator ) { }

  const value_type& operator*() const {
    return *( CellIterator::operator*().template begin<CardinalIterator>() );
  }

  const value_type* operator->() const { return &operator*(); }

  WorldIteratorAbridger& operator++() {
    CellIterator::operator++();
    return *this;
  }

  WorldIteratorAbridger operator++(int) {
    const auto res = *this;
    CellIterator::operator++();
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_WORLDITERATORABRIDGER_HPP_INCLUDE
