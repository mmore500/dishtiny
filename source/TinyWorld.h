#pragma once

#include <utility>
#include <iostream>
#include <math.h>

#include "base/vector.h"
#include "base/Ptr.h"
#include "tools/Random.h"
#include "tools/random_utils.h"
#include "Evolve/World.h"
#include "base/assert.h"
#include "tools/File.h"

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
  const bool ECOLOGICAL;
  const double CULL_TARGET;
  const double CULL_PENALTY;
  const size_t CULL_FREQ;
  const size_t CULL_DELAY;
  const double RAINYDAY_PROPORTION;

  emp::vector<size_t> shuffler;
  emp::vector<emp::Ptr<std::pair<size_t, bool>>> neighborsorter;

  size_t birth_loc;

  std::map<Organism,size_t> org_counts;

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
  emp::vector<dnod_genotype_t> dns_damage_suicide;


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
  // for tracking the amount of resource collected
  emp::DataNode<double,
      emp::data::Current, emp::data::Range,
      emp::data::Pull, emp::data::Log
    > dn_resource_harvested;
  // for tracking the amount of activation cost paid
  emp::DataNode<double,
      emp::data::Current, emp::data::Range,
      emp::data::Pull, emp::data::Log
    > dn_cost;
  // for tracking the amount of resource lost by death
  emp::DataNode<double,
      emp::data::Current, emp::data::Range,
      emp::data::Pull, emp::data::Log
    > dn_resource_lost;
  // for tracking the number of new channel reproductions
  emp::vector<dnod_phenotype_t> dns_channelrep;

  size_t channel_map_iterator;

