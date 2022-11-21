#pragma once
#ifndef DISH2_WORLD_ITERATORS_LIVECELLEXCEPTFOCALROOTIDITERATOR_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_LIVECELLEXCEPTFOCALROOTIDITERATOR_HPP_INCLUDE

#include <algorithm>
#include <cstddef>
#include <iterator>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../cell/Cell.hpp"

namespace dish2 {

template<typename Spec>
class LiveCellExceptFocalRootIDIterator
: public emp::vector<dish2::Cell<Spec>>::const_iterator {

  using parent_t
    = typename emp::vector<dish2::Cell<Spec>>::const_iterator;

  parent_t begin;
  parent_t end;
  size_t focal_root_id;

  LiveCellExceptFocalRootIDIterator(
    const parent_t& init,
    const parent_t& begin_,
    const parent_t& end_,
    const size_t focal_root_id_
  ) : parent_t(init)
  , begin(begin_)
  , end(end_)
  , focal_root_id(focal_root_id_)
  {}

public:

  LiveCellExceptFocalRootIDIterator(const LiveCellExceptFocalRootIDIterator &) = default;

  static LiveCellExceptFocalRootIDIterator make_begin(
    const emp::vector<dish2::Cell<Spec>>& population,
    const size_t focal_root_id_
  ) {
    return dish2::LiveCellExceptFocalRootIDIterator<Spec>{
      std::find_if(
        std::begin( population ), std::end( population ),
        [focal_root_id_]( const auto& cell ){
          return cell.GetRootID().value_or(focal_root_id_) != focal_root_id_;
        }
      ),
      std::begin( population ),
      std::end( population ),
      focal_root_id_
    };
  }

  static LiveCellExceptFocalRootIDIterator make_end(
    const emp::vector<dish2::Cell<Spec>>& population,
    const size_t focal_root_id_
  ) {
    return LiveCellExceptFocalRootIDIterator<Spec>{
      std::end(population),
      std::begin(population),
      std::end(population),
      focal_root_id_
    };
  }

  using value_type = dish2::Cell<Spec>;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = typename parent_t::difference_type;

  const value_type& operator*() const { return parent_t::operator*(); }

  const value_type* operator->() const { return &operator*(); }

  LiveCellExceptFocalRootIDIterator& operator++() {

    do {
      parent_t::operator++();
    } while (
      *this != end
      && (
        parent_t::operator*().GetRootID().value_or(focal_root_id)
        == focal_root_id
      )
    );

    emp_assert(
      *this == end
      || (
        parent_t::operator*().GetRootID().value_or(focal_root_id)
        != focal_root_id
      )
    );

    return *this;
  }

  LiveCellExceptFocalRootIDIterator operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  LiveCellExceptFocalRootIDIterator& operator--() {

    // might not quite be right, but should work in practice
    // in the case of extinction, this will yield the begin() iterator
    // which might not be alive
    // (alternative idea would be to set ourselves
    // to be an end iterator if no alive cells are available)
    // (another alternative idea would be to raise an exception if no live
    // cells are available)
    do {
      parent_t::operator--();
    } while (
      *this != begin
      && (
        parent_t::operator*().GetRootID().value_or(focal_root_id)
        == focal_root_id
      )
    );

    emp_assert(
      *this == begin
      || (
        parent_t::operator*().GetRootID().value_or(focal_root_id)
        != focal_root_id
      )
    );

    return *this;
  }

  LiveCellExceptFocalRootIDIterator operator--(int) {
    const auto res = *this;
    operator--();
    return res;
  }

  bool operator==(const LiveCellExceptFocalRootIDIterator& other) const {
    return static_cast<parent_t>( *this ) == static_cast<parent_t>( other );
  }

  bool operator!=(const LiveCellExceptFocalRootIDIterator& other) const {
    return static_cast<parent_t>( *this ) != static_cast<parent_t>( other );
  }

  size_t CalcDistance( const parent_t& from ) const {
    return std::distance( from, static_cast<parent_t>( *this ) );
  }


};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_LIVECELLEXCEPTFOCALROOTIDITERATOR_HPP_INCLUDE
