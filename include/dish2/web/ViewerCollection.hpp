#pragma once
#ifndef DISH2_WEB_VIEWERCOLLECTION_HPP_INCLUDE
#define DISH2_WEB_VIEWERCOLLECTION_HPP_INCLUDE

#include <tuple>

#include "../viz/artists/_index.hpp"

#include "GridViewer.hpp"
#include "SeriesViewer.hpp"
#include "viewer_categories/_index.hpp"
#include "ViewerManager.hpp"

namespace dish2 {

template<typename Spec>
using ViewerCollection = dish2::ViewerManager<
  dish2::GridViewer<dish2::ApoptosisRequestArtist<>, dish2::ApoptosisCategory>,
  dish2::GridViewer<dish2::CellBirthArtist<>, dish2::ApoptosisCategory>,
  dish2::GridViewer<dish2::EpochArtist<>, dish2::DemographicsCategory>,
  dish2::SeriesViewer<
    dish2::ExpressionByModuleArtist<>, dish2::GroupStructureCategory, 16
  >,
  dish2::GridViewer<dish2::HeirRequestArtist<>, dish2::SharingCategory>,
  dish2::GridViewer<
    dish2::IncomingInterMessageCounterArtist<>, dish2::MessagingCategory
  >,
  dish2::GridViewer<
    dish2::IncomingIntraMessageCounterArtist<>, dish2::MessagingCategory
  >,
  dish2::GridViewer<dish2::IsAliveArtist<>, dish2::DemographicsCategory>,
  dish2::SeriesViewer<
    dish2::IsPeripheralLevArtist<>, dish2::GroupStructureCategory, Spec::NLEV
  >,
  dish2::SeriesViewer<
    dish2::KinGroupAgeArtist<>, dish2::DemographicsCategory, Spec::NLEV
  >,
  dish2::GridViewer<
    dish2::KinGroupIDArtist<>, dish2::GroupStructureCategory, true
  >,
  dish2::SeriesViewer<
    dish2::KinGroupIDLevArtist<>, dish2::GroupStructureCategory, Spec::NLEV
  >,
  dish2::GridViewer<
    dish2::KinGroupIDViewArtist<>, dish2::GroupStructureCategory
  >,
  dish2::GridViewer<dish2::KinMatchArtist<>, dish2::GroupStructureCategory>,
  dish2::SeriesViewer<
    dish2::LearnedQuorumBitsArtist<>, dish2::GroupStructureCategory, Spec::NLEV
  >,
  dish2::GridViewer<
    dish2::NeighborKinGroupIDViewArtist<>, dish2::GroupStructureCategory
  >,
  dish2::GridViewer<
    dish2::NeighborPosArtist<>, dish2::GroupStructureCategory
  >,
  dish2::GridViewer<
    dish2::NumBusyCoresArtist<>, dish2::GroupStructureCategory
  >,
  dish2::GridViewer<
    dish2::NumModulesArtist<>, dish2::GroupStructureCategory
  >,
  dish2::GridViewer<
    dish2::PcaBinaryExpressionArtist<>, dish2::GroupStructureCategory
  >,
  dish2::GridViewer<
    dish2::PcaExpressionArtist<>, dish2::GroupStructureCategory
  >,
  dish2::GridViewer<
    dish2::PcaRegulationArtist<>, dish2::GroupStructureCategory
  >,
  dish2::GridViewer<
    dish2::PcaTrinaryRegulationArtist<>, dish2::GroupStructureCategory
  >,
  dish2::GridViewer<
    dish2::PhylogeneticRootArtist<>, dish2::DemographicsCategory
  >,
  dish2::SeriesViewer<
    dish2::QuorumBitsArtist<>, dish2::GroupStructureCategory, Spec::NLEV
  >,
  dish2::SeriesViewer<
    dish2::RegulationByModuleArtist<>, dish2::SharingCategory, 16
  >,
  dish2::SeriesViewer<
    dish2::RegulationExposedByModuleArtist<>, dish2::SharingCategory, 16
  >,
  dish2::GridViewer<
    dish2::ResourceInputPeekArtist<>, dish2::SharingCategory
  >,
  dish2::GridViewer<
    dish2::ResourceStockpileArtist<>, dish2::SharingCategory
  >,
  dish2::GridViewer<
    dish2::SpawnArrestArtist<>, dish2::SharingCategory
  >,
  dish2::GridViewer<
    dish2::SpawnRequestArtist<>, dish2::SharingCategory
  >,
  dish2::GridViewer<
    dish2::TaxaArtist<>, dish2::SharingCategory
  >
>;

} // namespace dish2

#endif // #ifndef DISH2_WEB_VIEWERCOLLECTION_HPP_INCLUDE
