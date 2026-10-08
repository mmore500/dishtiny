#pragma once
#ifndef DISH2_VIZ_UTIL_PCA_BY_GROUP_HPP_INCLUDE
#define DISH2_VIZ_UTIL_PCA_BY_GROUP_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <vector>

#include "../../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../../third-party/header-only-pca/include/hopca/normalize.hpp"
#include "../../../../third-party/header-only-pca/include/hopca/pca.hpp"
#include "../../../../third-party/header-only-pca/include/hopca/types.hpp"
#include "../../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

namespace dish2 {

// performs a separate PCA for the rows (live cardinals) of each group,
// returns the (up to three) normalized components of each row, zeros if the
// group has no variation to decompose
inline std::vector< std::vector<double> > pca_by_group(
  const hopca::Matrix& summary, const std::vector<size_t>& group_ids
) {

  std::unordered_multimap<size_t, size_t> group_to_row;
  uitsl::for_each(
    std::begin( group_ids ), std::end( group_ids ), sgpl::CountingIterator{},
    [&]( const size_t group_id, const size_t row ){
      group_to_row.emplace( group_id, row );
    }
  );

  std::vector< std::vector<double> > res( group_ids.size(), {0.0, 0.0, 0.0} );

  for ( auto it = std::begin( group_to_row ); it != std::end( group_to_row ); ) {

    const auto [first, last] = group_to_row.equal_range( it->first );
    it = last;

    const size_t num_rows = uitsl::audit_cast<size_t>(
      std::distance( first, last )
    );

    hopca::Matrix group_summary = hola::matrix_new(
      num_rows, summary->n_col
    );
    uitsl::for_each(
      first, last, sgpl::CountingIterator{},
      [&]( const auto& entry, const size_t i ){
        // wrapped so the copy is freed, the raw pointer would be leaked
        const hopca::Vector row = hola::matrix_row_copy(
          summary, entry.second
        );
        hola::matrix_copy_vector_into_row( group_summary, row, i );
      }
    );

    const auto group_expression = hopca::drop_homogenous_columns( group_summary );
    if ( !group_expression.has_value() ) continue;

    hopca::PCA group_pca{ std::min(
      3ul, uitsl::audit_cast<size_t>( group_expression.value()->n_row )
    ) };

    const auto condensed = hopca::drop_homogenous_columns(
      group_pca.doPCA( *group_expression )
    );
    if ( !condensed.has_value() ) continue;

    const hopca::Matrix normalized = hopca::unit_normalize( *condensed );

    uitsl::for_each(
      first, last, sgpl::CountingIterator{},
      [&]( const auto& entry, const size_t i ){
        const hopca::Vector v = hola::matrix_row_copy( normalized, i );
        res[ entry.second ].assign( DATA(v), DATA(v) + v->length );
      }
    );

  }

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_VIZ_UTIL_PCA_BY_GROUP_HPP_INCLUDE
