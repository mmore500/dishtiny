#pragma once
#ifndef UITSL_NONCE_THREADUIDNORMALIZER_HPP_INCLUDE
#define UITSL_NONCE_THREADUIDNORMALIZER_HPP_INCLUDE

#include <functional>
#include <unordered_map>

namespace uitsl {

class ThreadUidNormalizer {

  std::function< uitsl::proc_id_t(size_t) > proc_assigner;
  std::function< uitsl::thread_id_t(size_t) > thread_assigner;
  std::unordered_map<
    uitsl::proc_id_t, std::unordered_map<uitsl::thread_id_t, size_t>
  > seen;

public:

  ThreadUidNormalizer(
    const std::function< uitsl::proc_id_t(size_t) >& proc_assigner_,
    const std::function< uitsl::thread_id_t(size_t) >& thread_assigner_
  ) : proc_assigner( proc_assigner_ ), thread_assigner( thread_assigner_ ) {}

  size_t operator()( const size_t arg ) {

    auto& by_proc_lookup = seen[ proc_assigner( arg ) ];

    const auto raw_thread_assignment = thread_assigner( arg );

    if ( !by_proc_lookup.count( raw_thread_assignment ) ) {
      by_proc_lookup.emplace( raw_thread_assignment, by_proc_lookup.size() );
    }

    return by_proc_lookup.at( raw_thread_assignment );

  }

};

} // namespace uitsl

#endif // #ifndef UITSL_NONCE_THREADUIDNORMALIZER_HPP_INCLUDE
