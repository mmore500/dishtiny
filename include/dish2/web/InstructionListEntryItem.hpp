#pragma once
#ifndef DISH2_WEB_INSTRUCTIONLISTENTRYITEM_HPP_INCLUDE
#define DISH2_WEB_INSTRUCTIONLISTENTRYITEM_HPP_INCLUDE

#include <functional>
#include <string>
#include <type_traits>

#include "../../../third-party/Empirical/source/tools/string_utils.h"
#include "../../../third-party/Empirical/source/tools/hash_namify.h"
#include "../../../third-party/Empirical/source/web/commands.h"
#include "../../../third-party/Empirical/source/web/Div.h"
#include "../../../third-party/Empirical/source/web/init.h"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"

namespace dish2 {

template< typename Spec >
struct InstructionListEntryItem {

  emp::web::Div list_group_item;

  using sgpl_spec_t = typename Spec::sgpl_spec_t;
  using instruction_t = sgpl::Instruction< sgpl_spec_t >;

  inline static std::unordered_map< std::string, std::string > color_classes{
    {"Global Anchor", "list-group-item-primary"},
    {"Local Anchor", "list-group-item-secondary"}
  };

  InstructionListEntryItem( const instruction_t& instruction ) {

    list_group_item.SetAttr(
      "class", "list-group-item  list-group-item-action"
    );

    // TODO use AddAttr from sara boyd's branch
    list_group_item.SetAttr(
      "class", list_group_item.GetAttr(
        "class"
      ) + " " + color_classes[ instruction.GetOpName() ],
      "data-toggle", "collapse",
      "data-target", emp::to_string( ".instruction-detail-", &instruction )
    );

    list_group_item << instruction.GetOpName();

  }

  operator emp::web::Div&() { return list_group_item; }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_INSTRUCTIONLISTENTRYITEM_HPP_INCLUDE
