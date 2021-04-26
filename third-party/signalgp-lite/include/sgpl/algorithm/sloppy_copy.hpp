#pragma once
#ifndef SGPL_ALGORITHM_SLOPPY_COPY_HPP_INCLUDE
#define SGPL_ALGORITHM_SLOPPY_COPY_HPP_INCLUDE

#include <limits>
#include <tuple>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/Distribution.hpp"

#include "../utility/ThreadLocalRandom.hpp"

namespace sgpl {

  template <typename T>
  using sloppy_copy_res_t = std::tuple<
    T,
    size_t // cumulative num sites inserted and/or deleted
  >;

  template< typename T , bool Scramble=false, size_t TemplatedInstance=0 >
  sgpl::sloppy_copy_res_t<T> sloppy_copy(
    const T& original,
    const float p_defect,
    const std::pair<size_t, size_t> defect_bounds,
    const size_t res_size_limit=std::numeric_limits<size_t>::max()
  ) {

    sloppy_copy_res_t< T > res{};
    auto& res_vector = std::get<0>(res);
    auto& cumulative_insertion_deletion = std::get<1>(res);

    if (p_defect == 0) { // bail early to avoid crash when p = 0
      res_vector = original;
      cumulative_insertion_deletion = 0;
      return res;
    }

    std::get<0>(res).reserve( original.size() );

    // TODO cache based on p_defect
    thread_local emp::NegativeBinomial neg_bino( p_defect, 1 );
    neg_bino.Setup(p_defect, 1);

    size_t defect_countdown = neg_bino.PickRandom(
      sgpl::tlrand.Get()
    );

    // if we wanted to make the process uniform across a circular genome,
    // then we would draw this from [0, original.size())
    const size_t offset{};

    int scramble_countdown{};

    for (int idx{}; idx < uitsl::audit_cast<int>( original.size() ); ++idx) {

      if (--defect_countdown == 0) {
        const int defect = sgpl::tlrand.Get().GetInt(
          defect_bounds.first, defect_bounds.second
        );
        idx += defect;
        cumulative_insertion_deletion += std::abs( defect );
        defect_countdown = neg_bino.PickRandom(
          sgpl::tlrand.Get()
        );

        // scramble insertion mutations
        if constexpr ( Scramble ) {
          scramble_countdown += -defect;
          scramble_countdown = std::max( scramble_countdown, 0 );
        }

      }

      res_vector.push_back( original[ (idx + offset) % original.size() ] );

      if ( scramble_countdown ) {
        --scramble_countdown;
        sgpl::tlrand.Get().RandFill(
          reinterpret_cast<unsigned char*>( &res_vector.back() ),
          sizeof( res_vector.back() )
        );
      }

      // enforce maximum result size
      if ( res_vector.size() >= res_size_limit ) break;

    }

    if constexpr ( Scramble ) res_vector.Rectify();

    return res;

  }

  template< typename T, bool Scramble=false, size_t TemplatedInstance=0 >
  sgpl::sloppy_copy_res_t<T> sloppy_copy(
    const T& original,
    const float p_defect,
    const size_t defect_bound,
    const size_t res_size_limit=std::numeric_limits<size_t>::max()
  ) {

    return sgpl::sloppy_copy<T, Scramble, TemplatedInstance>(
      original,
      p_defect,
      { -defect_bound, defect_bound },
      res_size_limit
    );

  }

} // namespace sgpl

#endif // #ifndef SGPL_ALGORITHM_SLOPPY_COPY_HPP_INCLUDE
