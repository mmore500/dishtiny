#pragma once
#ifndef DISH2_WEB_INSTRUCTIONLISTGROUPITEM_HPP_INCLUDE
#define DISH2_WEB_INSTRUCTIONLISTGROUPITEM_HPP_INCLUDE

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
struct InstructionListGroupItem {

  emp::web::Div list_group_item;

  using sgpl_spec_t = typename Spec::sgpl_spec_t;
  using instruction_t = sgpl::Instruction< sgpl_spec_t >;

  InstructionListGroupItem( const instruction_t instruction ) {

    list_group_item.SetAttr(
      "class", "list-group-item"
    );

    list_group_item << instruction.GetOpName();
    list_group_item << emp::web::endl;
    if ( instruction.ShouldPrintTag() ) list_group_item << emp::hash_namify(
      std::hash< std::decay_t< decltype( instruction.GetTag() ) > >{}(
        instruction.GetTag()
      )
    );

  }

  operator emp::web::Div&() { return list_group_item; }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_INSTRUCTIONLISTGROUPITEM_HPP_INCLUDE
