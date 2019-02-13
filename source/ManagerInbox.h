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

  emp::Random &local_rng;

  Config &cfg;

  std::experimental::optional<Config::event_t> GetMsg(size_t box) {
    auto res = inboxes[box].empty() ?
      std::experimental::nullopt :
      std::experimental::optional<Config::event_t>(inboxes[box].front());

    if (!inboxes[box].empty()) inboxes[box].pop_front();

    return res;
  }

public:

  ManagerInbox(Config &cfg_, emp::Random &local_rng_)
    : inboxes(Cardi::Dir::NumDirs), local_rng(local_rng_), cfg(cfg_) { ; }


  void TakeMessage(const Config::event_t & event, size_t incoming_direction) {

    // Make room for new message in inbox. Remove oldest first.
    while (inboxes[incoming_direction].size() >= cfg.INBOX_CAPACITY())
      inboxes[incoming_direction].pop_front();

    inboxes[incoming_direction].emplace_back(event);

  }

  void ClearInboxes() {
    for(auto &in : inboxes) in.clear();
  }


  void QueueMessages(
    Config::hardware_t &hw,
    std::function<bool(size_t)> check_active
  ){

    emp::vector<size_t> dirsv;
    for(size_t d = 0; d < Cardi::Dir::NumDirs; ++d) {
      if(check_active(d)) dirsv.push_back(d);
    }
    emp::Shuffle(local_rng, dirsv);

    std::list<size_t> dirsl(dirsv.cbegin(),dirsv.cend());

    while(dirsl.size()) {

      auto it = dirsl.begin();
      while(it != dirsl.end()) {

        auto opt_msg = GetMsg(*it);

        if (opt_msg) {
          hw.QueueEvent(*opt_msg);
          ++it;
        } else it = dirsl.erase(it);

      }

    }
    // clear out non-active inboxes, too
    ClearInboxes();
  }


};
