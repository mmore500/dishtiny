#pragma once
#ifndef UITSL_PARALLEL_THREADMAP_HPP_INCLUDE
#define UITSL_PARALLEL_THREADMAP_HPP_INCLUDE

#include <mutex>
#include <shared_mutex>
#include <unordered_map>

#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../containers/safe/unordered_map.hpp"
#include "../utility/print_utils.hpp"

#include "thread_utils.hpp"

namespace uitsl {

template<typename T>
class ThreadMap {

  uitsl::safe::unordered_map<uitsl::thread_id_t, T> map;

public:

  T& GetWithDefault(const T& default_=T{}) {
    const uitsl::thread_id_t thread_id{ uitsl::get_thread_id() };

    if (map.count(thread_id) == 0) map.emplace(thread_id, default_);

    return map.at(thread_id);
  }

  T& Get() {
    const uitsl::thread_id_t thread_id{ uitsl::get_thread_id() };
    return map.at(thread_id);
  }

  bool HasEntry() const {
    const uitsl::thread_id_t thread_id{ uitsl::get_thread_id() };
    return map.count(thread_id);
  }

  size_t GetSize() const { return map.size(); }

  std::string ToString() {
    std::stringstream ss;
    for (const auto & [k, v] : map) {
      ss << uitsl::format_member(emp::to_string("thread ", k), v) << '\n';
    }
    return ss.str();
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL_THREADMAP_HPP_INCLUDE
