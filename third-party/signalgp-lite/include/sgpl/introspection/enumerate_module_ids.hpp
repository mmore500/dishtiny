#pragma once
#ifndef SGPL_INTROSPECTION_ENUMERATE_MODULE_IDS_HPP_INCLUDE
#define SGPL_INTROSPECTION_ENUMERATE_MODULE_IDS_HPP_INCLUDE

#include <algorithm>
#include <iterator>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../program/GlobalAnchorIterator.hpp"
#include "../program/Program.hpp"
#include "../utility/CountingIterator.hpp"

namespace sgpl {

template<typename Spec>
emp::vector<size_t> enumerate_module_ids( const sgpl::Program<Spec>& program ) {

  emp::vector< size_t > res;

  if ( program.empty() ) return res;

  uitsl::for_each(
    std::next( sgpl::GlobalAnchorIterator<Spec>::make_begin( program ) ),
    sgpl::GlobalAnchorIterator<Spec>::make_end( program ),
    sgpl::GlobalAnchorIterator<Spec>::make_begin( program ),
    sgpl::CountingIterator{},
    [&res](
      const auto& module_back,
      const auto& module_front,
      const size_t module_idx
    ) {
      const size_t module_length = std::distance(&module_front, &module_back);
      std::fill_n( std::back_inserter( res ), module_length, module_idx );
    }
  );

  // fill in last module
  res.resize( program.size() , res.size() ? res.back() + 1 : 0 );

  emp_assert( res.size() == program.size() );

  return res;

}

} // namespace sgpl

#endif // #ifndef SGPL_INTROSPECTION_ENUMERATE_MODULE_IDS_HPP_INCLUDE
