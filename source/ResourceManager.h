#pragma once

#include <list>
#include <iostream>

#include "base/vector.h"
#include "base/Ptr.h"
#include "Evolve/World.h"

#include "GridStack.h"
#include "DishtinyConfig.h"
#include "CustomConfig.h"
#include "ChannelManager.h"
#include "SignalManager.h"
#include "Organism.h"

class ResourceManager {

private:
  // x coordinate at center of most recent wave seed
  emp::vector<size_t> seedx;
  // y coordinate at center of most recent wave seed
  emp::vector<size_t> seedy;
  // timestamp of most recent wave seed
  emp::vector<size_t> seedt;
  // current resource wave distribution
  GridStack<double> resource;

  CustomConfig cconfig;

  emp::Ptr<emp::Random> rand;

public:

  ResourceManager(
    DishtinyConfig& dconfig,
    CustomConfig& _cconfig,
    GridSpec& spec,
    emp::Ptr<emp::Random> _r)
  : seedx(dconfig.NLEV())
  , seedy(dconfig.NLEV())
  , seedt(dconfig.NLEV())
  , resource(0.0, spec, dconfig.NLEV())
  , cconfig(_cconfig)
  , rand(_r) { ; }

  /*
   * Accessor function.
   */
  inline size_t const GetDepth() {
    return resource.GetDepth();
  }

  /*
   * Test if last seeded wave time exceeded, then seed a new wave if necessary.
   */
  inline void TryReseed(
      size_t t,
      SignalManager& sig,
      ChannelManager& cm,
      emp::World<Organism>& w
    ) {

    for (size_t lev = 0; lev < resource.GetDepth(); ++lev) {
      if (t % cconfig.EVENT_RADII[lev] == 0) {
        Reseed(lev, t, sig, cm, w);
      }
    }
  }

  /*
   * Lay resource for tesselated diamond waves over all levels.
   */
  inline void LayResource(size_t curtime) {
    // erase laid out resource from last update
    resource.Reset();
    for (size_t lev = 0; lev < resource.GetDepth(); ++lev) {
      LayResourceLev(lev, curtime - seedt[lev]);
    }
  }

  /*
   * Accessor function.
   */
  inline double CheckResource(size_t lev, size_t cell) const {
    return resource.Get(lev, cell);
  }

  /*
   * Put Animators into a supplied vector.
   */
  void MakeAnimators(
      emp::vector<emp::Ptr<GridAnimator<double>>>& dest,
      emp::web::Canvas& c,
      emp::vector<std::function<void()>>& cbs_beforedraw,
      emp::vector<std::function<void()>>& cbs_afterdraw
    ) {
    for (size_t lev = 0; lev < resource.GetDepth(); ++lev) {
      emp::Ptr<double> min_res = new double;
      *min_res = 0;
      emp::Ptr<double> max_res = new double;
      *max_res = cconfig.EVENT_VALS[lev];
      dest.push_back(new GridAnimator<double>(
          resource,
          lev,
          min_res,
          max_res,
          c,
          cbs_beforedraw,
          cbs_afterdraw
        ));
    }
  }


private:
  /*
   * Seed a new wave and tap cells at wave centers.
   */
  void inline Reseed(
      size_t lev,
      size_t t,
      SignalManager& sig,
      ChannelManager& cm,
      emp::World<Organism>& w
    ) {

    // randomly pick new offset for diamonds
    seedx[lev] = rand->GetUInt(0,resource.GetWidth());
    seedy[lev] = rand->GetUInt(0,resource.GetHeight());
    seedt[lev] = t;

    // perform taps (wave centers tesselate over grid based on seed position)
    for (
      int ix = 0;
      //TODO fix ugly casting
      ix <= ((int) resource.GetWidth()) - 2*cconfig.EVENT_RADII[lev];
      ix += (2 * cconfig.EVENT_RADII[lev])) {

      for (
        int iy = 0;
        //TODO fix ugly casting
        iy <= ((int) resource.GetHeight()) - 2*cconfig.EVENT_RADII[lev];
        iy += (2 * cconfig.EVENT_RADII[lev])) {

          sig.TryTakeTap(lev, seedx[lev] + ix, seedy[lev] + iy, w);

      }
    }

  }

  /*
   * Lay resource for a tesselated diamond waves over a level
   * given elapsed time.
   */
  inline void LayResourceLev(size_t lev, int elapsetime) {

    for (
      int ix = 0;
      //TODO fix ugly casting
      ix <= ((int) resource.GetWidth()) - 2*cconfig.EVENT_RADII[lev];
      ix += (2 * cconfig.EVENT_RADII[lev])) {

      for (
        int iy = 0;
        //TODO fix ugly casting
        iy <= ((int) resource.GetHeight()) - 2*cconfig.EVENT_RADII[lev];
        iy += (2 * cconfig.EVENT_RADII[lev])) {

        LayDiamond(lev, ix, iy, elapsetime);

      }
    }
  }

  /*
   * Lay resource for a single diamond wave given elapsed time.
   */
  inline void LayDiamond(size_t lev, int x, int y, int elapsetime) {

    // two cases: initial timestep (i.e. just a single dot)
    // or later timesteps (i.e. a four-sided diamond)
    if (elapsetime == 0) {
      resource(lev, seedx[lev]+x, seedy[lev]+y) += cconfig.EVENT_VALS[lev];
    } else {
      for (int i = 0; i < elapsetime; ++i) {
        resource(
          lev,
          seedx[lev]+x+i,
          seedy[lev]+y+elapsetime-i
        ) += cconfig.EVENT_VALS[lev];
        resource(
          lev,
          seedx[lev]+x+elapsetime-i,
          seedy[lev]+y-i
        ) += cconfig.EVENT_VALS[lev];
        resource(
          lev,
          seedx[lev]+x-i,
          seedy[lev]+y-elapsetime+i
        ) += cconfig.EVENT_VALS[lev];
        resource(
          lev,
          seedx[lev]+x-elapsetime+i,
          seedy[lev]+y+i
        ) += cconfig.EVENT_VALS[lev];
      }
    }

  }

};
