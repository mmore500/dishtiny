#pragma once
#ifndef NETUIT_MESH_MESHNODE_HPP_INCLUDE
#define NETUIT_MESH_MESHNODE_HPP_INCLUDE

#include <sstream>
#include <stddef.h>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "MeshNodeInput.hpp"
#include "MeshNodeOutput.hpp"

namespace netuit {

template<typename ImplSpec>
class MeshNode {

public:
  using input_t = netuit::MeshNodeInput<ImplSpec>;
  using output_t = netuit::MeshNodeOutput<ImplSpec>;

  using inputs_t = emp::vector<input_t>;
  using outputs_t = emp::vector<output_t>;

private:
  inputs_t inputs;
  outputs_t outputs;

  const size_t node_id;

public:

  MeshNode(const size_t node_id_) : node_id(node_id_) { ; }

  void AddInput(const input_t &input) {
    inputs.push_back(input);
  }

  void AddOutput(const output_t &output) {
    outputs.push_back(output);
  }

  size_t GetNodeID() const { return node_id; }

  inputs_t& GetInputs() { return inputs; }

  outputs_t& GetOutputs() { return outputs; }

  const inputs_t& GetInputs() const { return inputs; }

  const outputs_t& GetOutputs() const { return outputs; }

  input_t& GetInput(const size_t i) { return inputs[i]; }

  output_t& GetOutput(const size_t i) { return outputs[i]; }

  const input_t& GetInput(const size_t i) const { return inputs[i]; }

  const output_t& GetOutput(const size_t i) const { return outputs[i]; }

  size_t GetNumInputs() const { return inputs.size(); }

  size_t GetNumOutputs() const { return outputs.size(); }

  bool HasInputs() const { return GetNumInputs(); }

  bool HasOutputs() const { return GetNumOutputs(); }

  bool HasInput(const size_t i) const { return i < GetNumInputs(); }

  bool HasOutput(const size_t i) const { return i < GetNumOutputs(); }

  emp::optional<input_t> GetInputOrNullopt(const size_t i) const {
    return HasInput(i) ? emp::optional<input_t>{GetInput(i)} : std::nullopt;
  }

  emp::optional<output_t> GetOutputOrNullopt(const size_t i) const {
    return HasOutput(i) ? emp::optional<output_t>{GetOutput(i)} : std::nullopt;
  }

  std::string ToString() const {
    std::stringstream ss;

    ss << "node_id" << node_id << '\n';

    ss << "inputs" << '\n';
    for ( const auto& input : inputs ) {
      ss << input.ToString() << '\n';
    }

    ss << "outputs" << '\n';
    for ( const auto& output : outputs ) {
      ss << output.ToString() << '\n';
    }

    return ss.str();
  }

};

} // namespace netuit

#endif // #ifndef NETUIT_MESH_MESHNODE_HPP_INCLUDE
