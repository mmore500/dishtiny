#pragma once
#ifndef UITSL_PARALLEL_THREADIBARRIERFACTORY_HPP_INCLUDE
#define UITSL_PARALLEL_THREADIBARRIERFACTORY_HPP_INCLUDE

#include <memory>
#include <stddef.h>

#include "ThreadIbarrier.hpp"
#include "_ThreadIbarrierManager.hpp"

namespace uitsl {

class ThreadIbarrierFactory {

  std::shared_ptr<internal::ThreadIbarrierManager> manager;

public:

  ThreadIbarrierFactory(const size_t expected)
  : manager(std::make_shared<internal::ThreadIbarrierManager>(expected))
  { ; }

  uitsl::ThreadIbarrier MakeBarrier() { return manager->MakeBarrier(); }

};

} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL_THREADIBARRIERFACTORY_HPP_INCLUDE
