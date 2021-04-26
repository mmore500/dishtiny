#pragma once
#ifndef NETUIT_MESH_MESHNODEINPUT_HPP_INCLUDE
#define NETUIT_MESH_MESHNODEINPUT_HPP_INCLUDE

#include <memory>
#include <sstream>
#include <stddef.h>

#include "../../uit/spouts/Outlet.hpp"

namespace netuit {

template<typename ImplSpec>
class MeshNodeInput final
: public ImplSpec::template outlet_wrapper_t< uit::Outlet<ImplSpec> > {

  using parent_t
    = typename ImplSpec::template outlet_wrapper_t<uit::Outlet<ImplSpec>>;

  size_t edge_id;

public:

  MeshNodeInput(
    const parent_t& outlet,
    const size_t edge_id_
  ) : parent_t(outlet)
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

#endif // #ifndef NETUIT_MESH_MESHNODEINPUT_HPP_INCLUDE
