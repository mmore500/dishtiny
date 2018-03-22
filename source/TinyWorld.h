#pragma once

#include <utility>
#include <iostream>

#include "base/vector.h"
#include "base/Ptr.h"
#include "tools/Random.h"
#include "tools/random_utils.h"
#include "Evolve/World.h"
#include "base/assert.h"

#include "ChannelManager.h"
#include "CustomConfig.h"
#include "DishtinyConfig.h"
#include "Organism.h"
#include "ResourceManager.h"
#include "SignalManager.h"
#include "StoreManager.h"

class TinyWorld : public emp::World<Organism> {
private:
  const size_t nupdate;

  emp::Random rand;

  GridSpec spec;

  ChannelManager channel;
  ResourceManager resource;
  SignalManager signal;
  StoreManager store;

  const size_t NLEV;
  const size_t GRID_A;
  const double REP_THRESH;
  const size_t PUPDATE_FREQ;
  const double SUICIDE_EFF;
  const int SEED;

  emp::vector<size_t> shuffler;
  emp::vector<emp::Ptr<std::pair<size_t, bool>>> neighborsorter;

  size_t birth_loc;

  // for collecting genotype snapshots
  using dnod_genotype_t = emp::DataNode<double,
      emp::data::Current,
      emp::data::Range, emp::data::Stats,
      emp::data::Pull, emp::data::Log
    >;

  emp::vector<dnod_genotype_t> dns_endowment;
  emp::vector<dnod_genotype_t> dns_res_pool;
  emp::vector<dnod_genotype_t> dns_avoid_over;
  emp::vector<dnod_genotype_t> dns_off_ch_cap;
  emp::vector<dnod_genotype_t> dns_sort_off;
  dnod_genotype_t dn_damage_suicide;


  using dnod_phenotype_t = emp::DataNode<int,
      emp::data::Current, emp::data::Range,
      emp::data::Pull, emp::data::Log
    >;
  // for tracking how often cells decline to reproduce due to
  // avoiding placing offspring over neighboring cells on same channel
  dnod_phenotype_t dn_repdecline;
  // for tracking how often cells perform apoptosis
  dnod_phenotype_t dn_apoptosis;
  // for tracking how often mutations occur
  dnod_phenotype_t dn_mutation;
  // for tracking the number of cell reproductions
  dnod_phenotype_t dn_reproduce;
  // for tracking the number of new channel reproductions
  emp::vector<dnod_phenotype_t> dns_channelrep;

public:
  TinyWorld(
    int _nupdate,
    emp::Random& _rand,
    DishtinyConfig& dconfig,
    CustomConfig& cconfig)
  : emp::World<Organism>(_rand)
  , nupdate(_nupdate)
  , rand(_rand)
  , spec(dconfig)
  , channel(dconfig, spec, &rand)
  , resource(dconfig, cconfig, spec, &rand)
  , signal(dconfig, spec)
  , store(dconfig, cconfig, spec)
  , NLEV(dconfig.NLEV())
  , GRID_A(dconfig.GRID_H()*dconfig.GRID_W())
  , REP_THRESH(dconfig.REP_THRESH())
  , PUPDATE_FREQ(dconfig.PUPDATE_FREQ())
  , SUICIDE_EFF(dconfig.SUICIDE_EFF())
  , SEED(dconfig.SEED())
  , shuffler(emp::GetPermutation(rand, GRID_A))
  , neighborsorter()
  , dns_channelrep(dconfig.NLEV())
  {

    // add one element for all four neighbors
    for (size_t i = 0; i < 4; ++i) {
      neighborsorter.push_back(new std::pair<size_t, bool>);
    }

    // configure world object
    SetupWorld(dconfig, cconfig);

    // populate the world
    for (size_t cell = 0; cell < GRID_A; ++cell) {
      emp::Ptr<Organism> org = new Organism(&rand, dconfig, &cconfig);
      emp::World<Organism>::InjectAt(*org, cell);
    }

    // setup data nodes and data files
    emp::World<Organism>::SetupSystematicsFile(
      "Systematics_"
      +std::to_string(dconfig.SEED())
      +".csv"
    ).SetTimingRepeat(dconfig.GDATA_FREQ());

    emp::World<Organism>::SetupPopulationFile(
      "Population_"
      +std::to_string(dconfig.SEED())
      +".csv"
    ).SetTimingRepeat(dconfig.PDATA_FREQ());

    SetupGenotypeData(dconfig);
    SetupGenotypeFile(
      "Genotypes_"
      +std::to_string(dconfig.SEED())
      +".csv"
    ).SetTimingRepeat(dconfig.GDATA_FREQ());

    SetupPhenotypeFile(
      "Phenotypes_"
      +std::to_string(dconfig.SEED())
      +".csv"
    ).SetTimingRepeat(dconfig.PDATA_FREQ());
  }

