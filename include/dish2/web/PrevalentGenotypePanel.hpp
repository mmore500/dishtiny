#pragma once
#ifndef DISH2_WEB_PREVALENTGENOTYPEPANEL_HPP_INCLUDE
#define DISH2_WEB_PREVALENTGENOTYPEPANEL_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/Empirical/source/web/Div.h"
#include "../../../third-party/Empirical/source/web/Document.h"

#include "../introspection/get_prevalent_coding_genotype.hpp"
#include "../spec/Spec.hpp"

#include "InstructionListGroupItem.hpp"

namespace dish2 {

class PrevalentGenotypePanel {

  emp::web::Document panel{ "dominant_viewer-body" };

  using instruction_list_group_item_t
    = dish2::InstructionListGroupItem< dish2::Spec >;

  const dish2::ThreadWorld< dish2::Spec >& thread_world;

public:

  PrevalentGenotypePanel(
    const dish2::ThreadWorld< dish2::Spec >& thread_world_
  ) : thread_world( thread_world_ ) {

    panel.SetAttr(
      "class", "list-group list-group-flush"
    );

    Redraw();

  }

  void Redraw() {

    panel.ClearChildren();

    const auto& [coding_genotype, count]
      = dish2::get_prevalent_coding_genotype<dish2::Spec>( thread_world );
    const auto& [event_tags, program] = coding_genotype;

    std::for_each(
      std::begin( program ),
      std::end( program ),
      [this]( const auto& instruction ) {
        panel << (emp::web::Div) instruction_list_group_item_t{ instruction };
      }
    );

  }

};

} // namespace dish2



#endif // #ifndef DISH2_WEB_PREVALENTGENOTYPEPANEL_HPP_INCLUDE
