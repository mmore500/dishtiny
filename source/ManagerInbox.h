#pragma once

#include <optional>
#include <functional>
#include <mutex>

#include "tools/Random.h"
#include "tools/random_utils.h"

#include "Cardi.h"
#include "Config.h"

class ManagerInbox {

private:

  // arranged by incoming direction
  emp::vector<Config::inbox_t> inboxes;

  mutable emp::vector<std::mutex> spike_mutex;

  const Config &cfg;

  void HandleMessage(
    const Config::event_t & event,
    const size_t incoming_direction
  ) {

    // Make room for new message in inbox. Remove oldest first.
    while (
      cfg.INBOX_CAPACITY()
      && inboxes[incoming_direction].size() >= cfg.INBOX_CAPACITY()
    ) inboxes[incoming_direction].pop_front();

    if(cfg.INBOX_CAPACITY()) inboxes[incoming_direction].emplace_back(event);

  }

public:

  ManagerInbox(const Config &cfg_)
    : inboxes(Cardi::Dir::NumDirs + 1) // +1 for spiker
    , spike_mutex(Cardi::Dir::NumDirs + 1) // +1 for spiker
    , cfg(cfg_)
  { ; }


  emp::vector<Config::inbox_t>& GetInboxes() {
    return inboxes;
  }

  void ClearInboxes() {
    for(auto &in : inboxes) in.clear();
  }

  void TakeMessage(
    const Config::event_t & event,
    const size_t incoming_direction
  ) {

    std::lock_guard<std::mutex> lock(spike_mutex[incoming_direction]);
    HandleMessage(event, incoming_direction);

  }

  size_t GetTraffic(size_t dir) const {
    return inboxes[dir].size();
  }

  size_t GetTraffic() const {
    return std::accumulate(
      std::begin(inboxes),
      std::end(inboxes),
      0,
      [](const size_t & a, const auto & b){
        return a + b.size();
      }
    );
  }

};
