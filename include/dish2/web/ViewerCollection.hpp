#pragma once
#ifndef DISH2_WEB_VIEWERCOLLECTION_HPP_INCLUDE
#define DISH2_WEB_VIEWERCOLLECTION_HPP_INCLUDE

#include <tuple>

#include "../viz/artists/_index.hpp"

#include "GridViewer.hpp"

namespace dish2 {

using ViewerCollection = std::tuple<
  dish2::GridViewer<dish2::IsAliveArtist<>>,
  dish2::GridViewer<dish2::CellBirthArtist<>>,
  dish2::GridViewer<dish2::KinGroupIDArtist<>>,
  dish2::GridViewer<dish2::KinMatchArtist<>>,
  dish2::GridViewer<dish2::NeighborKinGroupIDViewArtist<>>,
  dish2::GridViewer<dish2::PhylogeneticRootArtist<>>,
  dish2::GridViewer<dish2::QuorumBitsArtist<>>,
  dish2::GridViewer<dish2::ResourceStockpileArtist<>>
>;

} // namespace dish2

#endif // #ifndef DISH2_WEB_VIEWERCOLLECTION_HPP_INCLUDE
