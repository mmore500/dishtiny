#pragma once

#include <algorithm>
#include <memory>
#include <ratio>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/Random.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"

#include "uitsl/algorithm/get_plurality.hpp"

#include "typedef.hpp"

class Cell {

  size_t received_message_counter{};
  size_t sent_message_counter{};

  using inputs_t = node_t::inputs_t;
  inputs_t inputs;

  using outputs_t = node_t::outputs_t;
  outputs_t outputs;

  size_t node_id;

  size_t set_channel{};

  emp::vector<double> p; // state vector

  size_t ProposeNextChannel() {

    thread_local emp::Random rand;

    const double d = rand.GetDouble();

    emp_assert( p.size() );

    emp_assert(
      std::abs( 1.0 - std::accumulate( std::begin(p), std::end(p), 0.0 ))
        < 1.0 / std::nano::den,
      std::accumulate( std::begin(p), std::end(p), 0.0 )
    );

    double sum{};

    // turn to index map
    for (size_t i{}; i < p.size() - 1; ++i) {
      sum += p[i];
      if (d < sum) return i;
    }

    // we have to do second check due to possible numerical precision issues
    return p.size() - 1;

  }

  bool DetectInterference(const size_t proposed_channel) const {
    return std::any_of(
      std::begin( inputs ), std::end( inputs ),
      [proposed_channel]( const auto& input ){
        return input.Get() == proposed_channel;
      }
    );
  }

  void UpdateSetChannel() {

    const double b = cfg.B();
    const size_t c = cfg.N_CHANNELS();
    emp_assert( c > 1 );

    thread_local std::vector<size_t> neighbor_channels;
    neighbor_channels.resize( inputs.size() );
    std::transform(
      std::begin(inputs),
      std::end(inputs),
      std::begin(neighbor_channels),
      [](const auto& input){ return input.Get(); }
    );

    // choose channel with probability p
    const size_t proposed_channel = ProposeNextChannel();
    emp_assert( proposed_channel < c );

    if( !DetectInterference(proposed_channel) ) {
      for (size_t i{}; i < p.size(); ++i) p[i] = (i == proposed_channel);
    } else {
      for (size_t i{}; i < p.size(); ++i) {
        if (i == proposed_channel) p[i] *= (1 - b);
        else p[i] = (1 - b) * p[i] + b / (c - 1);
      }
    }

    // choose proposed_channel
    set_channel = proposed_channel;

    emp_assert(
      std::abs( 1.0 - std::accumulate( std::begin(p), std::end(p), 0.0 ))
        < std::nano::den,
      std::accumulate( std::begin(p), std::end(p), 0.0 )
    );

  }

public:

  explicit Cell(const node_t& node)
  : inputs( node.GetInputs() )
  , outputs( node.GetOutputs() )
  , node_id( node.GetNodeID() )
  , p( cfg.N_CHANNELS(), 1.0 / cfg.N_CHANNELS() )
  { }

  void Update(const bool use_inter) {
    PullInputs(use_inter);
    UpdateSetChannel();
    PushOutputs(use_inter);
  }

  void PullInputs(const bool use_inter) {
    for (auto& in : inputs) {
      if (
        use_inter || !in.HoldsIntraImpl().has_value() || *in.HoldsIntraImpl()
      ) {
        if ( in.Jump() ) ++received_message_counter;
      }
    }
  }

  void PushOutputs(const bool use_inter) {
    for (auto& out : outputs) {
      if (
        use_inter || !out.HoldsIntraImpl().has_value() || *out.HoldsIntraImpl()
      ) {
        sent_message_counter += out.TryPut( set_channel );
        out.TryFlush();
      }
    }
  }

  size_t GetNumMessagesSent() const { return sent_message_counter; }

  size_t GetNumMessagesReceived() const { return received_message_counter; }

  const size_t GetNodeID() const { return node_id; }

  double GetState() const { return set_channel; }

  size_t CountConflicts() const {
    return std::count_if(
      std::begin( inputs ), std::end( inputs ),
      [this]( const auto& input ){ return input.Get() == set_channel; }
    );
  }

};
