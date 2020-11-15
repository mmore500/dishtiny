#pragma once
#ifndef DISH2_WEB_INSTRUCTIONLISTGROUPITEM_HPP_INCLUDE
#define DISH2_WEB_INSTRUCTIONLISTGROUPITEM_HPP_INCLUDE

#include <functional>
#include <string>

#include "../../../third-party/Empirical/source/tools/string_utils.h"
#include "../../../third-party/Empirical/source/web/commands.h"
#include "../../../third-party/Empirical/source/web/Div.h"
#include "../../../third-party/Empirical/source/web/init.h"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"

namespace dish2 {

template< typename Spec >
struct InstructionListGroupItem {

  emp::web::Div list_group_item;

  using sgpl_spec_t = typename Spec::sgpl_spec_t;
  using instruction_t = sgpl::Instruction< sgpl_spec_t >;

  InstructionListGroupItem( const instruction_t instruction ) {

    list_group_item.SetAttr(
      "class", "list-group-item"
    );

    list_group_item << instruction.GetOpName();

  }

  operator emp::web::Div&() { return list_group_item; }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_INSTRUCTIONLISTGROUPITEM_HPP_INCLUDE