  /*
   * Had to write this to fix Segfault on destruction.
   */
  ~TinyWorld() { emp::World<Organism>::Clear(); }

  /*
   * Take a single Update step. Return false if nupdate exceeded, else true.
   */
  inline bool Update() {

    // make sure that organisms are being tracked properly in the world
    emp_assert([this](){
      size_t count = 0;
      for (size_t cell = 0; cell < GRID_A; ++cell) {
        if (emp::World<Organism>::IsOccupied(cell)) ++count;
      }
      return count == emp::World<Organism>::GetNumOrgs();
    }());

    if (emp::World<Organism>::GetUpdate() % PUPDATE_FREQ == 0) {
      std::cout << "update " << emp::World<Organism>::GetUpdate() << std::endl;
    }

    signal.Propagate(channel, *this);

    resource.TryReseed(emp::World<Organism>::GetUpdate(), signal, channel, *this);

    resource.LayResource(emp::World<Organism>::GetUpdate());

    store.Harvest(signal, resource, channel, *this);

    store.PayStateCost(signal, resource, channel, *this);

    store.SettlePools(channel);

    store.KillDebtors(*this);

    // individual reproduction, paid for by own stockpile
    emp::Shuffle(rand, shuffler);
    for (size_t i = 0; i < GRID_A; ++i) {
      store.TransactStockpile(
        shuffler[i],
        TryReproduceCell(shuffler[i], store.CheckStockpile(shuffler[i]))
      );
    }

    // pool reproduction, just like individual reproduction but paid for by
    // channel resource pools
    for (size_t lev = 0; lev < NLEV; ++lev) {

      // we need to use a copy of the channel list
      // because reproduction occurs during iteration
      emp::vector<int> temp;
      channel.CopyChannelList(lev, temp);
      emp::Shuffle(rand, temp);

      for (size_t memb = 0; memb < temp.size(); ++memb) {
        int ch = temp[memb];
        if (channel.ChannelExists(lev, ch)) {
          ReproducePool(lev, ch);
        }
      }

    }

    emp::World<Organism>::Update();

    return (emp::World<Organism>::GetUpdate() < nupdate);
  }

  /*
   * Force the simulation to complete len steps at a time.
   */
  inline bool Update(size_t len) {

    bool res = false;
    for (size_t i = 0; i < len; ++i) res = Update();

    return res;

  }

  /*
   * Run the simulation until nupdate exceeded.
   */
  inline void Run() {
    while (Update());
  }

  /*
   * Make GridAnimators for each level and place them in supplied vector.
   */
  void MakeChannelAnimators(
      emp::vector<emp::Ptr<GridAnimator<int>>>& dest,
      emp::web::Canvas& c,
      emp::vector<std::function<void()>>& cbs_beforedraw,
      emp::vector<std::function<void()>>& cbs_afterdraw
    ) {
    return channel.MakeAnimators(dest, c, cbs_beforedraw, cbs_afterdraw);
  };

