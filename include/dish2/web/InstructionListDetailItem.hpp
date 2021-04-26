#pragma once
#ifndef DISH2_WEB_INSTRUCTIONLISTDETAILITEM_HPP_INCLUDE
#define DISH2_WEB_INSTRUCTIONLISTDETAILITEM_HPP_INCLUDE

#include <functional>
#include <string>
#include <type_traits>

#include "../../../third-party/Empirical/include/emp/tools/hash_namify.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/Empirical/include/emp/web/commands.hpp"
#include "../../../third-party/Empirical/include/emp/web/Div.hpp"
#include "../../../third-party/Empirical/include/emp/web/init.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"

namespace dish2 {

template< typename Spec >
class InstructionListDetailItem {

  emp::web::Div list_group_item;

  using sgpl_spec_t = typename Spec::sgpl_spec_t;
  using instruction_t = sgpl::Instruction< sgpl_spec_t >;

  static emp::web::Div MakeDetail(
    const std::string& key, const std::string& value
  ) {

    emp::web::Div res;

    res.SetAttr(
      "class", "list-group list-group-horizontal"
    ).SetCSS(
      "display", "flex",
      "flex-wrap", "wrap"
    );

    res << emp::web::Div(
    ).SetAttr(
      "class", "list-group-item border list-group-item-secondary"
    ).SetCSS(
      "flex", "1 1 200px"
    ) << key;

    res << emp::web::Div(
    ).SetAttr(
      "class", "list-group-item border"
    ).SetCSS(
      "flex", "1 1 200px"
    ) << value;

    return res;

  }

public:

  InstructionListDetailItem( const instruction_t& instruction ) {

    list_group_item.SetAttr(
      "class",
      emp::to_string(
        "collapse list-group-item instruction-detail-", &instruction
      )
    );

      for (const auto& [key, value] : instruction.GetDescriptors() ) {
        list_group_item << MakeDetail( key, value );
      }

  }

  operator emp::web::Div&() { return list_group_item; }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_INSTRUCTIONLISTDETAILITEM_HPP_INCLUDE
