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
    Spec,
    dish2::ApoptosisRequestArtist<>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::CardinalOrderArtist<>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::CellBirthArtist<>
  >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::EpochArtist<>
  // >,
  dish2::SeriesDrawer<
    Spec,
    dish2::ExpressionByModuleArtist<>
  >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::HeirRequestArtist<>
  // >
  dish2::GridDrawer<
    Spec,
    dish2::IncomingInterMessageCounterArtist<>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::IncomingIntraMessageCounterArtist<>
  >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::IsAliveArtist<>
  // >,
  // dish2::SeriesDrawer<
  //   Spec,
  //   dish2::IsPeripheralLevArtist<>
  // >,
  dish2::SeriesDrawer<
    Spec,
    dish2::KinGroupAgeArtist<>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::KinGroupIDArtist<>
  >,
  // dish2::SeriesDrawer<
  //   Spec,
  //   dish2::KinGroupIDLevArtist<>
  // >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::KinGroupIDViewArtist<>
  // >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::KinMatchArtist<>
  // >,
  // dish2::SeriesDrawer<
  //   Spec,
  //   dish2::LearnedQuorumBitsArtist<>
  // >,
  dish2::GridDrawer<
    Spec,
    dish2::NeighborKinGroupIDViewArtist<>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::NeighborPosArtist<>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::NumBusyCoresArtist<>
  >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::NumModulesArtist<>
  // >,
  dish2::GridDrawer<
    Spec,
    dish2::PcaBinaryExpressionArtist<>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::PcaExpressionArtist<>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::PcaRegulationArtist<>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::PcaTrinaryRegulationArtist<>
  >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::PhylogeneticRootArtist<>
  // >,
  // dish2::SeriesDrawer<
  //   Spec,
  //   dish2::QuorumBitsArtist<>,Spec::NLEV
  // >,
  dish2::SeriesDrawer<
    Spec,
    dish2::RegulationByModuleArtist<>
  >,
  dish2::SeriesDrawer<
    Spec,
    dish2::RegulationProtectedByModuleArtist<>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::ResourceInputPeekArtist<>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::ResourceStockpileArtist<>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::SpawnArrestArtist<>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::SpawnRequestArtist<>
  >
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::TaxaArtist<>
  // >
>;

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DRAWINGS_DRAWERCOLLECTION_HPP_INCLUDE
