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

  Config::inbox_t spike_broadcast_inbox;

  std::mutex spike_mutex;
  std::mutex spike_broadcast_mutex;

  const Config &cfg;

  void HandleMessage(
    const Config::event_t & event,
    Config::inbox_t & inbox
  ) {

    // Make room for new message in inbox. Remove oldest first.
    while (
      cfg.INBOX_CAPACITY()
      && inbox.size() >= cfg.INBOX_CAPACITY()
    ) inbox.pop_front();

    if(cfg.INBOX_CAPACITY()) inbox.emplace_back(event);

  }

public:

  ManagerInbox(const Config &cfg_)
    : inboxes(Cardi::Dir::NumDirs + 1) // +1 for spiker
    , cfg(cfg_)
  { ; }


  emp::vector<Config::inbox_t>& GetInboxes() {

    // filter out duplicates
    std::sort(std::begin(spike_broadcast_inbox), std::end(spike_broadcast_inbox));
    spike_broadcast_inbox.erase(
      std::unique(std::begin(spike_broadcast_inbox), std::end(spike_broadcast_inbox)),
      std::end(spike_broadcast_inbox)
    );

    // queue up the spike broadcast inbox
    while (spike_broadcast_inbox.size()) {
      for (auto & inbox : inboxes) HandleMessage(
        spike_broadcast_inbox.front(),
        inbox
      );

      spike_broadcast_inbox.pop_front();
    }

    return inboxes;

  }

  void ClearInboxes() {
    for(auto &in : inboxes) in.clear();
    spike_broadcast_inbox.clear();
  }

  void TakeMessage(
    const Config::event_t & event,
    const size_t incoming_direction
  ) {

    if (incoming_direction >= Cardi::Dir::NumDirs) {
      std::lock_guard<std::mutex> lock(spike_mutex);
      HandleMessage(event, inboxes[incoming_direction]);
    } else {
      HandleMessage(event, inboxes[incoming_direction]);
    }

  }

  void TakeSpikeBroadcastMessage(
    const Config::event_t & event
  ) {
    std::lock_guard<std::mutex> lock(spike_broadcast_mutex);
    HandleMessage(event, spike_broadcast_inbox);
  }

  size_t GetTraffic(size_t dir) const {
    return inboxes[dir].size();
  }

  size_t GetSpikeBroadcastTraffic() const {
    return spike_broadcast_inbox.size();
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
