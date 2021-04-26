#pragma once
#ifndef DISH2_WEB_PREVALENTGENOTYPEPANEL_HPP_INCLUDE
#define DISH2_WEB_PREVALENTGENOTYPEPANEL_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/Empirical/include/emp/web/Div.hpp"
#include "../../../third-party/Empirical/include/emp/web/Document.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/enumerate_module_ids.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../algorithm/make_battleship_phenotype_equivalent_nopout.hpp"
#include "../algorithm/make_jenga_phenotype_equivalent_nopout.hpp"
#include "../debug/LogLevelGuard.hpp"
#include "../introspection/get_prevalent_coding_genotype.hpp"
#include "../prefab/ModalGuard.hpp"

#include "InstructionListDetailItem.hpp"
#include "InstructionListEntryItem.hpp"
#include "InstructionListNopOutItem.hpp"
#include "InstructionListRefreshItem.hpp"

namespace dish2 {

template< typename Spec >
class PrevalentGenotypePanel {

  emp::web::Document panel{ "dominant_viewer-body" };

  using instruction_list_detail_item_t
    = dish2::InstructionListDetailItem< Spec >;
  using instruction_list_entry_item_t
    = dish2::InstructionListEntryItem< Spec >;
  using instruction_list_nop_out_item_t = dish2::InstructionListNopOutItem;
  using instruction_list_refresh_item_t = dish2::InstructionListRefreshItem;

  using program_t = typename Spec::program_t;

  const dish2::ThreadWorld< Spec >& thread_world;

  void Redraw( const dish2::Genome< Spec >& genome ) {

    const auto& program = genome.program;

    panel.ClearChildren();

    panel << (emp::web::Div) instruction_list_refresh_item_t{ [this](){
      Redraw();
      // why is this necessary?
      panel.SetAttr( "class", "list-group list-group-flush collapse show" );
    } };

    panel << (emp::web::Div) instruction_list_nop_out_item_t{ [this, genome](){

      const dish2::ModalGuard guard{ "modal-log-readout" };

      const dish2::LogLevelGuard guard2{ 2 };

      Redraw(
        dish2::make_battleship_phenotype_equivalent_nopout< Spec >(
          genome, "prevalent"
        )
      );
      // why is this necessary?
      panel.SetAttr( "class", "list-group list-group-flush collapse show" );

    } };

    panel << (emp::web::Div) instruction_list_nop_out_item_t{ [this, genome](){

      const dish2::ModalGuard guard{ "modal-log-readout" };

      const dish2::LogLevelGuard guard2{ 2 };

      Redraw(
        dish2::make_jenga_phenotype_equivalent_nopout< Spec >(
          genome, "prevalent"
        )
      );
      // why is this necessary?
      panel.SetAttr( "class", "list-group list-group-flush collapse show" );

    } };

    const auto enumerated_module_ids = sgpl::enumerate_module_ids( program );

    uitsl::for_each(
      std::begin( program ),
      std::end( program ),
      sgpl::CountingIterator{},
      std::begin( enumerated_module_ids ),
      [this](
        const auto& inst, const size_t program_idx, const size_t module_idx
      ) {
        panel << (emp::web::Div) instruction_list_entry_item_t{
          inst, program_idx, module_idx
        };
        panel << (emp::web::Div) instruction_list_detail_item_t{ inst };
      }
    );

  }

public:

  PrevalentGenotypePanel(
    const dish2::ThreadWorld< Spec >& thread_world_
  ) : thread_world( thread_world_ ) {

    panel.SetAttr(
      "class", "list-group list-group-flush collapse"
    );

    Redraw();

  }

  void Redraw() {
    const auto& [coding_genotype, count]
      = dish2::get_prevalent_coding_genotype<Spec>( thread_world );
    const auto& [event_tags, program] = coding_genotype;

    dish2::Genome< Spec > genome;
    genome.program = program;
    genome.event_tags = event_tags;

    Redraw( genome );
  }

};

} // namespace dish2



#endif // #ifndef DISH2_WEB_PREVALENTGENOTYPEPANEL_HPP_INCLUDE
