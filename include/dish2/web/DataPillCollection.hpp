#pragma once
#ifndef DISH2_WEB_DATAPILLCOLLECTION_HPP_INCLUDE
#define DISH2_WEB_DATAPILLCOLLECTION_HPP_INCLUDE

#include <cstddef>
#include <tuple>
#include <utility>

#include "data_pills/_index.hpp"

namespace dish2 {

namespace internal {

// adapted from https://en.cppreference.com/w/cpp/utility/integer_sequence
template< size_t... I >
auto make_data_pill_collection( std::index_sequence<I...> ) {
  return std::tuple<
    dish2::MaximumModuleCountPill,
    dish2::MaximumProgramLengthPill,
    dish2::MeanEpochPill,
    dish2::MeanModuleCountPill,
    dish2::MeanProgramLengthPill,
    dish2::NumLiveCellsPill,
    dish2::NumPhylogeneticRootsPill,
    dish2::NumUniqueGenotypesPill,
    dish2::ElapsedGenerationsPill<I>...
  >{};
}

} // namespace internal

using DataPillCollection = decltype(
  internal::make_data_pill_collection(
    std::make_index_sequence< dish2::Spec::NLEV + 1>{}
  )
);


} // namespace dish2

#endif // #ifndef DISH2_WEB_DATAPILLCOLLECTION_HPP_INCLUDE
