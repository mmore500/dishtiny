#pragma once
#ifndef DISH2_WEB_DATA_PILLS_ELAPSEDGENERATIONSPILL_HPP_INCLUDE
#define DISH2_WEB_DATA_PILLS_ELAPSEDGENERATIONSPILL_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../introspection/get_mean_generation.hpp"
#include "../../world/ThreadWorld.hpp"

#include "../DataPill.hpp"

namespace dish2 {

template<typename Spec, size_t Lev>
class ElapsedGenerationsPill {

  static auto DoWork(const ThreadWorld<Spec>& world) {
    return dish2::get_mean_generation<Spec>( world, Lev );
  }

public:

  static emp::web::Div Make( const ThreadWorld<Spec>& world ) {
    return dish2::DataPill(
      Lev
        ? emp::to_string("Mean Elapsed Group Generation ", Lev)
        : "Mean Elapsed Cell Generation"
      ,
      [&world](){ return emp::to_string( DoWork( world) ); },
      "TODO"
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_DATA_PILLS_ELAPSEDGENERATIONSPILL_HPP_INCLUDE
