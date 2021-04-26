#pragma once
#ifndef UITSL_PARALLEL_RECURSIVEMUTEX_HPP_INCLUDE
#define UITSL_PARALLEL_RECURSIVEMUTEX_HPP_INCLUDE

#include <shared_mutex>

#include "thread_utils.hpp"

namespace uitsl {

class RecursiveExclusiveLock;

class RecursiveMutex {

  friend class uitsl::RecursiveExclusiveLock;

  std::shared_mutex mutex;
  uitsl::thread_id_t mutex_owner{ uitsl::max_thread };

  void RegisterThreadOwnership() {
    emp_assert(mutex_owner == uitsl::max_thread);
    mutex_owner = uitsl::get_thread_id();
  }

  void DeregisterThreadOwnership() {
    emp_assert(mutex_owner != uitsl::max_thread);
    mutex_owner = uitsl::max_thread;
  }

  bool HasThreadOwnership() const {
    return mutex_owner == uitsl::get_thread_id();
  }

public:

  std::shared_mutex& Mutex() { return mutex; }

};

} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL_RECURSIVEMUTEX_HPP_INCLUDE
