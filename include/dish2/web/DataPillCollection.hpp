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
template< typename Spec, size_t... I >
auto make_data_pill_collection( std::index_sequence<I...> ) {
  return std::tuple<
    dish2::MaximumModuleCountPill<Spec>,
    dish2::MaximumProgramLengthPill<Spec>,
    dish2::MeanElapsedInsertionsDeletionsPill<Spec>,
    dish2::MeanElapsedMutationOccurencesPill<Spec>,
    dish2::MeanElapsedPointMutationsPill<Spec>,
    dish2::MeanEpochPill<Spec>,
    dish2::MeanModuleCountPill<Spec>,
    dish2::MeanProgramLengthPill<Spec>,
    dish2::NumLiveCellsPill<Spec>,
    dish2::NumPhylogeneticRootsPill<Spec>,
    dish2::NumUniqueGenotypesPill<Spec>,
    dish2::NumUniqueModuleExpressionProfilesPill<Spec>,
    dish2::NumUniqueModuleRegulationProfilesPill<Spec>,
    dish2::PrevalentGenotypeCountPill<Spec>,
    dish2::ElapsedGenerationsPill<Spec, I>...
  >{};
}

} // namespace internal

template< typename Spec >
using DataPillCollection = decltype(
  internal::make_data_pill_collection<Spec>(
    std::make_index_sequence< Spec::NLEV + 1>{}
  )
);


} // namespace dish2

#endif // #ifndef DISH2_WEB_DATAPILLCOLLECTION_HPP_INCLUDE
