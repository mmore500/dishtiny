#pragma once
#ifndef DISH2_WEB_DATAPILL_HPP_INCLUDE
#define DISH2_WEB_DATAPILL_HPP_INCLUDE

#include "../../../third-party/Empirical/source/web/commands.h"
#include "../../../third-party/Empirical/source/web/Div.h"
#include "../../../third-party/Empirical/source/tools/string_utils.h"

namespace dish2 {

struct DataPill {

  // const std::string & title;
  // const std::function<std::string()> value;
  // const std::string & description;
  // emp::web::Document & parent;

  emp::web::Div pill;

  DataPill(
    const std::string & title,
    const std::function<std::string()> value,
    const std::string & description,
    const bool split=true
  ) {

    pill.SetAttr(
      "class", split ? "col-md-6 p-3" : "col-md-6 col-lg-4 col-xl-3 p-3"
    );

    pill << emp::web::Div(
      ).SetAttr(
        "class", "card"
      ) << emp::web::Div(
        emp::to_string("datapill-header-", emp::slugify( title ))
      ).SetAttr(
        "class", "card-header"
      ) << emp::web::Div(
        emp::to_string("datapill-wrapper-", emp::slugify( title ))
      ).SetAttr(
        "data-toggle", "collapse",
        "href", emp::to_string("#datapill-collapse-", emp::slugify( title ))
      ) << emp::web::Div(
        emp::to_string("datapill-wrapper2-", emp::slugify( title ))
      ).OnClick(
        [](){
          // static bool active = false;
          // active = !active;
          // if (active) {
          //   parent.Div("datapill-active").SetCSS(
          //     "class", "btn w-100 btn-primary border-secondary active"
          //   ).SetAttr(
          //     "aria-pressed", "true"
          //   );
          // } else {
          //   parent.Div("datapill-active").SetCSS(
          //     "class", "btn w-100 btn-primary border-secondary"
          //   ).SetAttr(
          //     "aria-pressed", "false"
          //   );
          // }
        }
      ) << emp::web::Element(
         "button", emp::to_string("datapill-button-", emp::slugify( title ))
      ).SetAttr(
        "class", "btn btn-block btn-primary p-0 border-0",
        "data-toggle", "button"
      ) << emp::web::Div(
        emp::to_string("datapill-btngroup-", emp::slugify( title ))
      ).SetAttr(
        "class", "btn-group w-100",
        "role", "group"
      ) << emp::web::Div(
        emp::to_string("datapill-active-", emp::slugify( title ))
      ).SetAttr(
        "class", "btn w-100 btn-primary border-secondary"
      ).SetCSS(
        "max-width", "75%"
      ) << title << emp::web::Close(
        emp::to_string("datapill-active-", emp::slugify( title ))
      ) << emp::web::Div(
        emp::to_string("datapill-value-", emp::slugify( title ))
      ).SetAttr(
        "class", "badge-light btn w-25 border-secondary"
      ) <<
        value
       << emp::web::Close(
        emp::to_string("datapill-value-", emp::slugify( title ))
      ) << emp::web::Close(
        emp::to_string("datapill-btngroup-", emp::slugify( title ))
      ) << emp::web::Close(
        emp::to_string("datapill-button-", emp::slugify( title ))
      ) << emp::web::Close(
        emp::to_string("datapill-wrapper2-", emp::slugify( title ))
      ) << emp::web::Close(
        emp::to_string("datapill-wrapper-", emp::slugify( title ))
      ) << emp::web::Close(
        emp::to_string("datapill-header-", emp::slugify( title ))
      ) << emp::web::Div(
        emp::to_string("datapill-collapse-", emp::slugify( title ))
      ).SetAttr(
        "class", "card-body collapse"
      ) << description;

  }

  operator emp::web::Div&() { return pill; }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_DATAPILL_HPP_INCLUDE
