#pragma once

#include <algorithm>

#include "tools/Random.h"

#include "Config.h"
#include "DishWorld.h"
#include "ManagerApoptosis.h"
#include "ManagerChannel.h"
#include "ManagerHeir.h"
#include "ManagerFamily.h"
#include "ManagerInbox.h"
#include "ManagerPriority.h"
#include "ManagerStockpile.h"
#include "ManagerWave.h"

class DishWorld;

class Manager {

private:
  DishWorld &dw;
  emp::vector<emp::Ptr<ManagerApoptosis>> mas;
  emp::vector<emp::Ptr<ManagerChannel>> mcs;
  emp::vector<emp::Ptr<ManagerFamily>> mfs;
  emp::vector<emp::Ptr<ManagerHeir>> mhs;
  emp::vector<emp::Ptr<ManagerInbox>> mis;
  emp::vector<emp::Ptr<ManagerPriority>> mps;
  emp::vector<emp::Ptr<ManagerStockpile>> mss;
  emp::vector<emp::vector<emp::vector<emp::Ptr<ManagerWave>>>> mws;


public:

  Manager() = delete;

  Manager(
    DishWorld &dw_,
    emp::vector<emp::Ptr<emp::Random>> &local_rngs,
    emp::vector<emp::Ptr<emp::Random>> &global_rngs,
    const Config &cfg
  );

  ~Manager();

  ManagerApoptosis& Apoptosis(size_t pos);

  ManagerChannel& Channel(size_t pos);

  ManagerFamily& Family(size_t pos);

  ManagerHeir& Heir(size_t pos);

  ManagerInbox& Inbox(size_t pos);

  ManagerPriority& Priority(size_t pos);

  ManagerStockpile& Stockpile(size_t pos);

  ManagerWave& Wave(size_t rep, size_t pos, size_t lev);

  DishWorld &DW();

};
