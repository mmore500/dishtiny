#pragma once
#ifndef NETUIT_MESH_MESHNODEOUTPUT_HPP_INCLUDE
#define NETUIT_MESH_MESHNODEOUTPUT_HPP_INCLUDE

#include <memory>
#include <sstream>
#include <stddef.h>

#include "../../uit/spouts/Inlet.hpp"

namespace netuit {

template<typename ImplSpec>
class MeshNodeOutput
final : public ImplSpec::template inlet_wrapper_t< uit::Inlet<ImplSpec> > {

  using parent_t
    = typename ImplSpec::template inlet_wrapper_t< uit::Inlet<ImplSpec> >;

  size_t edge_id;

public:

  MeshNodeOutput(
    const parent_t& inlet,
    const size_t edge_id_
  ) : parent_t(inlet)
  , edge_id(edge_id_)
  { ; }

  size_t GetEdgeID() const { return edge_id; }

  std::string ToString() const {
    std::stringstream ss;

    ss << "edge_id " << edge_id << '\n';

    return ss.str();
  }

};

} // namespace netuit

#endif // #ifndef NETUIT_MESH_MESHNODEOUTPUT_HPP_INCLUDE
