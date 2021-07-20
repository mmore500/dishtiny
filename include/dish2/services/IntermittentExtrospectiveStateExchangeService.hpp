#pragma once
#ifndef DISH2_SERVICES_INTERMITTENTEXTROSPECTIVESTATEEXCHANGESERVICE_HPP_INCLUDE
#define DISH2_SERVICES_INTERMITTENTEXTROSPECTIVESTATEEXCHANGESERVICE_HPP_INCLUDE

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

#include "../cell/cardinal_iterators/ExtrospectiveStateWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Used for experimental manipulations testing the fitness effect of
 * extrospective state.
 *
 * (Not part of core simulation logic.)
 */
template<typename Spec>
struct IntermittentExtrospectiveStateExchangeService {

  inline static thread_local emp::vector<
    dish2::ReadableState<Spec>
  > stash{};

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq
      = dish2::cfg.INTERMITTENT_STATE_PERTURB_SERVICES_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{
      "intermittent extrospective state exchange service", "TODO", 3
    };

    using spec_t = typename Cell::spec_t;

    using extrospective_state_t = dish2::ReadableState<spec_t>;

    constexpr size_t exchange_chain_length
      = spec_t::STATE_EXCHANGE_CHAIN_LENGTH;
    using exchange_buffer_t
      = uitsl::RingBuffer<extrospective_state_t, exchange_chain_length>;

    emp_assert( stash.empty() );

    const auto& perturbation_config = cell.genome->GetRootPerturbationConfig();
    const auto& target_idx = perturbation_config.extrospective_state_target_idx;

    if ( perturbation_config.ShouldExchangeExtrospectiveState() ) std::for_each(
      cell.template begin<dish2::ExtrospectiveStateWrapper<spec_t>>(),
      cell.template end<dish2::ExtrospectiveStateWrapper<spec_t>>(),
      [&]( auto& extrospective_state ){
        thread_local exchange_buffer_t fifo;

        // stash state to be reverted in restore service
        stash.push_back( extrospective_state );

        // fill fifo up and then start exchanging
        if ( !fifo.IsFull() ) fifo.PushHead( extrospective_state );
        else {

          const auto incoming = fifo.GetTail();
          fifo.PopTail();
          fifo.PushHead( extrospective_state );

          if ( target_idx.has_value() ) {
            extrospective_state.Assign( *target_idx, incoming );
          } else extrospective_state = incoming;

        }

      }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_INTERMITTENTEXTROSPECTIVESTATEEXCHANGESERVICE_HPP_INCLUDE
