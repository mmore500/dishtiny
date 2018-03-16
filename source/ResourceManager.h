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

  emp::vector<size_t> seedx;
  emp::vector<size_t> seedy;
  emp::vector<size_t> seedt;
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

  inline size_t const GetDepth() {
    return resource.GetDepth();
  }

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

  inline void LayResource(size_t curtime) {
    // erase laid out resource from last update
    resource.Reset();
    for (size_t lev = 0; lev < resource.GetDepth(); ++lev) {
      LayResource(lev, curtime - seedt[lev]);
    }
  }

  inline double CheckResource(size_t lev, size_t cell) const {
    return resource.Get(lev, cell);
  }

private:
  void inline Reseed(
      size_t lev,
      size_t t,
      SignalManager& sig,
      ChannelManager& cm,
      emp::World<Organism>& w
    ) {

    // draw new offset for diamonds

    seedx[lev] = rand->GetUInt(0,resource.GetWidth());
    seedy[lev] = rand->GetUInt(0,resource.GetHeight());
    seedt[lev] = t;

    // perform taps

    for (
      int ix = 0;
      ix <= ((int) resource.GetWidth()) - 2*cconfig.EVENT_RADII[lev];
      ix += (2 * cconfig.EVENT_RADII[lev])) {

      for (
        int iy = 0;
        iy <= ((int) resource.GetHeight()) - 2*cconfig.EVENT_RADII[lev];
        iy += (2 * cconfig.EVENT_RADII[lev])) {

          sig.TryTakeTap(lev, seedx[lev] + ix, seedy[lev] + iy, w);

      }
    }

  }

  // lay resource at a single level
  inline void LayResource(size_t lev, int elapsetime) {

    for (
      int ix = 0;
      ix <= ((int) resource.GetWidth()) - 2*cconfig.EVENT_RADII[lev];
      ix += (2 * cconfig.EVENT_RADII[lev])) {

      for (
        int iy = 0;
        iy <= ((int) resource.GetHeight()) - 2*cconfig.EVENT_RADII[lev];
        iy += (2 * cconfig.EVENT_RADII[lev])) {

        LayDiamond(lev, ix, iy, elapsetime);

      }
    }
  }

  // lay resource at a single diamond
  inline void LayDiamond(size_t lev, int ix, int iy, int elapsetime) {

    if (elapsetime == 0) {
      resource(lev, seedx[lev]+ix, seedy[lev]+iy) += cconfig.EVENT_VALS[lev];
    } else {
      for (int i = 0; i < elapsetime; i ++) {
        resource(
          lev,
          seedx[lev]+ix+i,
          seedy[lev]+iy+elapsetime-i
        ) += cconfig.EVENT_VALS[lev];
        resource(
          lev,
          seedx[lev]+ix+elapsetime-i,
          seedy[lev]+iy-i
        ) += cconfig.EVENT_VALS[lev];
        resource(
          lev,
          seedx[lev]+ix-i,
          seedy[lev]+iy-elapsetime+i
        ) += cconfig.EVENT_VALS[lev];
        resource(
          lev,
          seedx[lev]+ix-elapsetime+i,
          seedy[lev]+iy+i
        ) += cconfig.EVENT_VALS[lev];
      }
    }

  }

};
