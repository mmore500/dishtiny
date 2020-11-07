#pragma once
#ifndef DISH2_WEB_VIEWERCOLLECTION_HPP_INCLUDE
#define DISH2_WEB_VIEWERCOLLECTION_HPP_INCLUDE

#include <tuple>

#include "../viz/artists/_index.hpp"

#include "GridViewer.hpp"
#include "ViewerManager.hpp"
#include "viewer_categories/_index.hpp"

namespace dish2 {

using ViewerCollection = dish2::ViewerManager<
  dish2::GridViewer<dish2::CellBirthArtist<>, dish2::ApoptosisCategory>,
  dish2::GridViewer<dish2::EpochArtist<>, dish2::DemographicsCategory>,
  dish2::GridViewer<dish2::HeirRequestArtist<>, dish2::SharingCategory>,
  dish2::GridViewer<
    dish2::IncomingInterMessageCounterArtist<>, dish2::MessagingCategory
  >,
  dish2::GridViewer<dish2::IsAliveArtist<>, dish2::DemographicsCategory>,
  dish2::GridViewer<dish2::KinGroupAgeArtist<>, dish2::DemographicsCategory>,
  dish2::GridViewer<
    dish2::KinGroupIDArtist<>, dish2::GroupStructureCategory, true
  >,
  dish2::GridViewer<
    dish2::KinGroupIDViewArtist<>, dish2::GroupStructureCategory
  >,
  dish2::GridViewer<dish2::KinMatchArtist<>, dish2::GroupStructureCategory>,
  dish2::GridViewer<
    dish2::LearnedQuorumBitsArtist<>, dish2::GroupStructureCategory
  >,
  dish2::GridViewer<
    dish2::NeighborKinGroupIDViewArtist<>, dish2::GroupStructureCategory
  >,
  dish2::GridViewer<
    dish2::PhylogeneticRootArtist<>, dish2::DemographicsCategory
  >,
  dish2::GridViewer<
    dish2::QuorumBitsArtist<>, dish2::GroupStructureCategory
  >,
  dish2::GridViewer<
    dish2::ResourceInputPeekArtist<>, dish2::SharingCategory
  >,
  dish2::GridViewer<
    dish2::ResourceStockpileArtist<>, dish2::SharingCategory
  >
>;

} // namespace dish2

#endif // #ifndef DISH2_WEB_VIEWERCOLLECTION_HPP_INCLUDE
