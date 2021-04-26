#include <algorithm>
#include <string>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/datastructs/PodInternalNode.hpp"
#include "uitsl/datastructs/PodLeafNode.hpp"
#include "uitsl/meta/decay_equiv.hpp"

// based on https://commons.wikimedia.org/wiki/File:Organizational_chart.svg#/media/File:Organizational_chart.svg

struct PrivateA{ std::string val{"PrivateA"}; };
struct PrivateB{ std::string val{"PrivateB"}; };

using SergeantB = uitsl::PodInternalNode<
  uitsl::PodLeafNode<PrivateA, 2>,
  uitsl::PodLeafNode<PrivateB, 1>,
  uitsl::PodLeafNode<PrivateA>
>;

struct SergeantA{ std::string val{"SergeantA"}; };

using CaptainA = uitsl::PodInternalNode<
  uitsl::PodLeafNode<SergeantA>,
  SergeantB
>;

// not in original example, child of CaptainB
struct SergeantC{ std::string val{"SergeantC"}; };

using CaptainB = uitsl::PodInternalNode<
  uitsl::PodLeafNode<SergeantC>
>;

struct CaptainC{ std::string val{"CaptainC"}; };

using ColonelB = uitsl::PodInternalNode<
  CaptainA,
  CaptainB,
  uitsl::PodLeafNode<CaptainC, 3>
>;

// ignore ColonelA

using General = uitsl::PodInternalNode<ColonelB>;


TEST_CASE("Test GetSize", "[nproc:1]") {

  REQUIRE( General::GetSize() == 9 );

  REQUIRE( ColonelB::GetSize() == 9 );

  REQUIRE( CaptainA::GetSize() == 5 );

  REQUIRE( CaptainB::GetSize() == 1 );

  REQUIRE( SergeantB::GetSize() == 4 );

}

TEST_CASE("Test Get by index type", "[nproc:1]") {

  General general;

  REQUIRE(
    uitsl::decay_equiv< decltype( general.Get<0>() ), SergeantA >::value
  );

  REQUIRE(
    uitsl::decay_equiv< decltype( general.Get<1>() ), PrivateA >::value
  );

  REQUIRE(
    uitsl::decay_equiv< decltype( general.Get<2>() ), PrivateA >::value
  );

  REQUIRE(
    uitsl::decay_equiv< decltype( general.Get<3>() ), PrivateB >::value
  );

  REQUIRE(
    uitsl::decay_equiv< decltype( general.Get<4>() ), PrivateA >::value
  );

  REQUIRE(
    uitsl::decay_equiv< decltype( general.Get<5>() ), SergeantC >::value
  );

  REQUIRE(
    uitsl::decay_equiv< decltype( general.Get<6>() ), CaptainC >::value
  );

  REQUIRE(
    uitsl::decay_equiv< decltype( general.Get<7>() ), CaptainC >::value
  );

  REQUIRE(
    uitsl::decay_equiv< decltype( general.Get<8>() ), CaptainC >::value
  );

}

TEST_CASE("Test const Get by index value", "[nproc:1]") {

  const General general;

  REQUIRE( general.Get<0>().val ==  "SergeantA" );

  REQUIRE( general.Get<1>().val == "PrivateA" );

  REQUIRE( general.Get<2>().val == "PrivateA" );

  REQUIRE( general.Get<3>().val == "PrivateB" );

  REQUIRE( general.Get<4>().val == "PrivateA" );

  REQUIRE( general.Get<5>().val == "SergeantC" );

  REQUIRE( general.Get<6>().val == "CaptainC" );

  REQUIRE( general.Get<7>().val == "CaptainC" );

  REQUIRE( general.Get<8>().val == "CaptainC" );

}

