#pragma once
#ifndef DISH2_WEB_DATA_PILLS_MAXIMUMPROGRAMLENGTHPILL_HPP_INCLUDE
#define DISH2_WEB_DATA_PILLS_MAXIMUMPROGRAMLENGTHPILL_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../introspection/get_maximum_program_length.hpp"
#include "../../world/ThreadWorld.hpp"

#include "../DataPill.hpp"

namespace dish2 {

template< typename Spec >
class MaximumProgramLengthPill {

  static auto DoWork( const ThreadWorld<Spec>& world ) {
    return dish2::get_maximum_program_length<Spec>( world );
  }

public:

  static emp::web::Div Make( const ThreadWorld<Spec>& world ) {
    return dish2::DataPill(
      "Maximum Program Length",
      [&world](){ return emp::to_string( DoWork( world ) ); },
      "TODO"
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_DATA_PILLS_MAXIMUMPROGRAMLENGTHPILL_HPP_INCLUDE
