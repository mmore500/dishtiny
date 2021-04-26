/**
 *  @note This file is part of Empirical, https://github.com/devosoft/Empirical
 *  @copyright Copyright (C) Michigan State University, MIT Software license; see doc/LICENSE.md
 *  @date 2018
 *
 *  @file  signalgp_utils.h
 *  @brief Helper functions for working with SignalGP virtual hardware/programs.
 *  @todo Mutator class
 *  @todo tests
 */

#ifndef SGP_BITSET_UTILS_H
#define SGP_BITSET_UTILS_H

#include <unordered_set>
#include <string>
#include <functional>
#include <algorithm>

#include "emp/base/errors.hpp"
#include "emp/bits/BitSet.hpp"
#include "emp/math/math.hpp"
#include "emp/math/Random.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/datastructs/map_utils.hpp"

namespace emp {

  /// Generate one random BitSet<W>.
  /// Given a vector of other BitSets (unique_from), this function will guarantee
  /// the generated BitSet is unique with respect to those BitSets.
  /// @param rnd - Random number generator to use.
  /// @param unique_from - Other BitSets that the generated BitSet should be unique from.
  template<size_t W>
  BitSet<W> RandomBitSet(emp::Random & rnd, const emp::vector<BitSet<W>> & unique_from=emp::vector<BitSet<W>>()) {
    std::unordered_set<BitSet<W>> unique_from_set(unique_from.begin(), unique_from.end());
    emp_assert(unique_from_set.size() < emp::Pow2(W), "BitSet<W> is not large enough to be able to guarantee requested number of unique tags");
    BitSet<W> new_bitset(rnd, 0.5);
    while (unique_from_set.size()) {
      if (emp::Has(unique_from_set, new_bitset)) {
        new_bitset.Randomize(rnd);
        continue;
      }
      break;
    }
    return new_bitset;
  }

  /// Generate 'count' number of random BitSet<W>.
  /// Given a vector of other bitsets (unique_from), this function will guarantee the bitsets generated
  /// and returned are unique with respect to unique_from.
  /// @param rnd - Random number generator to use when generating a random bitset.
  /// @param count - How many bitsets should be generated?
  /// @param guarantee_unique - Should generated bitsets be guaranteed to be unique from each other?
  /// @param unique_from - Other bitsets that the bitsets being generated should be unique with respect to. Only used if 'guarantee_unique' is true.
  template<size_t W>
  emp::vector<BitSet<W>> RandomBitSets(emp::Random & rnd, size_t count, bool guarantee_unique=false,
                                       const emp::vector<BitSet<W>> & unique_from=emp::vector<BitSet<W>>())
  {
    // If we don't have to make any promises, run quickly!
    emp::vector<BitSet<W>> new_bitsets(count);
    if (!guarantee_unique && unique_from.size() == 0) {
      for (auto & bs : new_bitsets) { bs.Randomize(rnd); }
    } else {
      std::unordered_set<BitSet<W>> unique_from_set(unique_from.begin(), unique_from.end());
      emp_assert(unique_from_set.size()+count <= emp::Pow2(W), "Not possible to generate requested number of BitSets");
      for (auto & bs : new_bitsets) {
        bs.Randomize(rnd);
        while (unique_from_set.size()) {
          if (emp::Has(unique_from_set, bs)) {
            bs.Randomize(rnd);
            continue;
          }
          break;
        }
        unique_from_set.emplace(bs);
      }
    }
    return new_bitsets;
  }

}

#endif
