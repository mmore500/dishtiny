#pragma once

#include <sstream>
#include <string>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "Cell.hpp"

class CellCollection {

  emp::vector<emp::optional<Cell>> cells;

public:

  explicit CellCollection(
    const submesh_t& submesh
  ) : cells( submesh.size() ) {
    // this approach is a workaround for deleted copy constructor on Cell
    for (size_t i = 0; i < submesh.size(); ++i) {
      cells[i].emplace( submesh[i] );
    }
  }

  void Update(const bool use_inter) {
    for (auto& cell : cells) cell->Update(use_inter);
  }

  void PullInputs() { for (auto& cell : cells) cell->PullInputs(true); }

  void PushOutputs() { for (auto& cell : cells) cell->PushOutputs(true); }

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

  size_t CountConflicts() const {
    return std::accumulate(
      std::begin(cells),
      std::end(cells),
      0,
      [](const size_t a, const auto& cell){
        return a + cell->CountConflicts();
      }
    );
  }

  std::string ToString() const {
    std::stringstream ss;
    ss << "node ids ";
    for (const auto& cell : cells) ss << cell->GetNodeID() << " ";
    ss << '\n';
    ss << "states ";
    for (const auto& cell : cells) ss << cell->GetState() << " ";
    return ss.str();
  }

};
