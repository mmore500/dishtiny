#pragma once
#ifndef DISH2_WEB_DATA_PILLS_NUMUNIQUEMODULEEXPRESSIONPROFILESPILL_HPP_INCLUDE
#define DISH2_WEB_DATA_PILLS_NUMUNIQUEMODULEEXPRESSIONPROFILESPILL_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../introspection/count_unique_module_expression_profiles.hpp"
#include "../../world/ThreadWorld.hpp"
#include "../../spec/Spec.hpp"

#include "../DataPill.hpp"

namespace dish2 {

class NumUniqueModuleExpressionProfilesPill {

  static auto DoWork( const ThreadWorld<dish2::Spec>& world ) {
    return dish2::count_unique_module_expression_profiles<dish2::Spec>( world );
  }

public:

  static emp::web::Div Make( const ThreadWorld<dish2::Spec>& world ) {
    return dish2::DataPill(
      "Num Unique Module Expression Profiles",
      [&world](){ return emp::to_string( DoWork( world ) ); },
      "TODO"
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_DATA_PILLS_NUMUNIQUEMODULEEXPRESSIONPROFILESPILL_HPP_INCLUDE
