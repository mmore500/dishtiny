#pragma once
#ifndef UITSL_META_C__STATIC_TEST_HPP_INCLUDE
#define UITSL_META_C__STATIC_TEST_HPP_INCLUDE

#include <type_traits>

#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../third-party/cereal/include/cereal/cereal.hpp"

#include "decay_equiv.hpp"

#define uitsl_c_message "T is not compatible with cereal or implementation headers in cereal/include/cereal/types have not been included"

namespace uitsl {
namespace c { // cereal type

template<typename T>
class StaticTest {

  using in_archive = cereal::BinaryInputArchive;

public:

  static constexpr bool value {
    cereal::traits::is_input_serializable<T, in_archive>::value
    && cereal::traits::is_input_serializable<T, in_archive>::value
  };

};

template<typename T>
constexpr bool static_test() { return StaticTest<T>::value; }

} // namespace c
} // namespace uitsl

#endif // #ifndef UITSL_META_C__STATIC_TEST_HPP_INCLUDE