  /*
   * Make GridAnimators for each level and place them in supplied vector.
   */
  void MakeResourceAnimators(
      emp::vector<emp::Ptr<GridAnimator<double>>>& dest,
      emp::web::Canvas& c,
      emp::vector<std::function<void()>>& cbs_beforedraw,
      emp::vector<std::function<void()>>& cbs_afterdraw
    ) {
    return resource.MakeAnimators(dest, c, cbs_beforedraw, cbs_afterdraw);
  };

  /*
   * Make GridAnimators for each level and place them in supplied vector.
   */
  void MakeSignalAnimators(
      emp::vector<emp::Ptr<GridAnimator<int>>>& dest,
      emp::web::Canvas& c,
      emp::vector<std::function<void()>>& cbs_beforedraw,
      emp::vector<std::function<void()>>& cbs_afterdraw
    ) {
    return signal.MakeAnimators(dest, c, cbs_beforedraw, cbs_afterdraw);
  };

private:
  /*
   * Actually reproduce a cell.
   * NOTE: this does function does not calculate the cost of reproduction or
   * levy that cost. These responsibilies are left to the caller.
   */
  inline void ReproduceCell(size_t parent, size_t dest, size_t off_level, double endowment) {

    // log reproduction...
    dn_reproduce.Add(1);

    // do reproduction...
    emp::Ptr<Organism> child =
      new Organism(emp::World<Organism>::GetOrg(parent));
    bool mut = child->DoMutations(rand);

    // takes care of killing trampled cell
    // birth_loc is hooked into DoBirth function through Lambda in this scope
    birth_loc = dest;
    emp::World<Organism>::DoBirth(*child, parent);

    // copy over channels on levels at and above off_level
    channel.Spawn(parent, dest, off_level, dns_channelrep);

    // give endowment
    store.MultilevelTransaction(emp::World<Organism>::GetOrg(dest), dest, channel, endowment);

    // if suicide on damage triggered, do it and log it
    if (mut) {
      dn_mutation.Add(1);
      if (rand.GetDouble() < child->GetDamageSuicide()
          && rand.GetDouble() < SUICIDE_EFF
        ) {
          emp::World<Organism>::DoDeath(dest);
          dn_apoptosis.Add(1);
      }
    }

  }

  /*
   * Check endowment requirements and channel size caps to determine the
   * hierarchical level on which a cell wishes to reproduce. During
   * reproduction, channels on levels at and above off_level will be copied
   * to the daugher cell. To reproduce at a parcticular level, avail_resource
   * must be sufficient to pay the endowment and reproduction cost AND the
   * organism's cap on channel population size must not be exceeded for that
   * level and all levels above.
   * If these critieria are met for no off_level, then one past the last valid
   * off_level is returned.
   */
  inline size_t pick_off_level(size_t cell, double avail_resource) {

    emp_assert(emp::World<Organism>::IsOccupied(cell));

    const Organism& org = emp::World<Organism>::GetOrg(cell);

    // check endowments and caps
    for (size_t tlev = 0; tlev < NLEV + 1; ++tlev) {
      bool res = true;

      // check cap
      for (size_t i = tlev; i < NLEV; ++i) {
        res &= (
            channel.GetCensusCount(i, channel.GetChannel(i, cell))
            < org.GetOffChCap(i)
          );
      }

      // check endowment
      res &= (avail_resource >= org.GetEndowment(tlev) + REP_THRESH);

      // choose lowest viable level for reproduction
      if (res) {
        return tlev;
      }
    }

    // on failure, return one past maximum valid reproduction level
    // where viable reproduction levels 0-NLEV inclusive are valid
    return NLEV+1;
  }

