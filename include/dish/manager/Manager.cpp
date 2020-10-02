#pragma once
#ifndef DISHTINY_MANAGER_CPP_INCLUDE
#define DISHTINY_MANAGER_CPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/source/tools/Random.h"

#include "../config/Config.hpp"
#include "../world/DishWorld.hpp"
#include "Manager.hpp"
#include "ManagerApoptosis.hpp"
#include "ManagerChannel.hpp"
#include "ManagerFamily.hpp"
#include "ManagerInbox.hpp"
#include "ManagerPriority.hpp"
#include "ManagerStockpile.hpp"
#include "ManagerWave.hpp"

namespace dish {

Manager::Manager(
  DishWorld &dw_,
  emp::vector<emp::Ptr<emp::Random>> &local_rngs,
  emp::vector<emp::Ptr<emp::Random>> &global_rngs,
  const Config &cfg
) : dw(dw_), mws(cfg.WAVE_REPLICATES()) {

  const size_t size = GeometryHelper(cfg).GetLocalSize();

  for(size_t i = 0; i < size; ++i) {
    mas.push_back(
      emp::NewPtr<ManagerApoptosis>()
    );
    mfs.push_back(
      emp::NewPtr<ManagerFamily>(cfg)
    );
    mcs.push_back(
      emp::NewPtr<ManagerChannel>(
        cfg,
        *local_rngs[i],
        [this, i](){ return mfs[i]->GetCellAge(dw.GetUpdate()); }
      )
    );
    mcos.push_back(
      emp::NewPtr<ManagerConnection>(
        i,
        [this](const size_t pos)->FrameCell&{ return DW().GetFrame(pos); },
        mcs,
        cfg,
        *local_rngs[i]
      )
    );
    mhs.push_back(
      emp::NewPtr<ManagerHeir>()
    );
    mis.push_back(
      emp::NewPtr<ManagerInbox>(cfg)
    );
    mshs.push_back(
      emp::NewPtr<ManagerSharing>(
        [this, i](const double frac){
          return Stockpile(i).RequestResourceFrac(frac);
        },
        [this, i, &cfg](){
          GeometryHelper geom(cfg);
          emp::vector<std::function<void(const double)>> res;
          for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
            const size_t target = geom.CalcLocalNeigh(i, dir);
            const size_t incoming_dir = Cardi::Opp[dir];
            res.push_back(
              [this, target, incoming_dir](const double amt){
                Stockpile(target).ExternalContribute(amt, incoming_dir);
              }
            );
          }

          // spiker
          res.push_back(
            [this, i, &cfg](const double amt){

              double tot_permittivity = 0.0;
              const auto & developed = Connection(i).ViewDeveloped();
              for (const auto & [neigh, cell] : developed) {
                const double cur_permittivity = 1.0 - Sharing(
                  neigh
                ).CheckInResistance(
                  Cardi::Dir::NumDirs
                );
                emp_assert(cur_permittivity >= 0.0);
                emp_assert(cur_permittivity <= 1.0);
                tot_permittivity += cur_permittivity;
              }
              emp_assert(tot_permittivity >= 0.0);

              for (const auto & [loc, cell] : developed) {
                const double cur_permittivity = 1.0 - Sharing(
                  loc
                ).CheckInResistance(
                  Cardi::Dir::NumDirs
                );
                emp_assert(cur_permittivity >= 0.0);
                emp_assert(cur_permittivity <= 1.0);
                const double frac = std::clamp(
                  tot_permittivity
                    ? cur_permittivity / tot_permittivity
                    : 0.0,
                  0.0,
                  1.0
                ); emp_assert(frac >= 0.0); emp_assert(frac <= 1.0);
                // make sure connections are only linked up
                // to same-channel cells
                std::ignore = cfg; // suppress compiler warnings in release mode
                emp_assert(
                  mcs[i]->CheckMatch(*mcs[loc], cfg.NLEV()-1),
                  "source id",
                  mcs[i]->GetID(cfg.NLEV()-1)
                    ? *mcs[i]->GetID(cfg.NLEV()-1)
                    : 0,
                  "dest id",
                  mcs[loc]->GetID(cfg.NLEV()-1)
                    ? *mcs[loc]->GetID(cfg.NLEV()-1)
                    : 0
                );
                Stockpile(loc).ExternalContribute(
                  amt * frac,
                  Cardi::Dir::NumDirs
                );
              }
            }
          );
          emp_assert(res.size() == Cardi::Dir::NumDirs + 1);
          return res;
        }(),
        cfg
      )
    );
    mss.push_back(
      emp::NewPtr<ManagerStockpile>(
        mcs.back()->MakeExpChecker(),
        cfg
      )
    );
  }

