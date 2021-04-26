#pragma once
#ifndef DISH2_WEB_DATA_PILLS_NUMUNIQUEMODULEEXPRESSIONPROFILESPILL_HPP_INCLUDE
#define DISH2_WEB_DATA_PILLS_NUMUNIQUEMODULEEXPRESSIONPROFILESPILL_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../introspection/count_unique_module_expression_profiles.hpp"
#include "../../world/ThreadWorld.hpp"

#include "../DataPill.hpp"

namespace dish2 {

template< typename Spec >
class NumUniqueModuleExpressionProfilesPill {

  static auto DoWork( const ThreadWorld<Spec>& world ) {
    return dish2::count_unique_module_expression_profiles<Spec>( world );
  }

public:

  static emp::web::Div Make( const ThreadWorld<Spec>& world ) {
    return dish2::DataPill(
      "Num Unique Module Expression Profiles",
      [&world](){ return emp::to_string( DoWork( world ) ); },
      "TODO"
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_DATA_PILLS_NUMUNIQUEMODULEEXPRESSIONPROFILESPILL_HPP_INCLUDE
