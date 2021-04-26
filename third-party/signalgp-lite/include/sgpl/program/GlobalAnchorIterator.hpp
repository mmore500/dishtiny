#pragma once
#ifndef SGPL_PROGRAM_GLOBALANCHORITERATOR_HPP_INCLUDE
#define SGPL_PROGRAM_GLOBALANCHORITERATOR_HPP_INCLUDE

#include <iterator>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "Instruction.hpp"

namespace sgpl {

template<typename Spec>
class GlobalAnchorIterator
: protected emp::vector<sgpl::Instruction<Spec>>::const_iterator {

  using inst_t = sgpl::Instruction<Spec>;
  using library_t = typename Spec::library_t;
  using container_t = emp::vector<sgpl::Instruction<Spec>>;
  using parent_t = typename container_t::const_iterator;

  parent_t end;

  GlobalAnchorIterator(
    const parent_t& init,
    const parent_t& end_
  ) : parent_t(init)
  , end(end_)
  {}


public:

  static GlobalAnchorIterator make_begin( const container_t& container ) {
    return GlobalAnchorIterator(
      std::begin( container ),
      std::end( container )
    );
  }

  static GlobalAnchorIterator make_end( const container_t& container ) {
    return GlobalAnchorIterator(
      std::end( container ),
      std::end( container )
    );
  }

  using value_type = inst_t;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = typename parent_t::difference_type;

  const value_type& operator*() { return parent_t::operator*(); }

  const value_type* operator->() { return &operator*(); }

  GlobalAnchorIterator& operator++() {

    bool has_encountered_local_anchor{ false };

    // todo fixme: this doesn't take into account circular genome wraparound
    do {
      has_encountered_local_anchor |= library_t::IsAnchorLocalOpCode(
        parent_t::operator*().op_code
      );
      parent_t::operator++();
    } while (
      *this != end
      && (
        !library_t::IsAnchorGlobalOpCode( parent_t::operator*().op_code )
        || !has_encountered_local_anchor
      )
    );
    return *this;
  }

  GlobalAnchorIterator operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  bool operator==(const GlobalAnchorIterator& other) const {
    return static_cast<parent_t>( *this ) == static_cast<parent_t>( other );
  }

  bool operator!=(const GlobalAnchorIterator& other) const {
    return static_cast<parent_t>( *this ) != static_cast<parent_t>( other );
  }

  size_t CalcDistance( const parent_t& from ) const {
    return std::distance( from, static_cast<parent_t>( *this ) );
  }


};

} // namespace sgpl

#endif // #ifndef SGPL_PROGRAM_GLOBALANCHORITERATOR_HPP_INCLUDE
