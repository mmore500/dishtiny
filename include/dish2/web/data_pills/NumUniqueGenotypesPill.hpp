#pragma once
#ifndef DISH2_WEB_DATA_PILLS_NUMUNIQUEGENOTYPESPILL_HPP_INCLUDE
#define DISH2_WEB_DATA_PILLS_NUMUNIQUEGENOTYPESPILL_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../introspection/count_unique_coding_genotypes.hpp"
#include "../../world/ThreadWorld.hpp"

#include "../DataPill.hpp"

namespace dish2 {

template< typename Spec >
class NumUniqueGenotypesPill {

  static auto DoWork( const ThreadWorld<Spec>& world ) {
    return dish2::count_unique_coding_genotypes<Spec>( world );
  }

public:

  static emp::web::Div Make( const ThreadWorld<Spec>& world ) {
    return dish2::DataPill(
      "Num Unique Genotypes",
      [&world](){ return emp::to_string( DoWork( world ) ); },
      "TODO"
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_DATA_PILLS_NUMUNIQUEGENOTYPESPILL_HPP_INCLUDE
