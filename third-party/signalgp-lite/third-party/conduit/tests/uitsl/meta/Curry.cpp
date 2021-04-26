#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/meta/Curry.hpp"

class CountUpA {
  inline static size_t counter{};
public:
  CountUpA(){ ++counter; };
  static size_t GetCount() { return counter; }
  static void ResetCount() { counter = 0; }
};

class CountUpB {
  inline static size_t counter{};
public:
  CountUpB(){ ++counter; };
  static size_t GetCount() { return counter; }
  static void ResetCount() { counter = 0; }
};

class CountUpC {
  inline static size_t counter{};
public:
  CountUpC(){ ++counter; };
  static size_t GetCount() { return counter; }
  static void ResetCount() { counter = 0; }
};

template<typename X, typename Y>
class TwoHolder {

  const X x;
  const Y y;

};

template<typename X, typename Y, typename Z>
class ThreeHolder {

  const X x;
  const Y y;
  const Z z;

};

TEST_CASE("Test Curry just typenames", "[nproc:1]") {

  CountUpA::ResetCount();
  CountUpB::ResetCount();
  CountUpC::ResetCount();

  REQUIRE( CountUpA::GetCount() == 0 );
  REQUIRE( CountUpB::GetCount() == 0 );
  REQUIRE( CountUpC::GetCount() == 0 );

  uitsl::Curry<TwoHolder, CountUpA>::template curried<CountUpB>{};

  REQUIRE( CountUpA::GetCount() == 1 );
  REQUIRE( CountUpB::GetCount() == 1 );
  REQUIRE( CountUpC::GetCount() == 0 );

  CountUpA::ResetCount();
  CountUpB::ResetCount();
  CountUpC::ResetCount();

  uitsl::Curry<
    uitsl::Curry<
      ThreeHolder,
      CountUpA
    >::template curried,
    CountUpB
  >::template curried<
    CountUpA
  >{};

  REQUIRE( CountUpA::GetCount() == 2 );
  REQUIRE( CountUpB::GetCount() == 1 );
  REQUIRE( CountUpC::GetCount() == 0 );

}

template<size_t X, size_t Y>
struct DoubleHolder {

  const size_t x{X};
  const size_t y{Y};

};

template<int X, char Y, size_t Z>
struct TripleHolder {

  int x{X};
  char y{Y};
  size_t z{Z};

};


TEST_CASE("Test Curry just types", "[nproc:1]") {

  uitsl::CurryValues<
    DoubleHolder,
    2
  >::template curried<1> double_holder{};

  REQUIRE( double_holder.x == 1 );
  REQUIRE( double_holder.y == 2 );

    uitsl::CurryValues<
      TripleHolder,
      99
    >::template curried<-100, 'a'> triple_holder{};

  REQUIRE( triple_holder.x == -100 );
  REQUIRE( triple_holder.y == 'a' );
  REQUIRE( triple_holder.z == 99 );

}

template<typename X, size_t Y>
struct MixedHolder {

  const X x;
  const size_t y{Y};

};

template<typename MeshFactory, typename Ntype>
struct MixedHolderTypeTemplate : public MixedHolder<
  MeshFactory,
  Ntype{}()
> {};

TEST_CASE("Test Curry mixed", "[nproc:1]") {

  CountUpA::ResetCount();
  CountUpB::ResetCount();
  CountUpC::ResetCount();

  REQUIRE( CountUpA::GetCount() == 0 );
  REQUIRE( CountUpB::GetCount() == 0 );
  REQUIRE( CountUpC::GetCount() == 0 );

  uitsl::Curry<
    MixedHolderTypeTemplate,
    uitsl::ValType<2u>
  >::template curried<CountUpA> a{};

  REQUIRE( CountUpA::GetCount() == 1 );
  REQUIRE( CountUpB::GetCount() == 0 );
  REQUIRE( CountUpC::GetCount() == 0 );
  REQUIRE( a.y == 2 );

  CountUpA::ResetCount();
  CountUpB::ResetCount();
  CountUpC::ResetCount();




}