  for (size_t i = 0; i < size; ++i) {

    const emp::vector<size_t> neighs = GeometryHelper(cfg).CalcLocalNeighs(i);

    emp::vector<std::function<void()>> refunders;
    for (size_t n : neighs) {
      refunders.push_back(mss[n]->MakeRepRefunder());
    }

    mps.push_back(
      emp::NewPtr<ManagerPriority>(
        cfg,
        *local_rngs[i],
        refunders
      )
    );

  }

  for (size_t r = 0; r < cfg.WAVE_REPLICATES(); ++r) {
    /* ManagerWaves part one */
    for(size_t i = 0; i < size; ++i) {
      mws[r].emplace_back();
      for(size_t l = 0; l < cfg.NLEV(); ++l) {
        mws[r][i].push_back(
          emp::NewPtr<ManagerWave>(
            *mcs[i],
            *mss[i],
            l,
            i,
            *global_rngs[i],
            cfg
        ));
      }
    }

    /* ManagerWaves part two */
    for(size_t i = 0; i < size; ++i) {
      auto neigh_poses = GeometryHelper(cfg).CalcLocalNeighs(i);
      for(size_t l = 0; l < cfg.NLEV(); ++l) {
        emp::vector<emp::Ptr<ManagerWave>> res(neigh_poses.size());
        std::transform(
          neigh_poses.begin(),
          neigh_poses.end(),
          res.begin(),
          [this, l, r](size_t pos) { return mws[r][pos][l]; }
        );
        mws[r][i][l]->SetNeighs(res);
      }
    }
  }

}

Manager::~Manager() {
  for (auto &ptr : mas) ptr.Delete();
  for (auto &ptr : mcs) ptr.Delete();
  for (auto &ptr : mcos) ptr.Delete();
  for (auto &ptr : mfs) ptr.Delete();
  for (auto &ptr : mhs) ptr.Delete();
  for (auto &ptr : mis) ptr.Delete();
  for (auto &ptr : mps) ptr.Delete();
  for (auto &ptr : mss) ptr.Delete();
  for (auto &rep : mws) {
    for (auto &vec : rep) {
      for (auto &ptr : vec) {
        ptr.Delete();
      }
    }
  }
}

ManagerApoptosis& Manager::Apoptosis(size_t pos) {
  return *mas[pos];
}

ManagerChannel& Manager::Channel(size_t pos) {
  return *mcs[pos];
}

const ManagerChannel& Manager::Channel(const size_t pos) const {
  return *mcs[pos];
}

ManagerConnection& Manager::Connection(size_t pos) {
  return *mcos[pos];
}

ManagerFamily& Manager::Family(size_t pos) {
  return *mfs[pos];
}

ManagerHeir& Manager::Heir(size_t pos) {
  return *mhs[pos];
}

ManagerInbox& Manager::Inbox(size_t pos) {
  return *mis[pos];
}

ManagerPriority& Manager::Priority(size_t pos) {
  return *mps[pos];
}

const ManagerPriority& Manager::Priority(size_t pos) const {
  return *mps[pos];
}

ManagerStockpile& Manager::Stockpile(size_t pos) {
  return *mss[pos];
}

ManagerSharing& Manager::Sharing(size_t pos) {
  return *mshs[pos];
}

ManagerWave& Manager::Wave(size_t rep, size_t pos, size_t lev) {
  return *mws[rep][pos][lev];
}

DishWorld& Manager::DW() { return dw; }

} // namespace dish

#endif // #ifndef DISHTINY_MANAGER_CPP_INCLUDE
