#pragma once
#ifndef DISH2_SERVICES_INTERMITTENTWRITABLESTATEEXCHANGESERVICE_HPP_INCLUDE
#define DISH2_SERVICES_INTERMITTENTWRITABLESTATEEXCHANGESERVICE_HPP_INCLUDE

#include <algorithm>
#include <numeric>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/datastructs/RingBuffer.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/algorithm/execute_cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../cell/cardinal_iterators/WritableStateWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

template<typename Spec>
struct IntermittentWritableStateExchangeService {

  inline static thread_local emp::vector< dish2::WritableState<Spec> > stash{};

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq
      = dish2::cfg.INTERMITTENT_WRITABLE_STATE_EXCHANGE_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{
      "intermittent writable state exchange service", "TODO", 3
    };

    using spec_t = typename Cell::spec_t;

    using writable_state_t = dish2::WritableState<spec_t>;

    constexpr size_t exchange_chain_length
      = spec_t::WRITABLE_STATE_EXCHANGE_CHAIN_LENGTH;
    using exchange_buffer_t
      = uitsl::RingBuffer<writable_state_t, exchange_chain_length>;

    emp_assert( stash.empty() );

    if (
      cell.genome->config_customizations.ShouldExchangeWritableState()
    ) std::for_each(
      cell.template begin<dish2::WritableStateWrapper<spec_t>>(),
      cell.template end<dish2::WritableStateWrapper<spec_t>>(),
      []( auto& writable_state ){
        thread_local exchange_buffer_t fifo;

        // stash state to be reverted in restore service
        stash.push_back( writable_state );

        // fill fifo up and then start exchanging
        if ( !fifo.IsFull() ) fifo.PushHead( writable_state );
        else {
          fifo.PushHead( std::exchange(
            writable_state, fifo.GetPopTail()
          ) );
        }

      }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_INTERMITTENTWRITABLESTATEEXCHANGESERVICE_HPP_INCLUDE
