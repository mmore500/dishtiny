#pragma once

#include "Trait.hpp"
#include "typedef.hpp"

event_lib_t make_event_lib() {

  event_lib_t event_lib;

  event_lib.AddEvent(
    "Msg",
    [](hardware_t & hw, const event_t & event){
      hw.GetTrait()->LogReceivedMessage();
      hw.SpawnCore(event.affinity, hw.GetMinBindThresh(), event.msg);
    },
    "Message event"
  );

  event_lib.RegisterDispatchFun(
    "Msg",
    [](hardware_t & hw, const event_t & event){
      auto& trait = *hw.GetTrait();
      for ( auto& output : trait.GetOutputs() ) {
        if ( output.TryPut(event) ) trait.LogSentMessage();
      }
    }
  );

  return event_lib;

};

const event_lib_t& get_event_lib() {
  static event_lib_t cache = make_event_lib();
  return cache;
}
