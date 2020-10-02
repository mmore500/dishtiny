#pragma once
#ifndef DISH_MANAGER_MANAGERINBOX_HPP_INCLUDE
#define DISH_MANAGER_MANAGERINBOX_HPP_INCLUDE

#include <optional>
#include <functional>
#include <mutex>

#include "../../../third-party/Empirical/source/tools/Random.h"
#include "../../../third-party/Empirical/source/tools/random_utils.h"

#include "../topology/Cardi.hpp"
#include "../config/Config.hpp"

namespace dish {

class ManagerInbox {

private:

  // arranged by incoming direction
  emp::vector<Config::inbox_t> inboxes;
  emp::vector<Config::inbox_t> trusted_inboxes;

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
    : inboxes(Cardi::Dir::NumDirs + 1) // +1 for spiker (should be unused)
    , trusted_inboxes(Cardi::Dir::NumDirs + 1) // +1 for spiker
    , cfg(cfg_)
  { ; }


  emp::vector<Config::inbox_t>& GetInboxes() { return inboxes; }

  emp::vector<Config::inbox_t>& GetTrustedInboxes() {

    // filter out duplicates
    std::sort(
      std::begin(spike_broadcast_inbox),
      std::end(spike_broadcast_inbox)
    );
    spike_broadcast_inbox.erase(
      std::unique(
        std::begin(spike_broadcast_inbox),
        std::end(spike_broadcast_inbox)
      ),
      std::end(spike_broadcast_inbox)
    );

    // queue up the spike broadcast inbox
    while (spike_broadcast_inbox.size()) {
      for (auto & inbox : trusted_inboxes) HandleMessage(
        spike_broadcast_inbox.front(),
        inbox
      );

      spike_broadcast_inbox.pop_front();
    }

    return trusted_inboxes;

  }

  void ClearInboxes() {
    for(auto &in : inboxes) in.clear();
    for(auto &in : trusted_inboxes) in.clear();
    spike_broadcast_inbox.clear();
  }

  void TakeMessage(
    const Config::event_t & event,
    const size_t incoming_direction
  ) {

    if (incoming_direction >= Cardi::Dir::NumDirs) {
      std::lock_guard<std::mutex> lock(spike_mutex);
      emp_assert(false, "all spike messages should be trusted");
      // HandleMessage(event, inboxes[incoming_direction]);
    } else {
      HandleMessage(event, inboxes[incoming_direction]);
    }

  }

  void TakeTrustedMessage(
    const Config::event_t & event,
    const size_t incoming_direction
  ) {

    if (incoming_direction >= Cardi::Dir::NumDirs) {
      std::lock_guard<std::mutex> lock(spike_mutex);
      HandleMessage(event, trusted_inboxes[incoming_direction]);
    } else {
      HandleMessage(event, trusted_inboxes[incoming_direction]);
    }

  }

  void TakeSpikeBroadcastMessage(
    const Config::event_t & event
  ) {
    std::lock_guard<std::mutex> lock(spike_broadcast_mutex);
    HandleMessage(event, spike_broadcast_inbox);
  }

  size_t GetTraffic(const size_t dir) const {
    return inboxes[dir].size();
  }

  size_t GetTrustedTraffic(const size_t dir) const {
    return trusted_inboxes[dir].size();
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
    ) + std::accumulate(
      std::begin(trusted_inboxes),
      std::end(trusted_inboxes),
      0,
      [](const size_t & a, const auto & b){
        return a + b.size();
      }
    ) + GetSpikeBroadcastTraffic();
  }

};

} // namespace dish

#endif // #ifndef DISH_MANAGER_MANAGERINBOX_HPP_INCLUDE
