#pragma once
#ifndef NETUIT_TOPOLOGY_TOPONODEOUTPUT_HPP_INCLUDE
#define NETUIT_TOPOLOGY_TOPONODEOUTPUT_HPP_INCLUDE

#include <iostream>
#include <stddef.h>
#include <utility>

#include "../../uitsl/utility/print_utils.hpp"

namespace netuit {

class TopoNodeOutput {

  size_t edge_id;

public:

  TopoNodeOutput() = default;
  TopoNodeOutput(const size_t edge_id_)
  : edge_id(edge_id_)
  { ; }

  size_t GetEdgeID() const { return edge_id; }

  std::string ToString() const {
    std::stringstream ss;
    ss << uitsl::format_member(
      "size_t edge_id",
      edge_id
    );
    return ss.str();
  }

  bool operator==(const TopoNodeOutput& other) const {
    return edge_id == other.edge_id;
  }

  friend std::ostream& operator<<(std::ostream& os, const TopoNodeOutput& node) {
    os << node.edge_id << " ";
    return os;
  }
};
} // namespace netuit

namespace std {
  template <>
  struct hash<netuit::TopoNodeOutput> {
    std::size_t operator()(const netuit::TopoNodeOutput& k) const {
      return std::hash<size_t>()(k.GetEdgeID());
    }
  };
} // namespace std

#endif // #ifndef NETUIT_TOPOLOGY_TOPONODEOUTPUT_HPP_INCLUDE
