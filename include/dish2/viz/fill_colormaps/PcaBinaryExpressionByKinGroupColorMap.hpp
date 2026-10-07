#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_PCABINARYEXPRESSIONBYKINGROUPCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_PCABINARYEXPRESSIONBYKINGROUPCOLORMAP_HPP_INCLUDE

#include <algorithm>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../../../../third-party/conduit/include/uitsl/algorithm/clamp_cast.hpp"
#include "../../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../../third-party/Empirical/include/emp/datastructs/tuple_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/header-only-pca/include/hopca/normalize.hpp"
#include "../../../../third-party/header-only-pca/include/hopca/pca.hpp"

#include "../../introspection/count_live_cells.hpp"
#include "../../introspection/get_live_cardinal_kin_group_ids.hpp"
#include "../../introspection/make_cardi_coord_to_live_cardi_idx_translator.hpp"
#include "../../introspection/summarize_module_expression.hpp"
#include "../util/pca_by_group.hpp"
#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
class PcaBinaryExpressionByKinGroupColorMap {

  std::reference_wrapper< const dish2::ThreadWorld<Spec> > thread_world;
  size_t lev;

  std::unordered_map<
    std::tuple<size_t, size_t>, size_t, emp::TupleHash<size_t, size_t>
  > cardi_coord_to_live_cardi_idx_translator;
  std::vector< std::vector<double> > pca_result;

public:

  template<typename... Args>
  PcaBinaryExpressionByKinGroupColorMap(
    const dish2::ThreadWorld<Spec>& thread_world_,
    const size_t lev_,
    Args&&...
  ) : thread_world( thread_world_ ), lev( lev_ ) {
    Refresh();
  }

  template<typename ValueType>
  std::string Paint(const ValueType& cardi_coord) const {
    if ( cardi_coord_to_live_cardi_idx_translator.count(
      cardi_coord
    ) == 0 ) return "transparent";

    const size_t live_cardi_idx = cardi_coord_to_live_cardi_idx_translator.at(
      cardi_coord
    );
    const auto& data = pca_result[ live_cardi_idx ];

    return emp::ColorRGB(
      255 - std::clamp( uitsl::clamp_cast<int>( data[0] * 255.0 ), 0, 255 ),
      data.size() > 1
        ? 255 - std::clamp( uitsl::clamp_cast<int>( data[1] * 255.0 ), 0, 255 )
        : 0
      ,
      data.size() > 2
        ? 255 - std::clamp( uitsl::clamp_cast<int>( data[2] * 255.0 ), 0, 255 )
        : 0
    );

  }

  void Refresh() {

    cardi_coord_to_live_cardi_idx_translator
      = dish2::make_cardi_coord_to_live_cardi_idx_translator< Spec >(
        thread_world.get()
      );

    hopca::Matrix summary = dish2::summarize_module_expression(
      thread_world.get()
    );
    std::transform(
      DATA( summary ),
      DATA( summary ) + summary->n_row * summary->n_col,
      DATA( summary ),
      []( const double val ){ return val > 0.0; }
    );

    pca_result = dish2::pca_by_group(
      summary, dish2::get_live_cardinal_kin_group_ids<Spec>( thread_world.get(), lev )
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_PCABINARYEXPRESSIONBYKINGROUPCOLORMAP_HPP_INCLUDE
