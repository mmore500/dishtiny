#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_CONDUITFLUSHSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_CONDUITFLUSHSERVICE_HPP_INCLUDE

namespace dish2 {

template <class Spec>
void Cell<Spec>::ConduitFlushService() {

  for (auto& cardinal : cardinals) {
    cardinal.genome_node_output.TryFlush();
    cardinal.message_node_output.TryFlush();
    cardinal.resource_node_output.TryFlush();
    cardinal.state_node_output.TryFlush();
  }

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_CONDUITFLUSHSERVICE_HPP_INCLUDE
