#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_PCAEXPRESSIONCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_PCAEXPRESSIONCOLORMAP_HPP_INCLUDE

#include <algorithm>
#include <functional>
#include <tuple>
#include <unordered_map>

#include "../../../../third-party/conduit/include/uitsl/algorithm/clamp_cast.hpp"
#include "../../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../../third-party/Empirical/include/emp/datastructs/tuple_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/header-only-pca/include/hopca/normalize.hpp"
#include "../../../../third-party/header-only-pca/include/hopca/pca.hpp"

#include "../../introspection/count_live_cells.hpp"
#include "../../introspection/make_cardi_coord_to_live_cardi_idx_translator.hpp"
#include "../../introspection/summarize_module_expression.hpp"
#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
class PcaExpressionColorMap {

  std::reference_wrapper< const dish2::ThreadWorld<Spec> > thread_world;

  std::unordered_map<
    std::tuple<size_t, size_t>, size_t, emp::TupleHash<size_t, size_t>
  > cardi_coord_to_live_cardi_idx_translator;
  emp::optional<hopca::Matrix> pca_result;

public:

  template<typename... Args>
  PcaExpressionColorMap(
    const dish2::ThreadWorld<Spec>& thread_world_,
    Args&&...
  ) : thread_world( thread_world_ ) {
    Refresh();
  }

  template<typename ValueType>
  std::string Paint(const ValueType& cardi_coord) const {
    if ( cardi_coord_to_live_cardi_idx_translator.count(
      cardi_coord
    ) == 0 ) return "transparent";
    else if ( !pca_result.has_value() ) return "transparent";

    const size_t live_cardi_idx = cardi_coord_to_live_cardi_idx_translator.at(
      cardi_coord
    );
    hopca::Vector v = hola::matrix_row_copy( *pca_result, live_cardi_idx );
    const auto data = DATA(v);

    emp_assert( std::clamp( v->length, 1, 3 ) == v->length );

    return emp::ColorRGB(
      255 - std::clamp( uitsl::clamp_cast<int>( data[0] * 255.0 ), 0, 255 ),
      v->length >= 1
        ? 255 - std::clamp( uitsl::clamp_cast<int>( data[1] * 255.0 ), 0, 255 )
        : 0
      ,
      v->length >= 2
        ? 255 - std::clamp( uitsl::clamp_cast<int>( data[2] * 255.0 ), 0, 255 )
        : 0
    );

  }

  void Refresh() {

    cardi_coord_to_live_cardi_idx_translator
      = dish2::make_cardi_coord_to_live_cardi_idx_translator< Spec >(
        thread_world.get()
      );

    pca_result.reset();

    const auto expression_summary = hopca::drop_homogenous_columns(
      dish2::summarize_module_expression(
        thread_world.get()
      )
    );

    if ( !expression_summary.has_value() ) return;

    hopca::PCA module_expression_pca{ std::min(
      3ul, uitsl::audit_cast<size_t>( expression_summary.value()->n_row )
    ) };

    const auto condensed_summary = hopca::drop_homogenous_columns(
      module_expression_pca.doPCA( *expression_summary )
    );

    if ( !condensed_summary.has_value() ) return;

    pca_result = hopca::unit_normalize( *condensed_summary );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_PCAEXPRESSIONCOLORMAP_HPP_INCLUDE
