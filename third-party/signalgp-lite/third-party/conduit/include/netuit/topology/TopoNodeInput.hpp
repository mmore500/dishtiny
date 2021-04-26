#pragma once
#ifndef NETUIT_TOPOLOGY_TOPONODEINPUT_HPP_INCLUDE
#define NETUIT_TOPOLOGY_TOPONODEINPUT_HPP_INCLUDE

#include <iostream>
#include <stddef.h>
#include <utility>

#include "../../uitsl/utility/print_utils.hpp"

namespace netuit {

class TopoNodeInput {

  size_t edge_id;

public:

  TopoNodeInput() = default;
  TopoNodeInput(const size_t edge_id_)
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

  bool operator==(const TopoNodeInput& other) const {
    return edge_id == other.edge_id;
  }

  friend std::ostream& operator<<(std::ostream& os, const TopoNodeInput& node) {
    os << node.edge_id << " ";
    return os;
  }

};
} // namespace netuit

namespace std {
  template <>
  struct hash<netuit::TopoNodeInput> {
    std::size_t operator()(const netuit::TopoNodeInput& k) const {
      return std::hash<size_t>()(k.GetEdgeID());
    }
  };
} // namespace std

#endif // #ifndef NETUIT_TOPOLOGY_TOPONODEINPUT_HPP_INCLUDE