public:
  TinyWorld(
    int _nupdate,
    emp::Random& _rand,
    DishtinyConfig& dconfig,
    CustomConfig& cconfig)
  : emp::World<Organism>(_rand, "TinyWorld")
  , nupdate(_nupdate)
  , rand(_rand)
  , spec(dconfig, &rand)
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
  , ECOLOGICAL(dconfig.ECOLOGICAL())
  , CULL_TARGET(dconfig.CULL_TARGET())
  , CULL_PENALTY(dconfig.CULL_PENALTY())
  , CULL_FREQ(dconfig.CULL_FREQ())
  , CULL_DELAY(dconfig.CULL_DELAY())
  , RAINYDAY_PROPORTION(dconfig.RAINYDAY_PROPORTION())
  , shuffler(emp::GetPermutation(rand, GRID_A))
  , neighborsorter()
  , org_counts()
  , dns_channelrep(dconfig.NLEV())
  {

    // add one element for all four neighbors
    for (size_t i = 0; i < 4; ++i) {
      neighborsorter.push_back(emp::NewPtr<std::pair<size_t, bool>>());
    }

    // this also sets the world to asynchronous mode
    emp::World<Organism>::SetPopStruct_Grid(dconfig.GRID_W(), dconfig.GRID_H());

    emp::World<Organism>::OnOrgDeath( [this](size_t pos) {
      // set channels to DEAD
      // if org was last of particular channel, remove that channel's res pool
      double res = channel.Kill(
        pos,
        [this](size_t lev, int ch){ return store.ErasePool(lev, ch); }
      );
      // erase organism's resource stockpile
      res += store.EraseStockpile(pos);

      // store the total amount of resorce destroyed
      dn_resource_lost.Add(-res);

    } );

    emp::World<Organism>::SetAddBirthFun(
      [this](emp::Ptr<Organism> new_org, emp::WorldPosition parent_id) {
        return birth_loc;
    } );

    if (dconfig.SYSTEMATICS()) {
      emp::Ptr<emp::Systematics<Organism,Organism>> sys
        = new emp::Systematics<Organism,Organism>(
          [](Organism & o){return o;}, true, true, false
        );
      emp::World<Organism>::AddSystematics(sys,"systematics");
    }

    // populate the world
    if (dconfig.SEED_POP()) {
      // open file with genotypes we want to seed
      emp::File source = emp::File(dconfig.SEED_POP_FILENAME());

      // first line is just the header keys
      // last line is empty
      const size_t source_count = source.GetNumLines() - 2;
      emp_assert(source_count > 0);
      // we are seeding SEED_POP_CLONECOUNT orgs per genotype
      const size_t n_seeded = source_count * dconfig.SEED_POP_CLONECOUNT();
      emp_assert(n_seeded > 0);

      // read in all the genotype strings
      emp::vector<std::string> string_genotypes;
      auto it = source.begin();
      ++it; // first line is just the header keys
      // last line is just an empty line
      for (; it != source.end() - 1; ++it) {
        // we want to seed multiple copies of each genotype
        for (size_t r = 0; r < dconfig.SEED_POP_CLONECOUNT(); ++r) {
          string_genotypes.push_back(*it);
        }
      }

      // shuffle the order that we insert seeded genotypes
      emp::Shuffle(rand, string_genotypes);

      // how many to skip per insert along one grid dimension
      size_t side_density = sqrt(GRID_A / n_seeded);

      // seed the genotypes evenly throughout the space
      auto sg = string_genotypes.begin();
      for (
        size_t x = 0;
        x < spec.GetWidth() && sg != string_genotypes.end();
        x += side_density
      ) {
        for (
          size_t y = 0;
          y < spec.GetHeight() && sg != string_genotypes.end();
          y += side_density
        ) {
          emp::World<Organism>::InjectAt(
              Organism(*sg, &rand, dconfig, &cconfig),
              spec.GetID(x,y)
            );
          channel.InitializeCell(spec.GetID(x,y));
          ++sg;
        }
      }
    } else {
      // generate a new set of random genotypes
      for (size_t cell = 0; cell < GRID_A; ++cell) {
        emp::World<Organism>::InjectAt(
            Organism(&rand, dconfig, &cconfig),
            cell
          );
        channel.InitializeCell(cell);
      }
    }

    // setup data nodes and data files
    if (dconfig.SYSTEMATICS()) {

      auto& sf = emp::World<Organism>::SetupSystematicsFile(
        "systematics",
        "Systematics_"
        +std::to_string(dconfig.SEED())
        +".csv",
        false
      );

      sf.SetTimingRepeat(
        dconfig.GDATA_FREQ()
      );
      sf.AddVar(
        SEED, "seed", "Random generator seed"
      );
      sf.PrintHeaderKeys();

    }

    auto& pf = emp::World<Organism>::SetupPopulationFile(
      "Population_"
      +std::to_string(dconfig.SEED())
      +".csv",
      false
    );
    pf.SetTimingRepeat(
      dconfig.PDATA_FREQ()
    );
    pf.AddVar(
      SEED, "seed", "Random generator seed"
    );
    pf.PrintHeaderKeys();

    SetupGenotypeData(dconfig);
    SetupGenotypeFile(
      "Genotypes_"
      +std::to_string(dconfig.SEED())
      +".csv"
    ).SetTimingRepeat(dconfig.GDATA_FREQ());

    SetupCensusFile(
      "Census_"
      +std::to_string(dconfig.SEED())
      +".csv"
    ).SetTimingRepeat(dconfig.CDATA_FREQ());

    SetupPhenotypeFile(
      "Phenotypes_"
      +std::to_string(dconfig.SEED())
      +".csv"
    ).SetTimingRepeat(dconfig.PDATA_FREQ());

    SetupChannelMapFile(
      "ChannelMap_"
      +std::to_string(dconfig.SEED())
      +".csv",
      dconfig
    );
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

    dn_resource_harvested.Add(store.Harvest(signal, resource, channel, *this));

    dn_cost.Add(store.PayStateCost(signal, resource, channel, *this));

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

    // cull


    if (
      emp::World<Organism>::GetUpdate() % CULL_FREQ == 0 && emp::World<Organism>::GetUpdate() > CULL_DELAY
    ) {
      while (
        ((double) this->GetNumOrgs()) / ((double) spec.GetArea())
        > 1 - CULL_TARGET
      ) {
        int sel_ch = channel.DrawLottery(NLEV-1);
        store.TransactPool(NLEV-1, sel_ch, CULL_PENALTY);
        store.SettlePools(channel);
        store.KillDebtors(*this);
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

#ifdef EMSCRIPTEN
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
#endif

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
    Organism child = Organism(emp::World<Organism>::GetOrg(parent));
    bool mut = false;
    if (!ECOLOGICAL) mut |= child.DoMutations(rand);

    // takes care of killing trampled cell
    // birth_loc is hooked into DoBirth function through Lambda in this scope
    birth_loc = dest;
    emp::World<Organism>::DoBirth(child, parent);

    // copy over channels on levels at and above off_level
    channel.Spawn(parent, dest, off_level, dns_channelrep);

    // give endowment
    store.MultilevelTransaction(emp::World<Organism>::GetOrg(dest), dest, channel, endowment);

    // if suicide on damage triggered, do it and log it
    if (mut) {
      dn_mutation.Add(1);
      if (rand.GetDouble() < child.GetDamageSuicide(off_level)
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
      res &= (avail_resource >= RAINYDAY_PROPORTION * org.GetEndowment(tlev) + REP_THRESH);

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
   * Setup our data file to collect genotypic information.
   */
  emp::DataFile& SetupGenotypeFile(const std::string& filename) {
    auto& file = emp::World<Organism>::SetupFile(filename);

    file.AddVar(SEED, "seed", "Random generator seed");

    file.AddVar(emp::World<Organism>::update, "update" , "Update");

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

    for (size_t i = 0; i < dns_damage_suicide.size(); ++i) {
      file.AddMean(
        dns_damage_suicide[i],
        "mean_damage_suicide"+std::to_string(i),
        "TODO"
      );
    }

    file.PrintHeaderKeys();

    return file;
  }

  void UpdateOrgCounts() {
    org_counts.clear();
    for (emp::Ptr<Organism> org : emp::World<Organism>::pop) {
       if (org) org_counts[*org] = 0;  // Initialize needed entries
    }
    for (emp::Ptr<Organism> org : emp::World<Organism>::pop) {
      if (org) org_counts[*org] += 1; // Count actual types.
    }
  }

  /*
   * Setup our data file to collect a genotypic census.
   */
  emp::DataFile& SetupCensusFile(const std::string& filename) {

    emp::Ptr<emp::ContainerDataFile<std::map<Organism,size_t>>> fp = new emp::ContainerDataFile<std::map<Organism,size_t>>(filename);

    emp::ContainerDataFile<std::map<Organism,size_t>>& file = *fp;

    emp::World<Organism>::AddDataFile(fp);

    file.SetUpdateContainerFun([this](){
        UpdateOrgCounts();
        return org_counts;
      });

    file.AddVar(SEED, "seed", "Random generator seed");

    file.AddVar(emp::World<Organism>::update, "update" , "Update");

    for (size_t i = 0; i < NLEV+Organism::ENDOWMENT_DEPTH_ADJ; ++i) {
      std::function<double(std::pair<const Organism, size_t>)> fun =
        [i](std::pair<const Organism, size_t> x){
          return x.first.GetEndowment(i);
          };
      file.AddContainerFun(
        fun,
        "endowment"+std::to_string(i),
        "TODO"
      );
    }

    for (size_t i = 0; i < NLEV+Organism::RESPOOL_DEPTH_ADJ; ++i) {
      std::function<double(std::pair<const Organism, size_t>)> fun =
        [i](std::pair<const Organism, size_t> x){
          return x.first.GetResPool(i);
          };
      file.AddContainerFun(
        fun,
        "res_pool"+std::to_string(i),
        "TODO"
      );
    }

    for (size_t i = 0; i < NLEV+Organism::AVOIDOVER_DEPTH_ADJ; ++i) {
      std::function<double(std::pair<const Organism, size_t>)> fun =
        [i](std::pair<const Organism, size_t> x){
          return x.first.GetAvoidOver(i);
          };
      file.AddContainerFun(
        fun,
        "avoid_over"+std::to_string(i),
        "TODO"
      );
    }

    for (size_t i = 0; i < NLEV+Organism::OFFCHCAP_DEPTH_ADJ; ++i) {
      std::function<double(std::pair<const Organism, size_t>)> fun =
        [i](std::pair<const Organism, size_t> x){
          return x.first.GetOffChCap(i);
          };
      file.AddContainerFun(
        fun,
        "off_ch_cap"+std::to_string(i),
        "TODO"
      );
    }

    for (size_t i = 0; i < NLEV+Organism::SORTOFF_DEPTH_ADJ; ++i) {
      std::function<double(std::pair<const Organism, size_t>)> fun =
        [i](std::pair<const Organism, size_t> x){
          return x.first.GetSortOff(i);
          };
      file.AddContainerFun(
        fun,
        "sort_off"+std::to_string(i),
        "TODO"
      );
    }

    for (size_t i = 0; i < NLEV+Organism::DAMAGESUICIDE_DEPTH_ADJ; ++i) {
      std::function<double(std::pair<const Organism, size_t>)> fun =
        [i](std::pair<const Organism, size_t> x){
          return x.first.GetDamageSuicide(i);
          };
      file.AddContainerFun(
        fun,
        "damage_suicide"+std::to_string(i),
        "TODO"
      );
    }

    std::function<size_t(std::pair<const Organism, size_t>)> func =
      [](std::pair<const Organism, size_t> x){return x.second;};
    file.AddContainerFun(
      func,
      "count",
      "TODO"
    );

    file.PrintHeaderKeys();

    return file;
  }

  /*
   * Setup our data file to collect intermittent channel maps.
   */
  emp::DataFile& SetupChannelMapFile(
    const std::string& filename,
    DishtinyConfig& dconfig
  ) {

    auto& file = emp::World<Organism>::SetupFile(filename);

    const size_t cdata_freq = dconfig.CDATA_FREQ();
    const size_t chanmap_dur = dconfig.CHANMAP_DUR();
    const size_t chanmap_timelapse_freq = dconfig.CHANMAP_TIMELAPSE_FREQ();

    file.AddVar(SEED, "seed", "Random generator seed");

    file.AddVar(emp::World<Organism>::update, "update" , "Update");

    file.AddFun<int>([this](){
      return spec.GetX(channel_map_iterator);
    },"ch_x","TODO");

    file.AddFun<int>([this](){
      return spec.GetY(channel_map_iterator);
    },"ch_y","TODO");

    for (size_t lev = 0; lev < NLEV; ++lev) {
      file.AddFun<int>([this, lev](){
        return channel.GetChannel(lev, channel_map_iterator);
      },"channel"+std::to_string(lev),"TODO");
    }

    file.PrintHeaderKeys();

    // we will write our own timing function and register it with OnUpdate
    file.SetTiming([](size_t update){ return false; });

    emp::World<Organism>::OnUpdate([this, cdata_freq, chanmap_dur, chanmap_timelapse_freq, &file](size_t update){
      if (
        (update%cdata_freq < chanmap_dur)
        || (update%chanmap_timelapse_freq == 0)
      ) {
        for (size_t cell = 0; cell < GRID_A; ++cell) {
          channel_map_iterator = cell;
          file.Update();
        }
      }
    });

    return file;
  }

  /*
   * Setup our data file to collect phenotypic information.
   */
  emp::DataFile& SetupPhenotypeFile(const std::string& filename) {
    auto& file = emp::World<Organism>::SetupFile(filename);

    file.AddVar(SEED, "seed", "Random generator seed");

    file.AddVar(emp::World<Organism>::update, "update" , "Update");

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

    file.AddTotal(
      dn_resource_harvested,
      "total_resource_harvested",
      "TODO",
      true
    );

    file.AddTotal(
      dn_resource_lost,
      "total_resource_lost",
      "TODO",
      true
    );

    file.AddTotal(
      dn_cost,
      "total_cost",
      "TODO",
      true
    );

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
    const size_t cdata_freq = dconfig.CDATA_FREQ();

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
      emp::World<Organism>::OnUpdate([this, i, gdata_freq, cdata_freq](size_t update){
        if (update%gdata_freq == 0 || update%cdata_freq == 0) {
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
      emp::World<Organism>::OnUpdate([this, i, gdata_freq, cdata_freq](size_t update){
        if (update%gdata_freq == 0 || update%cdata_freq == 0) {
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
      emp::World<Organism>::OnUpdate([this, i, gdata_freq, cdata_freq](size_t update){
        if (update%gdata_freq == 0 || update%cdata_freq == 0) {
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
      emp::World<Organism>::OnUpdate([this, i, gdata_freq, cdata_freq](size_t update){
        if (update%gdata_freq == 0 || update%cdata_freq == 0) {
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
      emp::World<Organism>::OnUpdate([this, i, gdata_freq, cdata_freq](size_t update){
        if (update%gdata_freq == 0 || update%cdata_freq == 0) {
          dns_sort_off[i].Reset();
          dns_sort_off[i].PullData();
        }
      });
    }

    for (size_t i = 0; i < examp.GetDamageSuicideDepth(); ++i) {
      dns_damage_suicide.push_back(dnod_genotype_t());
      dns_damage_suicide[i].AddPullSet([this, i](){
        emp::vector<double> res;
        for(
          auto it = emp::World<Organism>::begin();
          it != emp::World<Organism>::end();
          ++it
        ) {
          res.push_back((*it).GetDamageSuicide(i));
        }
        return res;
      });
      emp::World<Organism>::OnUpdate([this, i, gdata_freq, cdata_freq](size_t update){
        if (update%gdata_freq == 0 || update%cdata_freq == 0) {
          dns_damage_suicide[i].Reset();
          dns_damage_suicide[i].PullData();
        }
      });
    }


  }

};