  /*
   * Pick a neighboring cell to place offspring in. Return GRID_A (one past
   * maximum valid cell) on failure.
   */
  inline size_t pick_off_dest(size_t cell, size_t off_level) {

    // initialize neighborsorter
    for (size_t i = 0; i < neighborsorter.size(); ++i) {
      neighborsorter[i]->second = true;
    }

    const size_t x = spec.GetX(cell);
    const size_t y = spec.GetY(cell);

    neighborsorter[0]->first = spec.GetID(x, y+1);
    neighborsorter[1]->first = spec.GetID(x, y-1);
    neighborsorter[2]->first = spec.GetID(x+1, y);
    neighborsorter[3]->first = spec.GetID(x-1, y);

    // at each level, decide if want to exclude cells that match own channel

    const Organism& org = emp::World<Organism>::GetOrg(cell);

    for (size_t lev = 0; lev < NLEV; ++lev) {
      if (org.GetAvoidOver(lev) > rand.GetDouble()) {
        int cur_ch = channel.GetChannel(lev, cell);
        for (size_t i = 0; i < neighborsorter.size(); ++i) {
          neighborsorter[i]->second
            &= (channel.GetChannel(lev,neighborsorter[i]->first) != cur_ch);
        }
      }
    }

    emp::Shuffle(rand, neighborsorter);
    if (off_level < NLEV && org.GetSortOff(off_level) > rand.GetDouble()) {
      // if desired, sort neighbors to try to put in neighbor closest
      // to center of off_level centroid first
      channel.SortByCentroids(
        neighborsorter,
        off_level,
        channel.GetChannel(off_level, cell));
    }

    // proceed through neighbors in random (or perhaps sorted) order,
    // returning first index that is valid
    for (size_t i = 0; i < neighborsorter.size(); ++i) {
      if (neighborsorter[i]->second) {
        return neighborsorter[i]->first;
      }
    }

    // on failure, return one past maximum valid cell index
    return GRID_A;
  }

  /*
   * Return 0.0 for failure or cost of reproduction (incl. endowment) if
   * reproduction succeeds. Take care of reproduction on success.
   */
  inline double TryReproduceCell(size_t cell, double avail_resource) {
    // is the cell alive?
    if (!emp::World<Organism>::IsOccupied(cell)) {
      return 0.0;
    }

    // did the cell pick a hierarchical level to reproduce on?
    // (this is where endowment requirements and channel size caps are checked)
    size_t off_level = pick_off_level(cell, avail_resource);
    // 0 thru NLEV are valid responses, NLEV + 1 returned on fail
    if (off_level == NLEV + 1) {
      return 0.0;
    }

    // did the cell pick a viable neighboring cell to place its offspring in?
    size_t off_dest = pick_off_dest(cell, off_level);
    if (off_dest == GRID_A) {
      // the cell declined to reproduce to avoid trampling channel-mates
      dn_repdecline.Add(1);
      return 0.0;
    }

    // from this point onwards, we're cleared for reproduction
    double endow = emp::World<Organism>::GetOrg(cell).GetEndowment(off_level);

    ReproduceCell(cell, off_dest, off_level, endow);

    return -REP_THRESH - endow;
  }


  /*
   * In the ChannelManager's Census, organisms in each channel are organized
   * from close to channel clump center to far from channel clump center. If
   * pool resource exceeds REP_THRESH, give each cell a chance to reproduce in
   * order from center outwards. Once someone reproduces, if there's still
   * enough resource then go back through the (re-sorted) list until nobody
   * reproduces.
   */
  inline void ReproducePool(size_t lev, int ch) {

    double res;
    do {

      res = 0.0;
      for (
        size_t memb = 0;
        (memb < channel.GetCensusCount(lev, ch) &&
        res == 0.0 &&
        store.CheckPool(lev,ch) >= REP_THRESH);
        ++memb
        ) {

          res = TryReproduceCell(
            channel.GetCensusCell(lev,ch,memb),
            store.CheckPool(lev,ch)
            );
          store.TransactPool(lev, ch, res);

      }
    } while (store.CheckPool(lev, ch) >= REP_THRESH && channel.GetCensusCount(lev,ch) > 0 && res != 0.0);

  }

