#pragma once
#ifndef NETUIT_TOPOLOGY_TOPOEDGE_HPP_INCLUDE
#define NETUIT_TOPOLOGY_TOPOEDGE_HPP_INCLUDE

#include "TopoNodeInput.hpp"
#include "TopoNodeOutput.hpp"

namespace netuit {

class TopoEdge {

private:

  const size_t uid;

public:

  TopoEdge(const size_t uid_) : uid(uid_) { ; }

  TopoNodeOutput GetInlet() const { return TopoNodeOutput{uid}; }

  TopoNodeInput GetOutlet() const { return TopoNodeInput{uid}; }

};

} // namespace netuit

#endif // #ifndef NETUIT_TOPOLOGY_TOPOEDGE_HPP_INCLUDE
