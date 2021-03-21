#pragma once
#ifndef DISH2_WEB_DATA_PILLS_MEANEPOCHPILL_HPP_INCLUDE
#define DISH2_WEB_DATA_PILLS_MEANEPOCHPILL_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../introspection/get_mean_epoch.hpp"
#include "../../world/ThreadWorld.hpp"

#include "../DataPill.hpp"

namespace dish2 {

template< typename Spec >
class MeanEpochPill {

  static auto DoWork( const ThreadWorld<Spec>& world ) {
    return dish2::get_mean_epoch<Spec>( world );
  }

public:

  static emp::web::Div Make( const ThreadWorld<Spec>& world ) {
    return dish2::DataPill(
      "Mean Epoch",
      [&world](){ return emp::to_string( DoWork( world ) ); },
      "TODO"
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_DATA_PILLS_MEANEPOCHPILL_HPP_INCLUDE
