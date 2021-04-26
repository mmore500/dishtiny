#pragma once
#ifndef NETUIT_ARRANGE_RINGTOPOLOGYFACTORY_HPP_INCLUDE
#define NETUIT_ARRANGE_RINGTOPOLOGYFACTORY_HPP_INCLUDE

#include <algorithm>
#include <list>

#include "../topology/TopoEdge.hpp"
#include "../topology/Topology.hpp"
#include "../topology/TopoNode.hpp"

namespace netuit {

inline Topology make_ring_topology(const size_t cardinality) {

  /*
  * goal
  * nodes: -> 0 -> 1 -> 2 -> 3 ->
  *
  * node 0: input from 3 & output to 1
  * node 1: input from 0 & output to 2
  * node 2: input from 1 & output to 3
  * node 3: input from 2 & output to 0
  *
  * node 0: outlet 3 & inlet 0
  * node 1: outlet 0 & inlet 1
  * node 2: outlet 1 & inlet 2
  * node 3: outlet 2 & inlet 3
  */

  size_t edge_counter{};
  emp::vector<TopoEdge> edges;
  std::generate_n(
    std::back_inserter(edges),
    cardinality,
    [&](){ return TopoEdge{++edge_counter}; }
  );

  std::list<TopoNodeOutput> outputs;
  std::transform(
    std::begin(edges),
    std::end(edges),
    std::back_inserter(outputs),
    [](const auto & edge) { return edge.GetInlet(); }
  );

  std::list<TopoNodeInput> inputs;
  std::transform(
    std::begin(edges),
    std::end(edges),
    std::back_inserter(inputs),
    [](const auto & edge) { return edge.GetOutlet(); }
  );

  /*
  * before rotate
  * outputs:  0 1 2 3  (inlets to conduits)
  *           | | | |
  * inputs:   0 1 2 3  (outlets from conduits)
  */

  // rotate node inputs a.k.a. conduit outlets right by one
  if (cardinality) inputs.splice(
    std::begin(inputs),
    inputs,
    std::prev(std::end(inputs))
  );

  /*
  * after rotate
  * nodes:    0 1 2 3
  * outputs:  0 1 2 3   (inlets to conduits)
  *          \ \ \ \ \
  * inputs:   3 0 1 2   (outlets from conduits)
  * nodes:    0 1 2 3
  */

  Topology res;

  auto outputs_iterator = std::begin(outputs);
  auto inputs_iterator = std::begin(inputs);
  for (
    ;
    outputs_iterator != std::end(outputs)
      && inputs_iterator != std::end(inputs);
    ++outputs_iterator, ++inputs_iterator
  ) {
    res.push_back(TopoNode{
      {*inputs_iterator},
      {*outputs_iterator}
    });
  }

  return res;

}

struct RingTopologyFactory {

  Topology operator()(const size_t cardinality) const {
    return make_ring_topology(cardinality);
  }

  netuit::Topology operator()(const emp::vector<size_t> cardinality) const {
    emp_assert(cardinality.size() == 1);
    return make_ring_topology(cardinality.front());
  }

  static std::string GetName() { return "Ring Topology"; }

  static std::string GetSlug() { return "ring"; }

};

} // namespace netuit

#endif // #ifndef NETUIT_ARRANGE_RINGTOPOLOGYFACTORY_HPP_INCLUDE