  /*
   * Configure World.h stuff.
   */
  inline void SetupWorld(DishtinyConfig& dconfig, CustomConfig& cconfig) {
    // this also sets the world to asynchronous mode
    emp::World<Organism>::SetGrid(dconfig.GRID_W(), dconfig.GRID_H());

    emp::World<Organism>::OnOrgDeath( [this](size_t pos) {
      // set channels to DEAD
      // if org was last of particular channel, remove that channel's res pool
      channel.Kill(
        pos,
        [this](size_t lev, int ch){ store.ErasePool(lev, ch); }
      );
      // erase organism's resource stockpile
      store.EraseStockpile(pos);
    } );

    emp::World<Organism>::SetAddBirthFun(
      [this](emp::Ptr<Organism> new_org, size_t parent_id) {
        // kill old organism if necessary, place new organism
        emp::World<Organism>::AddOrgAt(
          new_org,
          birth_loc,
          emp::World<Organism>::genotypes[parent_id]
        );
        return birth_loc;
    } );

  }

  /*
   * Setup our data file to collect genotypic information.
   */
  emp::World_file& SetupGenotypeFile(const std::string& filename) {
    auto& file = SetupFile(filename);

    file.AddVar(SEED, "seed", "Random generator seed");

    file.AddVar(update, "update" , "Update");

    for (size_t i = 0; i < dns_endowment.size(); ++i) {
      file.AddMean(
        dns_endowment[i],
        "mean_endowment"+std::to_string(i),
        "TODO"
      );
    }

    for (size_t i = 0; i < dns_res_pool.size(); ++i) {
      file.AddMean(
        dns_res_pool[i],
        "mean_res_pool"+std::to_string(i),
        "TODO"
      );
    }

    for (size_t i = 0; i < dns_avoid_over.size(); ++i) {
      file.AddMean(
        dns_avoid_over[i],
        "mean_avoid_over"+std::to_string(i),
        "TODO"
      );
    }

    for (size_t i = 0; i < dns_off_ch_cap.size(); ++i) {
      file.AddMean(
        dns_off_ch_cap[i],
        "mean_off_ch_cap"+std::to_string(i),
        "TODO"
      );
    }

    for (size_t i = 0; i < dns_sort_off.size(); ++i) {
      file.AddMean(
        dns_sort_off[i],
        "mean_sort_off"+std::to_string(i),
        "TODO"
      );
    }

    file.AddMean(
      dn_damage_suicide,
      "mean_damage_suicide",
      "TODO"
    );

    file.PrintHeaderKeys();

    return file;
  }

  /*
   * Setup our data file to collect phenotypic information.
   */
  emp::World_file& SetupPhenotypeFile(const std::string& filename) {
    auto& file = SetupFile(filename);

    file.AddVar(SEED, "seed", "Random generator seed");

    file.AddVar(update, "update" , "Update");

    file.AddTotal(
      dn_repdecline,
      "total_repdecline",
      "TODO",
      true
    );

    file.AddTotal(
      dn_apoptosis,
      "total_apoptosis",
      "TODO",
      true
    );

    file.AddTotal(
      dn_mutation,
      "total_mutation",
      "TODO",
      true
    );

    file.AddTotal(
      dn_reproduce,
      "total_reproduce",
      "TODO",
      true
    );

    for (size_t i = 0; i < dns_channelrep.size(); ++i) {
      file.AddTotal(
        dns_channelrep[i],
        "total_channelrep"+std::to_string(i),
        "TODO",
        true
      );
    }

    for (size_t lev = 0; lev < NLEV; ++lev) {
      file.AddFun<size_t>(
          [this, lev](){ return channel.GetChannelCount(lev); },
          "num_channels"+std::to_string(lev),
          "TODO"
        );
    }

    file.PrintHeaderKeys();

    return file;
  }

