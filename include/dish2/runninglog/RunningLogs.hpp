#pragma once
#ifndef DISH2_RUNNINGLOG_RUNNINGLOGS_HPP_INCLUDE
#define DISH2_RUNNINGLOG_RUNNINGLOGS_HPP_INCLUDE

#include <tuple>
#include <utility>

#include "BirthEvent.hpp"
#include "DeathEvent.hpp"
#include "RunningLog.hpp"
#include "SpawnEvent.hpp"

namespace dish2 {

template<typename Spec>
class RunningLogs {

  std::tuple<
    dish2::RunningLog<dish2::BirthEvent<Spec>>,
    dish2::RunningLog<dish2::DeathEvent<Spec>>,
    dish2::RunningLog<dish2::SpawnEvent<Spec>>
  > logs;

public:

  template<typename Event>
  void Record( const Event& event ) {
    std::get<dish2::RunningLog<Event>>( logs ).Record( event );
  }

  void Purge() {
    std::apply(
      [](auto& ...x){ (..., x.Purge()); }, logs
    );
  }

  template<typename Event>
  const auto& GetLog() const {
    return std::get<dish2::RunningLog<Event>>( logs );
  }

};

} // namespace dish2



#endif // #ifndef DISH2_RUNNINGLOG_RUNNINGLOGS_HPP_INCLUDE
