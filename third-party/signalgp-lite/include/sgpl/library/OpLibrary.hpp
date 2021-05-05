#pragma once
#ifndef SGPL_LIBRARY_OPLIBRARY_HPP_INCLUDE
#define SGPL_LIBRARY_OPLIBRARY_HPP_INCLUDE

#include <tuple>

#include "../../../third-party/conduit/include/uitsl/meta/tuple_has_type.hpp"
#include "../../../third-party/conduit/include/uitsl/meta/tuple_index.hpp"

#include "../operations/flow_global/Anchor.hpp"
#include "../operations/flow_local/Anchor.hpp"
#include "../utility/ByteEnumeration.hpp"

#include "OpLookup.hpp"

namespace sgpl {

template<typename... Ops>
struct OpLibrary : public std::tuple<Ops...> {

  using parent_t = std::tuple<Ops...>;

  using this_t = sgpl::OpLibrary<Ops...>;

  inline static sgpl::OpLookup<this_t> lookup_table;

  constexpr static bool IsAnchorLocalOpCode(const size_t op_code) noexcept {

    if constexpr (
      uitsl::tuple_has_type<sgpl::local::Anchor, parent_t>::value
    ) {

      constexpr size_t anchor_local_op_code
        = uitsl::tuple_index<sgpl::local::Anchor, parent_t>::value;
      return op_code == anchor_local_op_code;

    } else return false;

  }

  constexpr static bool IsAnchorGlobalOpCode(const size_t op_code) noexcept {

    if constexpr (
      uitsl::tuple_has_type<sgpl::global::Anchor, parent_t>::value
    ) {

      constexpr size_t anchor_global_op_code
        = uitsl::tuple_index<sgpl::global::Anchor, parent_t>::value;
      return op_code == anchor_global_op_code;

    } else return false;

  }

  static bool IsNopOpCode(const size_t op_code) noexcept {

    return GetOpName( op_code ).rfind("Nop-", 0) == 0;

  }

  constexpr static bool IsAnchorOpCode( const size_t op_code ) noexcept {
    return IsAnchorLocalOpCode( op_code ) || IsAnchorGlobalOpCode( op_code );
  }

  constexpr static size_t GetSize() noexcept {
    return std::tuple_size<parent_t>();
  }

  template<size_t I>
  using Operation = typename std::tuple_element<I, parent_t>::type;

  static std::string GetOpName(const size_t op_code) {
    return decltype( lookup_table )::GetOpName( op_code );
  }

  template< typename Spec >
  static size_t GetOpNumRngTouches(const size_t op_code) {
    return decltype(lookup_table)::template GetOpNumRngTouches<Spec>( op_code );
  }

  static unsigned char GetOpCode(const std::string& op_name) {
    return lookup_table.GetOpCode( op_name );
  }

  static unsigned char GetNopOpCode( const size_t num_rng_touches=0 ) {
    return decltype( lookup_table )::GetNopOpCode( num_rng_touches );
  }

  static size_t GetOpPrevalence(const size_t op_code) {
    return decltype( lookup_table )::GetOpPrevalence( op_code );
  }

  template< typename Instruction >
  static auto GetOpDescriptors(
    const size_t op_code, const Instruction& instruction
  ) {
    return decltype( lookup_table )::GetOpDescriptors(
      op_code, instruction
    );
  }

  template< typename Instruction >
  static auto GetOpCategories(
    const size_t op_code, const Instruction& instruction
  ) {
    return decltype( lookup_table )::GetOpCategories(
      op_code, instruction
    );
  }

};

} // namespace sgpl

#endif // #ifndef SGPL_LIBRARY_OPLIBRARY_HPP_INCLUDE
