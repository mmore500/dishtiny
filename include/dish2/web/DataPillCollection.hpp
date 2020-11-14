#pragma once
#ifndef DISH2_WEB_DATAPILLCOLLECTION_HPP_INCLUDE
#define DISH2_WEB_DATAPILLCOLLECTION_HPP_INCLUDE

#include <tuple>

#include "data_pills/_index.hpp"

namespace dish2 {

using DataPillCollection = std::tuple<
  dish2::MeanEpochPill,
  dish2::NumPhylogeneticRootsPill
>;

} // namespace dish2

#endif // #ifndef DISH2_WEB_DATAPILLCOLLECTION_HPP_INCLUDE
