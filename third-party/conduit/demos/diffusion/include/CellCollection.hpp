#pragma once

#include <sstream>
#include <string>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "Cell.hpp"

class CellCollection {

  emp::vector<emp::optional<Cell>> cells;

public:

  CellCollection(
    const submesh_t& submesh,
    const size_t num_nodes
  ) : cells( submesh.size() ) {
    // this approach is a workaround for deleted copy constructor on Cell
    for (size_t i = 0; i < submesh.size(); ++i) {
      cells[i].emplace( submesh[i], num_nodes );
    }
  }

  void Update() { for (auto& cell : cells) cell->Update(); }

  size_t GetNumMessagesSent() const {
    return std::accumulate(
      std::begin(cells),
      std::end(cells),
      0,
      [](const size_t a, const auto& cell){
        return a + cell->GetNumMessagesSent();
      }
    );
  }

  size_t GetNumMessagesReceived() const {
    return std::accumulate(
      std::begin(cells),
      std::end(cells),
      0,
      [](const size_t a, const auto& cell){
        return a + cell->GetNumMessagesReceived();
      }
    );
  }

  size_t GetSize() const { return cells.size(); }

  double GetMass() const { return std::accumulate(
    std::begin(cells),
    std::end(cells),
    0.0,
    [](const double cum, const auto& cell){ return cum + cell->GetMass(); }
  ); }

  std::string ToString() const {
    std::stringstream ss;
    ss << "node ids ";
    for (const auto& cell : cells) ss << cell->GetNodeID() << " ";
    ss << '\n';
    ss << "masses ";
    for (const auto& cell : cells) ss << cell->GetMass() << " ";
    return ss.str();
  }

};
