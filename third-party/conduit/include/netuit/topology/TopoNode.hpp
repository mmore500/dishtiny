#pragma once
#ifndef NETUIT_TOPOLOGY_TOPONODE_HPP_INCLUDE
#define NETUIT_TOPOLOGY_TOPONODE_HPP_INCLUDE

#include <algorithm>
#include <iostream>
#include <string_view>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/polyfill/span.hpp"

#include "../../uitsl/utility/print_utils.hpp"

#include "TopoNodeInput.hpp"
#include "TopoNodeOutput.hpp"

namespace netuit {

class TopoNode {

public:
  using input_t = netuit::TopoNodeInput;
  using output_t = netuit::TopoNodeOutput;

  using inputs_t = emp::vector<input_t>;
  using outputs_t = emp::vector<output_t>;

private:
  inputs_t inputs;
  outputs_t outputs;

public:

  TopoNode() = default;
  TopoNode(const inputs_t& inputs_, const outputs_t& outputs_)
  : inputs(inputs_), outputs(outputs_)
  { ; }

  const inputs_t& GetInputs() const noexcept { return inputs; }

  inputs_t& GetInputs() noexcept { return inputs; }

  const outputs_t& GetOutputs() const noexcept { return outputs; }

  outputs_t& GetOutputs() noexcept { return outputs; }

  void AddInput(const netuit::TopoNodeInput& input_) {
    inputs.push_back(input_);
  }

  void AddOutput(const netuit::TopoNodeOutput& output_) {
    outputs.push_back(output_);
  }

  void RemoveInput(const netuit::TopoNodeInput& input_) {
    inputs.erase(
      std::remove(
        inputs.begin(),
        inputs.end(),
        input_
      ),
      inputs.end()
    );
  }

  void RemoveOutput(const netuit::TopoNodeOutput& output_) {
    outputs.erase(
      std::remove(
        outputs.begin(),
        outputs.end(),
        output_
      ),
      outputs.end()
    );
  }

  size_t GetNumInputs() const noexcept { return inputs.size(); }

  size_t GetNumOutputs() const noexcept { return outputs.size(); }

  bool HasInputs() const noexcept { return inputs.size(); }

  bool HasOutputs() const noexcept { return outputs.size(); }

  std::string ToString() const {
    std::stringstream ss;
    ss << uitsl::format_member(
      "emp::vector<netuit::TopoNodeInput> inputs",
      uitsl::to_string(inputs)
    ) << '\n';
    ss << uitsl::format_member(
      "emp::vector<netuit::TopoNodeInput> outputs",
      uitsl::to_string(outputs)
    );
    return ss.str();
  }

  bool operator==(const TopoNode& other) const {
    return inputs == other.inputs && outputs == other.outputs;
  }

  friend std::ostream& operator<<(std::ostream&, const TopoNode&);

};

inline std::ostream& operator<<(std::ostream& os, const TopoNode& node) {
  // make sure node has outputs
  if (!node.HasOutputs()) return os;

  // loop through span of all outputs, except the last
  for (const auto& edge : std::span<const TopoNode::output_t>(
    node.outputs.data(),
    node.outputs.size() - 1
  )) {
    // out edge ID of edge
    os << edge.GetEdgeID() << " ";
  }
  // out edge ID of last output
  os << node.outputs.back().GetEdgeID();
  return os;
}


} // namespace netuit

#endif // #ifndef NETUIT_TOPOLOGY_TOPONODE_HPP_INCLUDE
