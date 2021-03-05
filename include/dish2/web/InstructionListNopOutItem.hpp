#pragma once
#ifndef DISH2_WEB_INSTRUCTIONLISTNOPOUTITEM_HPP_INCLUDE
#define DISH2_WEB_INSTRUCTIONLISTNOPOUTITEM_HPP_INCLUDE

#include <functional>

#include "../../../third-party/Empirical/include/emp/tools/hash_namify.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/Empirical/include/emp/web/commands.hpp"
#include "../../../third-party/Empirical/include/emp/web/Div.hpp"
#include "../../../third-party/Empirical/include/emp/web/init.hpp"

namespace dish2 {

class InstructionListNopOutItem {

  emp::web::Div list_group_item;

public:

  InstructionListNopOutItem( std::function<void()> callback ) {

#ifdef __EMSCRIPTEN_PTHREADS__
    list_group_item.SetAttr(
      "class",
      "list-group-item list-group-item-action list-group-item-danger"
    ).SetCSS(
      "cursor", "pointer"
    ).OnClick(
      [ callback ](){ callback(); }
    ) << "Do Nop Out";
#else
    list_group_item.SetAttr(
      "class",
      "list-group-item list-group-item-action list-group-item-danger disabled"
    ) << "Phenotype-neutral Nop Out is not available in your browser.";
#endif
  }

  operator emp::web::Div&() { return list_group_item; }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_INSTRUCTIONLISTNOPOUTITEM_HPP_INCLUDE
