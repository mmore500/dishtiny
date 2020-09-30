#pragma once

#include <numeric>
#include <limits>

#include "base/assert.h"

#include "Config.h"

class ManagerSharing {

private:

  const Config &cfg;

  emp::vector<double> in_resistance;
  emp::vector<double> in_resistance_pending;
  emp::vector<size_t> in_resistance_fresh;

  emp::vector<double> out_resistance;
  emp::vector<double> out_resistance_pending;
  emp::vector<size_t> out_resistance_fresh;

  emp::vector<double> sharing_fracs;

  // in: fraction, out: amount given
  std::function<double(const double)> request_resource_frac;

  // in: amount
  // vector is by outgoing_dir relative to here
  emp::vector<
    std::function<void(const double)>
  > external_contribute;

public:

  ManagerSharing(
    std::function<double(const double)> request_resource_frac_,
    const emp::vector<
      std::function<void(const double)>
    > & external_contribute_,
    const Config &cfg_
  ) : cfg(cfg_)
  , in_resistance(Cardi::Dir::NumDirs+1) // +1 for spiker
  , in_resistance_pending(Cardi::Dir::NumDirs+1) // +1 for spiker
  , in_resistance_fresh(Cardi::Dir::NumDirs+1) // +1 for spiker
  , out_resistance(Cardi::Dir::NumDirs+1) // +1 for spiker
  , out_resistance_pending(Cardi::Dir::NumDirs+1) // +1 for spiker
  , out_resistance_fresh(Cardi::Dir::NumDirs+1) // +1 for spiker
  , sharing_fracs(Cardi::Dir::NumDirs+1, 0.0) // +1 for spiker
  , request_resource_frac(request_resource_frac_)
  , external_contribute(external_contribute_)
  { Reset(); }

  double CheckInResistance(const size_t outgoing_dir) const {
    return in_resistance[outgoing_dir];
  }

  double CheckOutResistance(const size_t outgoing_dir) const {
    return out_resistance[outgoing_dir];
  }

  void SetInResistance(
    const size_t outgoing_dir,
    const double set,
    const size_t dur
  ) {
    emp_assert(set >= 0.0);
    emp_assert(set <= 1.0);
    in_resistance_pending[outgoing_dir] = set;
    in_resistance_fresh[outgoing_dir] = dur;
  }

  void SetOutResistance(
    const size_t outgoing_dir,
    const double set,
    const size_t dur
  ) {
    emp_assert(set >= 0.0);
    emp_assert(set <= 1.0);
    out_resistance_pending[outgoing_dir] = set;
    out_resistance_fresh[outgoing_dir] = dur;
  }

  double ViewSharingFrac(const size_t dir) const {
    return sharing_fracs[dir];
  }

  void ResolveNextResistance(const size_t update) {

    emp_assert(update % cfg.ENV_TRIG_FREQ() == 0);

    in_resistance = in_resistance_pending;
    out_resistance = out_resistance_pending;

    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {

      if (out_resistance_fresh[dir]) --out_resistance_fresh[dir];
      if (in_resistance_fresh[dir]) --in_resistance_fresh[dir];

      if (!out_resistance_fresh[dir]) out_resistance[dir] = 0.0;
      if (!in_resistance_fresh[dir]) in_resistance[dir] = 0.0;

    }

    in_resistance_pending = in_resistance;
    out_resistance_pending = out_resistance;

    emp_assert(std::all_of(
      std::begin(in_resistance),
      std::end(in_resistance),
      [](const auto val){ return val >= 0.0 && val <= 1.0; }
    ));
    emp_assert(std::all_of(
      std::begin(out_resistance),
      std::end(out_resistance),
      [](const auto val){ return val >= 0.0 && val <= 1.0; }
    ));

  }

  void AddSharingRequest(
    const size_t outgoing_dir,
    const double frac // in_resistance * out_resistance * req_frac
  ) {
    emp_assert(frac >= 0.0); emp_assert(frac <= 1.0);
    emp_assert(std::isfinite(frac));
    sharing_fracs[outgoing_dir] += (
      (1.0 - sharing_fracs[outgoing_dir]) * frac
    );
    emp_assert(sharing_fracs[outgoing_dir] >= 0.0);
    emp_assert(sharing_fracs[outgoing_dir] <= 1.0);
    emp_assert(std::isfinite(sharing_fracs[outgoing_dir]));
  }

  void ProcessSharingRequest(const size_t update) {

    emp_assert(std::all_of(
      std::begin(sharing_fracs),
      std::end(sharing_fracs),
      [](const auto val){ return val >= 0.0 && val <= 1.0; }
    ));

    emp_assert(std::all_of(
      std::begin(sharing_fracs),
      std::end(sharing_fracs),
      [](const auto val){ return std::isfinite(val); }
    ));

    // have to make sure we don't take a fraction greater than largest request
    const double max_frac = *std::max_element(
      std::begin(sharing_fracs),
      std::end(sharing_fracs)
    );

    emp_assert(
      std::find(
        std::begin(sharing_fracs),
        std::end(sharing_fracs),
        max_frac
      ) != std::end(sharing_fracs)
    );

    emp_assert(std::all_of(
      std::begin(sharing_fracs),
      std::end(sharing_fracs),
      [max_frac](const auto val){ return val <= max_frac; }
    ));

    const double max_amt = request_resource_frac(max_frac);
    emp_assert(max_amt >= 0.0); emp_assert(std::isfinite(max_frac));

    const double tot_fracs = std::accumulate(
      std::begin(sharing_fracs),
      std::end(sharing_fracs),
      0.0
    );
    emp_assert(tot_fracs >= 0.0);

    // normalize fractions
    std::for_each(
      std::begin(sharing_fracs),
      std::end(sharing_fracs),
      [tot_fracs](double & val){ val /= tot_fracs ?  tot_fracs : 1.0; }
    );

    emp_assert(std::all_of(
      std::begin(sharing_fracs),
      std::end(sharing_fracs),
      [](const auto val){ return val >= 0.0 && val <= 1.0; }
    ));

    emp_assert(std::all_of(
      std::begin(sharing_fracs),
      std::end(sharing_fracs),
      [](const auto val){ return isfinite(val); }
    ));

    emp_assert((
      std::set<double>{ 0.0, 1e9 }.count(
        std::round(
          std::accumulate(
            std::begin(sharing_fracs),
            std::end(sharing_fracs),
            0.0
          ) * 1e9
        )
      )
    ));

    for (size_t d = 0; d < sharing_fracs.size(); ++d) {
      external_contribute[d]( sharing_fracs[d] * max_amt );
    }

    if (update % cfg.ENV_TRIG_FREQ() == 0) {
      // reset
      std::fill(
        std::begin(sharing_fracs),
        std::end(sharing_fracs),
        0.0
      );
    }
  }

  void Reset() {
    std::fill(std::begin(in_resistance), std::end(in_resistance), 0.0);
    std::fill(
      std::begin(in_resistance_pending),
      std::end(in_resistance_pending),
      0.0
    );
    std::fill(
      std::begin(in_resistance_fresh),
      std::end(in_resistance_fresh),
      0
    );
    std::fill(std::begin(out_resistance), std::end(out_resistance), 0.0);
    std::fill(
      std::begin(out_resistance_pending),
      std::end(out_resistance_pending),
      0.0
    );
    std::fill(
      std::begin(out_resistance_fresh),
      std::end(out_resistance_fresh),
      0
    );
    std::fill(std::begin(sharing_fracs), std::end(sharing_fracs), 0.0);
  }

};