TEST_CASE("Test Get by index value", "[nproc:1]") {

  General general;

  REQUIRE( general.Get<0>().val ==  "SergeantA" );
  std::transform(
    std::begin( general.Get<0>().val ),
    std::end( general.Get<0>().val ),
    std::begin( general.Get<0>().val ),
    ::toupper
  );
  REQUIRE( general.Get<0>().val ==  "SERGEANTA" );

  REQUIRE( general.Get<1>().val == "PrivateA" );
  std::transform(
    std::begin( general.Get<1>().val ),
    std::end( general.Get<1>().val ),
    std::begin( general.Get<1>().val ),
    ::toupper
  );
  REQUIRE( general.Get<1>().val == "PRIVATEA" );

  REQUIRE( general.Get<2>().val == "PrivateA" );
  std::transform(
    std::begin( general.Get<2>().val ),
    std::end( general.Get<2>().val ),
    std::begin( general.Get<2>().val ),
    ::toupper
  );
  REQUIRE( general.Get<2>().val == "PRIVATEA" );

  REQUIRE( general.Get<3>().val == "PrivateB" );
  std::transform(
    std::begin( general.Get<3>().val ),
    std::end( general.Get<3>().val ),
    std::begin( general.Get<3>().val ),
    ::toupper
  );
  REQUIRE( general.Get<3>().val == "PRIVATEB" );

  REQUIRE( general.Get<4>().val == "PrivateA" );
  std::transform(
    std::begin( general.Get<4>().val ),
    std::end( general.Get<4>().val ),
    std::begin( general.Get<4>().val ),
    ::toupper
  );
  REQUIRE( general.Get<4>().val == "PRIVATEA" );

  REQUIRE( general.Get<5>().val == "SergeantC" );
  std::transform(
    std::begin( general.Get<5>().val ),
    std::end( general.Get<5>().val ),
    std::begin( general.Get<5>().val ),
    ::toupper
  );
  REQUIRE( general.Get<5>().val == "SERGEANTC" );

  REQUIRE( general.Get<6>().val == "CaptainC" );
  std::transform(
    std::begin( general.Get<6>().val ),
    std::end( general.Get<6>().val ),
    std::begin( general.Get<6>().val ),
    ::toupper
  );
  REQUIRE( general.Get<6>().val == "CAPTAINC" );

  REQUIRE( general.Get<7>().val == "CaptainC" );
  std::transform(
    std::begin( general.Get<7>().val ),
    std::end( general.Get<7>().val ),
    std::begin( general.Get<7>().val ),
    ::toupper
  );
  REQUIRE( general.Get<7>().val == "CAPTAINC" );

  REQUIRE( general.Get<8>().val == "CaptainC" );
  std::transform(
    std::begin( general.Get<8>().val ),
    std::end( general.Get<8>().val ),
    std::begin( general.Get<8>().val ),
    ::toupper
  );
  REQUIRE( general.Get<8>().val == "CAPTAINC" );

}

TEST_CASE("Test HasType", "[nproc:1]") {

  REQUIRE( General::HasType<SergeantA>() );

  REQUIRE( General::HasType<CaptainC>() );

  REQUIRE( General::HasType<CaptainC>() );

  REQUIRE( General::HasType<SergeantB>() );

  REQUIRE( !General::HasType<std::string>() );

  REQUIRE ( SergeantB::HasType<PrivateA>() );

  REQUIRE ( !SergeantB::HasType<CaptainC>() );

  REQUIRE ( !SergeantB::HasType<General>() );

}

TEST_CASE("Test const Get by type", "[nproc:1]") {

  const General general;

  REQUIRE( general.Get<SergeantA>().val == "SergeantA");

  REQUIRE( general.Get<CaptainC>().val == "CaptainC");

  REQUIRE( general.Get<CaptainC>().val == "CaptainC");

  REQUIRE( general.Get<SergeantB>().GetSize() == 4 );

  REQUIRE ( general.Get<PrivateA>().val == "PrivateA");

}

TEST_CASE("Test Get by type", "[nproc:1]") {

  General general;

  general.Get<PrivateA>().val = "Ryan";

  REQUIRE( general.Get<1>().val == "Ryan" );

  REQUIRE( general.Get<2>().val == "PrivateA" );

}

TEST_CASE("Test Reset", "[nproc:1]") {

  General general;

  general.Get<PrivateA>().val = "Ryan";

  REQUIRE( general.Get<1>().val == "Ryan" );

  REQUIRE( general.Get<2>().val == "PrivateA" );

  general.Reset();

  REQUIRE( general.Get<1>().val == "PrivateA" );

  REQUIRE( general.Get<2>().val == "PrivateA" );


}

TEST_CASE("Test memory layout", "[nproc:1]") {
#ifdef NDEBUG // emp::array messes up sizing in debug mode...

  REQUIRE( General::GetSize() * sizeof(std::string) == sizeof(General) );

  REQUIRE( ColonelB::GetSize() * sizeof(std::string) == sizeof(ColonelB) );

  REQUIRE( CaptainA::GetSize() * sizeof(std::string) == sizeof(CaptainA) );

  REQUIRE( CaptainB::GetSize() * sizeof(std::string) == sizeof(CaptainB) );

  REQUIRE( SergeantB::GetSize() * sizeof(std::string) == sizeof(SergeantB) );
#endif
}
