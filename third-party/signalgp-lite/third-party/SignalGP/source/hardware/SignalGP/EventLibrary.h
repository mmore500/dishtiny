#ifndef EMP_EVENT_LIBRARY_H
#define EMP_EVENT_LIBRARY_H

#include <functional>
#include <unordered_set>
#include <map>
#include <string>
#include <iostream>

#include "emp/functional/FunctionSet.hpp"
#include "emp/datastructs/map_utils.hpp"
#include "emp/base/vector.hpp"

// Assumptions:
//  - this really does assume that it is a _SignalGP_ event library
//    - It will work with other hardware types that have the appropriate hooks,
//      but it's not actually generic (e.g., `using event_t = typename hardware::event_t;`)
//    - @AML: Should I move this into a SignalGP namespace, then?

// Notes
// - What is an event?
//   - label <==> payload

namespace sgp {


  /// Base event struct. All SignalGP events should be derived from this.
  struct BaseEvent {
    size_t id;  ///< Specifies associated event type in the EventLibrary.

    BaseEvent(size_t _id=0) : id(_id) { }

    size_t GetID() const { return id; }

    virtual void Print(std::ostream & os) const {
      os << "{id:" << GetID() << "}";
    }
  };

  template<typename HARDWARE_T>
  class EventLibrary {
  public:
    using hardware_t = HARDWARE_T;
    using event_t = BaseEvent;
    using event_handler_fun_t = std::function<void(hardware_t &, const event_t &)>;     ///< Type alias for event-handler functions.
    using event_dispatcher_fun_t = std::function<void(hardware_t &, const event_t &)>;  ///< Type alias for event-dispatcher functions.
    using event_dispatcher_set_t = emp::FunctionSet<void(hardware_t &, const event_t &)>;    ///< Type alias for dispatcher function set type.

    struct EventDef {
      std::string name;                     ///< Name of this event.
      event_handler_fun_t handler_fun;      ///< Function to call when handling this event type.
      std::string desc;                     ///< Description of this event.
      event_dispatcher_set_t dispatch_funs; ///< Functions to call when this type of event is triggered.

      EventDef(const std::string & _name, event_handler_fun_t _handler,
               event_dispatcher_set_t _dispatchers, const std::string & _desc)
        : name(_name), handler_fun(_handler), desc(_desc), dispatch_funs(_dispatchers) { ; }
      EventDef(const EventDef &) = default;
    };

  protected:
    emp::vector<EventDef> event_lib;        ///< Full definitions of each event type.
    std::map<std::string, size_t> name_map; ///< How do event names link to event IDs?

  public:
    EventLibrary() : event_lib(), name_map() { ; }
    EventLibrary(const EventLibrary &) = default;
    ~EventLibrary() { ; }

    EventLibrary & operator=(const EventLibrary &) = default;
    EventLibrary & operator=(EventLibrary &&) = default;

    /// Remove all known events from event library.
    void Clear() {
      event_lib.clear();
      name_map.clear();
    }

    /// Get the string name of the specified event definition.
    const std::string & GetName(size_t id) const { return event_lib[id].name; }

    /// Get the event ID of the event given by string name.
    size_t GetID(const std::string & name) const {
      emp_assert(emp::Has(name_map, name), name);
      return emp::Find(name_map, name, (size_t)-1);
    }

    /// Get the handler function of the specified event definition.
    const event_handler_fun_t & GetHandler(size_t id) const { return event_lib[id].handler_fun; }

    /// Get the dispatch function set of the specified event definition.
    const event_dispatcher_set_t & GetDispatchFuns(size_t id) const { return event_lib[id].dispatch_funs; }

    /// Get the string description of the specified event definition.
    const std::string & GetDesc(size_t id) const { return event_lib[id].desc; }

    /// Get the number of events registered to this event library.
    size_t GetSize() const { return event_lib.size(); }

    /// Add a new event to the event library.
    size_t AddEvent(const std::string & name,
                  const event_handler_fun_t & handler_fun,
                  const event_dispatcher_set_t & dispatchers=event_dispatcher_set_t(),
                  const std::string & desc="") {
      const size_t id = event_lib.size();
      event_lib.emplace_back(name, handler_fun, dispatchers, desc);
      name_map[name] = id;
      return id;
    }

    /// Register a new dispatch function for an event.
    void RegisterDispatchFun(size_t id, event_dispatcher_fun_t dispatch_fun) {
      event_lib[id].dispatch_funs.Add(dispatch_fun);
    }

    /// Register a dispatch function for the event specified by name.
    void RegisterDispatchFun(const std::string & name, event_dispatcher_fun_t dispatch_fun) {
      event_lib[GetID(name)].dispatch_funs.Add(dispatch_fun);
    }

    /// Trigger an event.
    template<typename EVENT_T>
    void TriggerEvent(hardware_t & hw, const EVENT_T & event) const {
      event_lib[event.GetID()].dispatch_funs.Run(hw, event);
    }

    /// Handle an event.
    template<typename EVENT_T>
    void HandleEvent(hardware_t & hw, const EVENT_T & event) const {
      event_lib[event.GetID()].handler_fun(hw, event);
    }
  };

}

#endif