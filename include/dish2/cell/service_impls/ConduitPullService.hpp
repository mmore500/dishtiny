#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_CONDUITPULLSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_CONDUITPULLSERVICE_HPP_INCLUDE

#include "../../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::ConduitPullService() {

  static uitsl::WarnOnce warning{ "TODO ConduitPullService unimplemented" };

  for (auto& cardinal : cardinals) {
    cardinal.genome_node_input.JumpGet();
    cardinal.message_node_input.JumpGet();
    cardinal.resource_node_input.JumpGet();
    cardinal.state_node_input.JumpGet();
  }

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_CONDUITPULLSERVICE_HPP_INCLUDE
