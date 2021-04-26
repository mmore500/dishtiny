#pragma once
#ifndef NETUIT_ARRANGE_ADJACENCYFILETOPOLOGYFACTORY_HPP_INCLUDE
#define NETUIT_ARRANGE_ADJACENCYFILETOPOLOGYFACTORY_HPP_INCLUDE

#include <fstream>

#include "../topology/TopoEdge.hpp"
#include "../topology/Topology.hpp"
#include "../topology/TopoNode.hpp"

namespace netuit {

inline Topology make_adjacency_file_topology(const std::string& filename) {
  std::ifstream file(filename);

  emp_always_assert(file);

  return dynamic_cast<std::istream&>(file);
}

struct AdjacencyFileTopologyFactory {

  Topology operator()(const std::string& filename) const {
    return make_adjacency_file_topology(filename);
  }

  static std::string GetName() { return "Adjacency File Topology"; }

  static std::string GetSlug() { return "adjfile"; }

};

} // namespace netuit

#endif // #ifndef NETUIT_ARRANGE_ADJACENCYFILETOPOLOGYFACTORY_HPP_INCLUDE
