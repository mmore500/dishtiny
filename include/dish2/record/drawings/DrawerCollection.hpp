#pragma once
#ifndef DISH2_RECORD_DRAWINGS_DRAWERCOLLECTION_HPP_INCLUDE
#define DISH2_RECORD_DRAWINGS_DRAWERCOLLECTION_HPP_INCLUDE

#include "../../viz/artists/_index.hpp"

#include "DrawerManager.hpp"
#include "GridDrawer.hpp"
#include "SeriesDrawer.hpp"

namespace dish2 {

template<typename Spec>
using DrawerCollection = dish2::DrawerManager<
  dish2::GridDrawer<
    dish2::ApoptosisRequestArtist<>
  >,
  dish2::GridDrawer<
    dish2::CellBirthArtist<>
  >,
  // dish2::GridDrawer<
  //   dish2::EpochArtist<>
  // >,
  dish2::SeriesDrawer<
    dish2::ExpressionByModuleArtist<>, 16
  >,
  // dish2::GridDrawer<
  //   dish2::HeirRequestArtist<>
  // >
  dish2::GridDrawer<
    dish2::IncomingInterMessageCounterArtist<>
  >,
  dish2::GridDrawer<
    dish2::IncomingIntraMessageCounterArtist<>
  >,
  // dish2::GridDrawer<
  //   dish2::IsAliveArtist<>
  // >,
  // dish2::SeriesDrawer<
  //   dish2::IsPeripheralLevArtist<>, Spec::NLEV
  // >,
  dish2::SeriesDrawer<
    dish2::KinGroupAgeArtist<>, Spec::NLEV
  >,
  dish2::GridDrawer<
    dish2::KinGroupIDArtist<>
  >,
  // dish2::SeriesDrawer<
  //   dish2::KinGroupIDLevArtist<>, Spec::NLEV
  // >,
  // dish2::GridDrawer<
  //   dish2::KinGroupIDViewArtist<>,
  // >,
  // dish2::GridDrawer<
  //   dish2::KinMatchArtist<>
  // >,
  // dish2::SeriesDrawer<
  //   dish2::LearnedQuorumBitsArtist<>, Spec::NLEV
  // >,
  // dish2::GridDrawer<
  //   dish2::NeighborKinGroupIDViewArtist<>
  // >,
  dish2::GridDrawer<
    dish2::NumBusyCoresArtist<>
  >,
  // dish2::GridDrawer<
  //   dish2::NumModulesArtist<>,
  // >,
  dish2::GridDrawer<
    dish2::PcaBinaryExpressionArtist<>
  >,
  dish2::GridDrawer<
    dish2::PcaExpressionArtist<>
  >,
  dish2::GridDrawer<
    dish2::PcaRegulationArtist<>
  >,
  dish2::GridDrawer<
    dish2::PcaTrinaryRegulationArtist<>
  >,
  // dish2::GridDrawer<
  //   dish2::PhylogeneticRootArtist<>,
  // >,
  // dish2::SeriesDrawer<
  //   dish2::QuorumBitsArtist<>,Spec::NLEV
  // >,
  dish2::SeriesDrawer<
    dish2::RegulationByModuleArtist<>, 16
  >,
  dish2::SeriesDrawer<
    dish2::RegulationProtectedByModuleArtist<>, 16
  >,
  dish2::GridDrawer<
    dish2::ResourceInputPeekArtist<>
  >,
  dish2::GridDrawer<
    dish2::ResourceStockpileArtist<>
  >,
  dish2::GridDrawer<
    dish2::SpawnArrestArtist<>
  >,
  dish2::GridDrawer<
    dish2::SpawnRequestArtist<>
  >
  // dish2::GridDrawer<
  //   dish2::TaxaArtist<>,
  // >
>;

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DRAWINGS_DRAWERCOLLECTION_HPP_INCLUDE
