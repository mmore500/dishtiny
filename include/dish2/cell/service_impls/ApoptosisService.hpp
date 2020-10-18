#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_APOPTOSISSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_APOPTOSISSERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../cardinal_iterators/ApoptosisRequestWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::ApoptosisService() {

  if ( std::any_of(
    begin<dish2::ApoptosisRequestWrapper<Spec>>(),
    end<dish2::ApoptosisRequestWrapper<Spec>>(),
    std::identity
  ) ) DeathRoutine();

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_APOPTOSISSERVICE_HPP_INCLUDE
