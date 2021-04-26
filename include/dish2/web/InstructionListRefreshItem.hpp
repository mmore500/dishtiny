#pragma once
#ifndef DISH2_WEB_INSTRUCTIONLISTREFRESHITEM_HPP_INCLUDE
#define DISH2_WEB_INSTRUCTIONLISTREFRESHITEM_HPP_INCLUDE

#include <functional>

#include "../../../third-party/Empirical/include/emp/tools/hash_namify.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/Empirical/include/emp/web/commands.hpp"
#include "../../../third-party/Empirical/include/emp/web/Div.hpp"
#include "../../../third-party/Empirical/include/emp/web/init.hpp"

namespace dish2 {

class InstructionListRefreshItem {

  emp::web::Div list_group_item;

public:

  InstructionListRefreshItem( std::function<void()> callback ) {

    list_group_item.SetAttr(
      "class",
      "list-group-item list-group-item-action list-group-item-success"
    ).SetCSS(
      "cursor", "pointer"
    ).OnClick(
      [ callback ](){ callback(); }
    ) << "Refresh";

  }

  operator emp::web::Div&() { return list_group_item; }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_INSTRUCTIONLISTREFRESHITEM_HPP_INCLUDE
