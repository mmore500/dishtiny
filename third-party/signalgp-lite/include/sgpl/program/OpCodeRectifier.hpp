#pragma once
#ifndef SGPL_PROGRAM_OPCODERECTIFIER_HPP_INCLUDE
#define SGPL_PROGRAM_OPCODERECTIFIER_HPP_INCLUDE

#include <algorithm>
#include <numeric>

#include "../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

namespace sgpl {

template<typename Library>
class OpCodeRectifier {

  emp::array< unsigned char, 256> mapper;

public:

  OpCodeRectifier() {

    // first part of mapper must be 1-1 with instruction codes
    emp_assert( Library::GetSize() && Library::GetSize() <= mapper.size());
    std::iota(
      std::begin(mapper),
      std::begin(mapper) + Library::GetSize(),
      0
    );

    // fill the rest of the op codes according to prevalence
    unsigned char op_code{};
    size_t rep_countdown{};
    bool first_time_thru{ true };
    for (
      size_t mapper_idx = Library::GetSize();
      mapper_idx < mapper.size();
      ++mapper_idx
    ) {

      if (rep_countdown == 0) {
        ++op_code;
        if (op_code == Library::GetSize()) first_time_thru = false;
        op_code %= Library::GetSize();
        emp_assert( Library::GetOpPrevalence( op_code ) );
        rep_countdown = Library::GetOpPrevalence( op_code ) - first_time_thru;
        if ( rep_countdown == 0 ) { mapper_idx -= first_time_thru; continue; }
      }

      mapper[ mapper_idx ] = op_code;

      --rep_countdown;

    }

  }

  unsigned char Rectify(const unsigned char oc) const { return mapper[oc]; }

};

} // namespace sgpl

#endif // #ifndef SGPL_PROGRAM_OPCODERECTIFIER_HPP_INCLUDE
