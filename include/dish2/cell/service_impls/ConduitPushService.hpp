#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_CONDUITPUSHSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_CONDUITPUSHSERVICE_HPP_INCLUDE

#include "../../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::ConduitPushService() {

  static uitsl::WarnOnce warning{ "TODO ConduitPullService unimplemented" };

  for (auto& cardinal : cardinals) {
    cardinal.genome_node_output.Flush();

    cardinal.message_node_output.Flush();

    cardinal.resource_node_output.Flush();

    cardinal.state_node_output.TryPut( cardinal.peripheral.readable_state );
    cardinal.state_node_output.TryFlush();
  }

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_CONDUITPUSHSERVICE_HPP_INCLUDE
