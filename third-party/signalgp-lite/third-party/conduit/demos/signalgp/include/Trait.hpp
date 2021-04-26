#pragma once

#include "typedef.hpp"

class Trait {

  using outputs_t = typename node_t::outputs_t;
  outputs_t outputs;

  size_t received_message_counter{};

  size_t sent_message_counter{};

public:

  Trait(const outputs_t& outputs_) : outputs(outputs_) { ; }

  void LogReceivedMessage() { ++received_message_counter; }

  void LogSentMessage() { ++sent_message_counter; }

  size_t GetNumMessagesReceived() const { return received_message_counter; }

  size_t GetNumMessagesSent() const { return sent_message_counter; }

  outputs_t& GetOutputs() { return outputs; }

  void FlushOutputs() { for (auto& output : outputs) output.TryFlush(); }

};
