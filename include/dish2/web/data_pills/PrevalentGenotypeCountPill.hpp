#pragma once
#ifndef DISH2_WEB_DATA_PILLS_PREVALENTGENOTYPECOUNTPILL_HPP_INCLUDE
#define DISH2_WEB_DATA_PILLS_PREVALENTGENOTYPECOUNTPILL_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../introspection/get_prevalent_coding_genotype.hpp"
#include "../../world/ThreadWorld.hpp"

#include "../DataPill.hpp"

namespace dish2 {

template< typename Spec >
class PrevalentGenotypeCountPill {

  static auto DoWork( const ThreadWorld<Spec>& world ) {
    const auto& [genotype, count]
      = dish2::get_prevalent_coding_genotype<Spec>( world );
    return count;
  }

public:

  static emp::web::Div Make( const ThreadWorld<Spec>& world ) {
    return dish2::DataPill(
      "Prevalent Genotype Count",
      [&world](){ return emp::to_string( DoWork( world ) ); },
      "TODO"
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_DATA_PILLS_PREVALENTGENOTYPECOUNTPILL_HPP_INCLUDE
