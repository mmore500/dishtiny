#pragma once

#include <experimental/optional>
#include <functional>

#include "tools/Random.h"
#include "tools/random_utils.h"

#include "Cardi.h"
#include "Config.h"

class ManagerInbox {

private:

  // arranged by incoming direction
  emp::vector<Config::inbox_t> inboxes;

  const Config &cfg;

public:

  ManagerInbox(const Config &cfg_)
    : inboxes(Cardi::Dir::NumDirs), cfg(cfg_) { ; }


  emp::vector<Config::inbox_t>& GetInboxes() {
    return inboxes;
  }

  void ClearInboxes() {
    for(auto &in : inboxes) in.clear();
  }

  void TakeMessage(const Config::event_t & event, const size_t incoming_direction) {

    // Make room for new message in inbox. Remove oldest first.
    while (inboxes[incoming_direction].size() >= cfg.INBOX_CAPACITY())
      inboxes[incoming_direction].pop_front();

    inboxes[incoming_direction].emplace_back(event);

  }

  size_t GetTraffic(size_t dir) const {
    return inboxes[dir].size();
  }


};