  /*
   * Setup our data nodes to collect genotypic snapshots. During a run, data
   * are added to these nodes through a DataPull registered to the OnUpdate
   * signal.
   */
  void inline SetupGenotypeData(DishtinyConfig& dconfig) {
    // example organism
    Organism& examp = emp::World<Organism>::GetOrg(0);

    const size_t gdata_freq = dconfig.GDATA_FREQ();

    // setup data nodes
    for (size_t i = 0; i < examp.GetEndowmentDepth(); ++i) {
      dns_endowment.push_back(dnod_genotype_t());
      dns_endowment[i].AddPullSet([this, i](){
        emp::vector<double> res;
        for(
          auto it = emp::World<Organism>::begin();
          it != emp::World<Organism>::end();
          ++it
        ) {
          res.push_back((*it).GetEndowment(i));
        }
        return res;
      });
      OnUpdate([this, i, gdata_freq](size_t update){
        if (update%gdata_freq == 0) {
          dns_endowment[i].Reset();
          dns_endowment[i].PullData();
        }
      });
    }

    for (size_t i = 0; i < examp.GetResPoolDepth(); ++i) {
      dns_res_pool.push_back(dnod_genotype_t());
      dns_res_pool[i].AddPullSet([this, i](){
        emp::vector<double> res;
        for(
          auto it = emp::World<Organism>::begin();
          it != emp::World<Organism>::end();
          ++it
        ) {
          res.push_back((*it).GetResPool(i));
        }
        return res;
      });
      OnUpdate([this, i, gdata_freq](size_t update){
        if (update%gdata_freq == 0) {
          dns_res_pool[i].Reset();
          dns_res_pool[i].PullData();
        }
      });
    }

    for (size_t i = 0; i < examp.GetAvoidOverDepth(); ++i) {
      dns_avoid_over.push_back(dnod_genotype_t());
      dns_avoid_over[i].AddPullSet([this, i](){
        emp::vector<double> res;
        for(
          auto it = emp::World<Organism>::begin();
          it != emp::World<Organism>::end();
          ++it
        ) {
          res.push_back((*it).GetAvoidOver(i));
        }
        return res;
      });
      OnUpdate([this, i, gdata_freq](size_t update){
        if (update%gdata_freq == 0) {
          dns_avoid_over[i].Reset();
          dns_avoid_over[i].PullData();
        }
      });
    }

    for (size_t i = 0; i < examp.GetOffChCapDepth(); ++i) {
      dns_off_ch_cap.push_back(dnod_genotype_t());
      dns_off_ch_cap[i].AddPullSet([this, i](){
        emp::vector<double> res;
        for(
          auto it = emp::World<Organism>::begin();
          it != emp::World<Organism>::end();
          ++it
        ) {
          res.push_back((*it).GetOffChCap(i));
        }
        return res;
      });
      OnUpdate([this, i, gdata_freq](size_t update){
        if (update%gdata_freq == 0) {
          dns_off_ch_cap[i].Reset();
          dns_off_ch_cap[i].PullData();
        }
      });
    }

    for (size_t i = 0; i < examp.GetSortOffDepth(); ++i) {
      dns_sort_off.push_back(dnod_genotype_t());
      dns_sort_off[i].AddPullSet([this, i](){
        emp::vector<double> res;
        for(
          auto it = emp::World<Organism>::begin();
          it != emp::World<Organism>::end();
          ++it
        ) {
          res.push_back((*it).GetSortOff(i));
        }
        return res;
      });
      OnUpdate([this, i, gdata_freq](size_t update){
        if (update%gdata_freq == 0) {
          dns_sort_off[i].Reset();
          dns_sort_off[i].PullData();
        }
      });
    }

    dn_damage_suicide.AddPullSet([this](){
      emp::vector<double> res;
      for(
        auto it = emp::World<Organism>::begin();
        it != emp::World<Organism>::end();
        ++it
      ) {
        res.push_back((*it).GetDamageSuicide());
      }
      return res;
    });
    OnUpdate([this, gdata_freq](size_t update){
      if (update%gdata_freq == 0) {
        dn_damage_suicide.Reset();
        dn_damage_suicide.PullData();
      }
    });

  }

};
