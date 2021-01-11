#pragma once
#ifndef DISH2_WEB_INSTRUCTIONLISTENTRYITEM_HPP_INCLUDE
#define DISH2_WEB_INSTRUCTIONLISTENTRYITEM_HPP_INCLUDE

#include <functional>
#include <string>
#include <type_traits>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/tools/hash_namify.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/Empirical/include/emp/web/commands.hpp"
#include "../../../third-party/Empirical/include/emp/web/Div.hpp"
#include "../../../third-party/Empirical/include/emp/web/init.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"

namespace dish2 {

template< typename Spec >
class InstructionListEntryItem {

  emp::web::Div list_group_item;

  using sgpl_spec_t = typename Spec::sgpl_spec_t;
  using instruction_t = sgpl::Instruction< sgpl_spec_t >;

  inline static std::unordered_map< std::string, std::string > color_classes{
    {"Global Anchor", "list-group-item-primary"},
    {"Local Anchor", "list-group-item-secondary"}
  };

  emp::web::Div make_badges(
    const size_t program_idx, const emp::optional<size_t> module_idx
  ) {
    emp::web::Div res;
    if ( module_idx.has_value() ) res << emp::web::Div().SetAttr(
      "class", "badge badge-primary badge-pill m-1"
    ).SetCSS(
      "border", "0px"
    ) << "module " << *module_idx;
    res << emp::web::Div().SetAttr(
      "class", "badge badge-secondary badge-pill m-1"
    ).SetCSS(
      "border", "0px"
    ) << program_idx;
    return res;
  }

public:

  InstructionListEntryItem(
    const instruction_t& instruction,
    const size_t program_idx, const size_t module_idx
  ) {

    list_group_item.SetAttr(
      "class",
      "list-group-item list-group-item-action"
        " d-flex justify-content-between align-items-center"
    ).SetCSS(
      "cursor", "pointer"
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

    list_group_item << make_badges(
      program_idx,
      instruction.GetOpName() == "Global Anchor"
        ? emp::optional<size_t>{ module_idx }
        : std::nullopt
      );

  }

  operator emp::web::Div&() { return list_group_item; }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_INSTRUCTIONLISTENTRYITEM_HPP_INCLUDE
