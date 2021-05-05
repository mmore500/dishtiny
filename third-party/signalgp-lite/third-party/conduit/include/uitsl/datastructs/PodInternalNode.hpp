#pragma once
#ifndef UITSL_DATASTRUCTS_PODINTERNALNODE_HPP_INCLUDE
#define UITSL_DATASTRUCTS_PODINTERNALNODE_HPP_INCLUDE

#include <tuple>
#include <type_traits>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

#include "../meta/tuple_has_type.hpp"

namespace uitsl {

template<typename First, typename... Rest>
class PodInternalNode : public std::tuple<First, Rest...> {

  using parent_t = std::tuple<First, Rest...>;
  using this_t = PodInternalNode<First, Rest...>;

  template<typename Arg1, typename... Args> friend class PodInternalNode;
  friend struct Workaround;

  // clang workaround, see https://stackoverflow.com/a/52884963
  struct Workaround {
    /*
     * Metaprogramming utility.
     */
    template<size_t RemainingSteps, size_t ChildIndex=0>
    static constexpr auto GetLeafType() noexcept {

      emp_assert( false , "GetLeafType should not be run" );

      using Child = typename std::tuple_element<ChildIndex, parent_t>::type;
      constexpr size_t ChildSteps = Child::GetSize();

      if constexpr ( RemainingSteps < ChildSteps ) {
        if constexpr ( Child::IsLeaf() ) return Child{};
        else return Child::Workaround::template GetLeafType<RemainingSteps>();
      } else return GetLeafType<RemainingSteps - ChildSteps, ChildIndex + 1>();

    }

    template<size_t RemainingSteps, size_t ChildIndex=0>
    static constexpr size_t GetLeafIndex() noexcept {

      using Child = typename std::tuple_element<ChildIndex, parent_t>::type;
      constexpr size_t ChildSteps = Child::GetSize();

      if constexpr ( RemainingSteps < ChildSteps ) {
        if constexpr ( Child::IsLeaf() ) return RemainingSteps;
        else return Child::Workaround::template GetLeafIndex<RemainingSteps>();
      } else return GetLeafIndex<RemainingSteps - ChildSteps, ChildIndex + 1>();

    }

  };

public:

  // inherit constructors
  using parent_t::parent_t;

  template< size_t Index >
  using leaf_t = decltype( Workaround::template GetLeafType<Index>() );

  /*
   * Get index into leaf node.
   */
  template<size_t Index>
  static constexpr size_t GetLeafIndex() noexcept {
    return Workaround::template GetLeafIndex< Index >();
  }

  static constexpr bool IsLeaf() noexcept { return false; }

  /*
   * Get number of descendant leaf nodes.
   */
  static constexpr size_t GetSize() noexcept {
    if constexpr ( sizeof...(Rest) > 0 ) {
      return First::GetSize() + PodInternalNode<Rest...>::GetSize();
    } else return First::GetSize();
  }

  /*
   * Get leaf by index.
   */
  template<size_t RemainingSteps, size_t ChildIndex=0>
  constexpr auto& GetByIndex() noexcept {

    using Child = typename std::tuple_element<ChildIndex, parent_t>::type;
    constexpr size_t ChildSteps = Child::GetSize();

    if constexpr ( RemainingSteps < ChildSteps ) {
      return std::get<ChildIndex>(*this).template
        GetByIndex<RemainingSteps>();
    } else return GetByIndex<RemainingSteps - ChildSteps, ChildIndex + 1>();

  }

  /*
   * Get leaf by index.
   */
  template<size_t RemainingSteps, size_t ChildIndex=0>
  constexpr auto& Get() noexcept {

    using Child = typename std::tuple_element<ChildIndex, parent_t>::type;
    constexpr size_t ChildSteps = Child::GetSize();

    if constexpr ( RemainingSteps < ChildSteps ) {
        return std::get<ChildIndex>(*this).template Get<RemainingSteps>();
    } else return Get<RemainingSteps - ChildSteps, ChildIndex + 1>();

  }

  /*
   * Get leaf by index.
   */
  template<size_t RemainingSteps, size_t ChildIndex=0>
  constexpr const auto& Get() const noexcept {
    return const_cast<this_t *>(this)->Get<RemainingSteps, ChildIndex>();
  }


  /*
   * Is the Query type contained in the subtree?
   */
  template<typename Query>
  static constexpr bool HasType() noexcept {
    if constexpr ( sizeof...(Rest) > 0 ) {
      return (
        std::is_same<First, Query>() // is this node the Query?
        || First::template HasType<Query>() // does first child contain Query?
        // does a subsequent node contain Query?
        || PodInternalNode<Rest...>::template HasType<Query>()
      );
    } else return (
      std::is_same<First, Query>() // is this node the Query?
      || First::template HasType<Query>() // does first child contain Query?
    );
  }

  /*
   * Get leaf by type.
   * Returns first leaf of Query type.
   */
  template<typename Query, size_t SearchIndex=0>
  constexpr Query& Get() noexcept {

    using Child = typename std::tuple_element<SearchIndex, parent_t>::type;

    if constexpr ( uitsl::tuple_has_type<Query, parent_t>::value ) {
      // if one of immediate children is Query
      return std::get<Query>(*this);
    } else if constexpr ( Child::template HasType<Query>() ) {
      // if is one of child's children
      return std::get<SearchIndex>(*this).template Get<Query>();
    } else { // check the next child
      return Get<Query, SearchIndex + 1>();
    }

  }

  /*
   * Get leaf by type.
   * Returns first leaf with Query type.
   */
  template<typename Query, size_t SearchIndex=0>
  constexpr const Query& Get() const noexcept {
    return const_cast<this_t *>(this)->Get<Query, SearchIndex>();
  }

  /*
   * Set all child nodes to value-initialized state.
   */
  void Reset() noexcept {
    // adapted from https://stackoverflow.com/a/45498003
    std::apply(
      [](auto& ...x){ (..., [](auto& v){ v.Reset(); }(x)); },
      *static_cast<parent_t*>(this)
    );
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_DATASTRUCTS_PODINTERNALNODE_HPP_INCLUDE
