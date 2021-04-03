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
    dish2::ApoptosisRequestArtist<Spec>
  >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::CardinalOrderArtist<Spec>
  // >,
  dish2::GridDrawer<
    Spec,
    dish2::CellBirthArtist<Spec>
  >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::EpochArtist<Spec>
  // >,
  // dish2::SeriesDrawer<
  //   Spec,
  //   dish2::ExpressionByModuleArtist<Spec>
  // >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::HeirRequestArtist<Spec>
  // >
  dish2::GridDrawer<
    Spec,
    dish2::IncomingInterMessageCounterArtist<Spec>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::IncomingIntraMessageCounterArtist<Spec>
  >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::IsAliveArtist<Spec>
  // >,
  // dish2::SeriesDrawer<
  //   Spec,
  //   dish2::PeripheralityLevArtist<Spec>
  // >,
  dish2::SeriesDrawer<
    Spec,
    dish2::KinGroupAgeArtist<Spec>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::KinGroupIDArtist<Spec>
  >,
  // dish2::SeriesDrawer<
  //   Spec,
  //   dish2::KinGroupIDLevArtist<Spec>
  // >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::KinGroupIDViewArtist<Spec>
  // >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::KinMatchArtist<Spec>
  // >,
  // dish2::SeriesDrawer<
  //   Spec,
  //   dish2::LearnedQuorumBitsArtist<Spec>
  // >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::NeighborKinGroupIDViewArtist<Spec>
  // >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::NeighborPosArtist<Spec>
  // >,
  dish2::GridDrawer<
    Spec,
    dish2::NumBusyCoresArtist<Spec>
  >,
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::NumModulesArtist<Spec>
  // >,
  dish2::GridDrawer<
    Spec,
    dish2::PcaBinaryExpressionArtist<Spec>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::PcaExpressionArtist<Spec>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::PcaRegulationArtist<Spec>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::PcaTrinaryRegulationArtist<Spec>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::PhylogeneticRootArtist<Spec>
  >,
  // dish2::SeriesDrawer<
  //   Spec,
  //   dish2::QuorumBitsArtist<Spec>,
  //   Spec::NLEV
  // >,
  // dish2::SeriesDrawer<
  //   Spec,
  //   dish2::RegulationByModuleArtist<Spec>
  // >,
  // dish2::SeriesDrawer<
  //   Spec,
  //   dish2::RegulationExposedByModuleArtist<Spec>
  // >,
  dish2::GridDrawer<
    Spec,
    dish2::ResourceInputPeekArtist<Spec>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::ResourceStockpileArtist<Spec>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::SpawnArrestArtist<Spec>
  >,
  dish2::GridDrawer<
    Spec,
    dish2::SpawnRequestArtist<Spec>
  >
  // dish2::GridDrawer<
  //   Spec,
  //   dish2::TaxaArtist<Spec>
  // >
>;

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DRAWINGS_DRAWERCOLLECTION_HPP_INCLUDE
