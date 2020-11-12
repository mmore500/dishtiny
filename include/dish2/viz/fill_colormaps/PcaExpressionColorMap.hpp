#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_PCAEXPRESSIONCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_PCAEXPRESSIONCOLORMAP_HPP_INCLUDE

#include <algorithm>
#include <functional>
// #include <unordered_map> // TODO fixme
#include <map>

#include "../../../../third-party/conduit/include/uitsl/algorithm/clamp_cast.hpp"
#include "../../../../third-party/Empirical/source/tools/math.h"
#include "../../../../third-party/Empirical/source/tools/math.h"
#include "../../../../third-party/header-only-pca/include/hopca/pca.hpp"

#include "../../introspection/make_cardi_coord_to_live_cardi_idx_translator.hpp"
#include "../../introspection/summarize_module_expression.hpp"
#include "../../world/ThreadWorld.hpp"

namespace dish2 {

struct PcaExpressionColorMap {

  std::reference_wrapper< const dish2::ThreadWorld<Spec> > thread_world;

  std::map< // todo fixme
    std::tuple<size_t, size_t>, size_t
  > cardi_coord_to_live_cardi_idx_translator;
  hopca::Matrix pca_result{};

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
    else {
      const size_t live_cardi_idx = cardi_coord_to_live_cardi_idx_translator.at(
        cardi_coord
      );
      hopca::Vector v = hola::matrix_column_copy( pca_result, live_cardi_idx );
      const auto data = DATA(v);

      return emp::ColorRGB(
        std::clamp( uitsl::clamp_cast<int>( data[0] * 255.0 ), 0, 255 ),
        std::clamp( uitsl::clamp_cast<int>( data[1] * 255.0 ), 0, 255 ),
        std::clamp( uitsl::clamp_cast<int>( data[2] * 255.0 ), 0, 255 )
      );

    }
  }

  void Refresh() {
    const auto expression_summary = dish2::summarize_module_expression(
      thread_world.get()
    );
    cardi_coord_to_live_cardi_idx_translator
      = dish2::make_cardi_coord_to_live_cardi_idx_translator< Spec >(
        thread_world.get()
      );

    hopca::PCA module_expression_pca{ 3 };

    pca_result = module_expression_pca.doPCA( expression_summary );


  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_PCAEXPRESSIONCOLORMAP_HPP_INCLUDE
