#pragma once

#include <memory>

#include "netuit/mesh/Mesh.hpp"
#include "netuit/mesh/MeshNode.hpp"

using message_t = double;

#include "ImplSpec.hpp"

using submesh_t = typename netuit::Mesh<ImplSpec>::submesh_t;

using node_t = netuit::MeshNode<ImplSpec>;
