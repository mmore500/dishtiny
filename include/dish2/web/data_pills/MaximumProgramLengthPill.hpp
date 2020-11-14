#pragma once
#ifndef DISH2_WEB_DATA_PILLS_MAXIMUMPROGRAMLENGTHPILL_HPP_INCLUDE
#define DISH2_WEB_DATA_PILLS_MAXIMUMPROGRAMLENGTHPILL_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/tools/string_utils.h"

#include "../../introspection/get_maximum_program_length.hpp"
#include "../../world/ThreadWorld.hpp"
#include "../../spec/Spec.hpp"

#include "../DataPill.hpp"

namespace dish2 {

class MaximumProgramLengthPill {

  static auto DoWork( const ThreadWorld<dish2::Spec>& world ) {
    return dish2::get_maximum_program_length<dish2::Spec>( world );
  }

public:

  static emp::web::Div Make( const ThreadWorld<dish2::Spec>& world ) {
    return dish2::DataPill(
      "Maximum Program Length",
      [&world](){ return emp::to_string( DoWork( world ) ); },
      "TODO"
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_DATA_PILLS_MAXIMUMPROGRAMLENGTHPILL_HPP_INCLUDE
