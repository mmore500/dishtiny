#pragma once

#include "base/Ptr.h"

#include "Config.h"

class EventLibrary {

public:

  using hardware_t = Config::hardware_t;
  using event_lib_t = Config::event_lib_t;
  using event_t = event_lib_t::event_t;

  static const emp::Ptr<event_lib_t> Make(Config &cfg) {

    static event_lib_t el;

    if (el.GetSize() == 0) {

      auto handle_message = [](hardware_t & hw, const event_t & event){
        hw.SpawnCore(event.affinity, hw.GetMinBindThresh(), event.msg);
      };

      el.AddEvent(
        "SendMessage",
        handle_message,
        "Send message event."
      );

      el.AddEvent(
        "BroadcastMessage",
        handle_message,
        "Broadcast message event."
      );


      el.RegisterDispatchFun(
        "SendMessage",
        [](hardware_t & hw, const event_t & event) {

          CellFrame &fr = *hw.GetTrait(0);
          Manager &man = fr.GetManager();
          const size_t outgoing_dir = fr.GetMsgDir();
          const size_t dest = fr.GetNeigh(outgoing_dir);

          man.Inbox(dest).TakeMessage(event, Cardi::Opp[outgoing_dir]);
        }
      );

      el.RegisterDispatchFun(
        "BroadcastMessage",
        [](hardware_t &hw, const event_t &event) {
          CellFrame &fr = *hw.GetTrait(0);
          Manager &man = fr.GetManager();

          for(size_t d = 0; d < Cardi::Dir::NumDirs; ++d) {
            man.Inbox(fr.GetNeigh(d)).TakeMessage(event, Cardi::Opp[d]);
          }
        }
      );

      std::cout << "Event Library Size: " << el.GetSize() << std::endl;

    }

    return &el;

  }

};
