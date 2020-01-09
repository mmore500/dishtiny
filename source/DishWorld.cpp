#pragma once

#include <tuple>
#include <utility>
#include <limits>
#ifdef EMSCRIPTEN
#include <experimental/filesystem>
#else
#include <filesystem>
#endif

#include "base/vector.h"
#include "Evolve/World.h"
#include "tools/Random.h"
#include "tools/hash_utils.h"
#include "tools/BitSet.h"
#include "tools/keyname_utils.h"
#include "tools/string_utils.h"

#include "Cardi.h"
#include "DishWorld.h"
#include "Config.h"
#include "FrameCell.h"
#include "Genome.h"
#include "LibraryEvent.h"
#include "LibraryInstruction.h"
#include "LibraryInstructionSpiker.h"
#include "Manager.h"
#include "Mutator.h"


DishWorld::DishWorld(const Config &cfg_, size_t uid_offset/*=0*/)
: cfg(cfg_)
, mut(cfg_)
, inst_lib(LibraryInstruction::Make(cfg))
, inst_lib_spiker(LibraryInstructionSpiker::Make(cfg))
, event_lib(LibraryEvent::Make(cfg))
{
  SetPopStruct_Grid(cfg.GRID_W(), cfg.GRID_H());

  emp::Random rand_init(cfg.SEED());
  for(size_t i = 0; i < GetSize(); ++i) {
    local_rngs.push_back(
      emp::NewPtr<emp::Random>(rand_init.GetInt(
        1,
        1000000000
      ))
    );
    global_rngs.push_back(emp::NewPtr<emp::Random>(cfg.SEED()));
  }

  man = emp::NewPtr<Manager>(*this, local_rngs, global_rngs, cfg);

  for(size_t i = 0; i < GetSize(); ++i) {
    frames.push_back(emp::NewPtr<FrameCell>(
      *local_rngs[i],
      cfg,
      *man,
      i,
      inst_lib,
      inst_lib_spiker,
      event_lib
    ));
  }

  // OnOffspringReady([](){;}});

  OnOrgDeath([this](const size_t pos){
    double amt = std::max(0.0,man->Stockpile(pos).QueryResource());
    if (man->Apoptosis(pos).IsMarked()) {
      amt += cfg.REP_THRESH() * cfg.APOP_RECOVERY_FRAC();
    }

    const auto neighs = GeometryHelper(cfg).CalcLocalNeighs(pos);

    const size_t h_count = man->Heir(pos).HeirCount();
    if(h_count) {
      for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
        if(man->Heir(pos).IsHeir(dir)) {
          man->Stockpile(neighs[dir]).InternalAcceptResource(amt/h_count);
        }
      }
    }
    man->Stockpile(pos).Reset();
    man->Sharing(pos).Reset();
    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      man->Priority(neighs[dir]).ClearPauses(Cardi::Opp[dir]);
    }
    man->Connection(pos).Reset();
  });

  OnPlacement([this](const size_t pos){
    man->Apoptosis(pos).Reset();
    man->Stockpile(pos).Reset();
    man->Sharing(pos).Reset();
    man->Heir(pos).Reset();
    man->Priority(pos).Reset();
    frames[pos]->Reset();
    frames[pos]->SetProgram(GetOrg(pos).GetProgram());
    frames[pos]->SetProgramSpiker(GetOrg(pos).GetProgramSpiker());
    man->Inbox(pos).ClearInboxes();
  });

  OnUpdate([this](size_t upd){ Step(); });

  InitSystematics();

  if (cfg.SEED_POP()) {
    LoadPopulation();
  } else {
    GeneratePopulation();
  }

}

void DishWorld::GeneratePopulation() {
  for(size_t i = 0; i < GetSize(); ++i) {
    Genome g(
      *local_rngs[i],
      inst_lib,
      inst_lib_spiker,
      cfg
    );
    InjectAt(g, emp::WorldPosition(i));
    emp_assert(GetOrg(i).GetProgram().GetSize());
    man->Stockpile(i).InternalAcceptResource(cfg.START_RESOURCE());
  }
}

void DishWorld::LoadPopulation() {

  #ifdef EMSCRIPTEN
  auto directory = std::experimental::filesystem::directory_iterator("./seedpop/");
  emp::vector<std::string> filenames;
  std::transform(
    std::experimental::filesystem::begin(directory),
    std::experimental::filesystem::end(directory),
    std::back_inserter(filenames),
    [](const auto & entry){ return entry.path(); }
  );
  #else
  auto directory = std::filesystem::directory_iterator("./seedpop/");
  emp::vector<std::string> filenames;
  std::transform(
    std::filesystem::begin(directory),
    std::filesystem::end(directory),
    std::back_inserter(filenames),
    [](const auto & entry){ return entry.path(); }
  );
  #endif

  // get ids of seeded cells
  emp::vector<size_t> ids;
  for (const auto & filename : filenames) {
    if (
      const auto res = emp::keyname::unpack(filename);
      res.count("id")
    ) {
      size_t id;
      std::stringstream(res.at("id")) >> id;
      emp_assert(id > 0);
      ids.push_back(id);
    }
  }

  // remove duplicate ids
  std::sort(std::begin(ids), std::end(ids));
  ids.erase(
    std::unique(std::begin(ids), std::end(ids)),
    std::end(ids)
  );

  // if single seed ID configured, drop all others
  if (cfg.SEED_POP_ID()) {
    ids.erase(
      std::remove_if(
        std::begin(ids),
        std::end(ids),
        [this](const auto & id){ return id != cfg.SEED_POP_ID(); }
      ),
      std::end(ids)
    );
  }

  // pick where to put seeded cells
  emp::Random rand(cfg.SEED());
  emp::vector<size_t> targets = emp::Choose(
    rand,
    GetSize(),
    cfg.SEED_POP_CLONECOUNT() * ids.size()
  );

  auto target = std::begin(targets);

  for (const auto & id : ids) {

    std::ifstream genome_stream(
      *std::find_if(
        std::begin(filenames),
        std::end(filenames),
        [this, id](const auto & filename){
          const auto res = emp::keyname::unpack(filename);

          return (
            res.count("id") && res.at("id") == emp::to_string(id)
            && res.count("treat")
            && res.at("treat") == cfg.TREATMENT_DESCRIPTOR()
            && res.count("component") && res.at("component") == "genome"
            && res.count("ext") && res.at("ext") == ".txt"
          );
        }
      )
    );

    cereal::JSONInputArchive genome_archive(genome_stream);
    Genome genome(
      cfg,
      LibraryInstruction::Make(cfg),
      LibraryInstructionSpiker::Make(cfg)
    );
    genome_archive(genome);

    for (size_t clone = 0; clone < cfg.SEED_POP_CLONECOUNT(); ++clone) {
      InjectAt(
        genome,
        emp::WorldPosition(
          *(target++)
        )
      );
    }
  }

  // turn off restoring regulator state for now
  // target = std::begin(targets);
  //
  // for (const auto & id : ids) {
  //
  //   for (size_t clone = 0; clone < cfg.SEED_POP_CLONECOUNT(); ++clone) {
  //
  //     for (size_t dir = 0; dir < Cardi::Dir::NumDirs + 1; ++dir) {
  //       Config::matchbin_t::state_t state;
  //       std::ifstream reg_stream(
  //         *std::find_if(
  //           std::begin(filenames),
  //           std::end(filenames),
  //           [this, id, dir](const auto & filename){
  //             const auto res = emp::keyname::unpack(filename);
  //             return (
  //               res.count("id") && res.at("id") == emp::to_string(id)
  //               && res.count("treat")
  //               && res.at("treat") == cfg.TREATMENT_DESCRIPTOR()
  //               && res.count("dir") && res.at("dir") == emp::to_string(dir)
  //               && res.count("component") && res.at("component") == "regulator"
  //               && res.count("ext") && res.at("ext") == ".json"
  //             );
  //           }
  //         )
  //       );
  //       cereal::JSONInputArchive reg_archive(reg_stream);
  //       reg_archive(state);
  //       frames[*target]->GetFrameHardware(
  //         dir
  //       ).SetMatchBinState(state);
  //     }
  //
  //     ++target;
  //
  //   }
  //
  // }

  // kill everything that's not a target
  std::unordered_set<size_t> target_set(
    std::begin(targets),
    std::end(targets)
  );

  for (size_t i = 0; i < GetSize(); ++i) {
    if (!target_set.count(i)) {
      DoDeath(i);
      man->Channel(i).ClearIDs();
    }
  }

}

DishWorld::~DishWorld() {
  Clear();
  for (auto &ptr : global_rngs) ptr.Delete();
  for (auto &ptr : local_rngs) ptr.Delete();
  for (auto &ptr : frames) ptr.Delete();
  man.Delete();
}

void DishWorld::InitSystematics() {

  if (cfg.SYSTEMATICS()) {

    AddSystematics(
      emp::NewPtr<emp::Systematics<Genome,Genome>>(
        [](Genome & o){return o;},
        true,
        true,
        false
      ),
      "systematics"
    );

    #ifndef EMSCRIPTEN
    auto& sf = SetupSystematicsFile(
      "systematics",
      emp::keyname::pack({
        {"title", "systematics"},
        {"treat", cfg.TREATMENT_DESCRIPTOR()},
        {"seed", emp::to_string(cfg.SEED())},
        {"_emp_hash", STRINGIFY(EMPIRICAL_HASH_)},
        {"_source_hash", STRINGIFY(DISHTINY_HASH_)},
        {"ext", ".csv"}
      }),
      false
    );

    sf.SetTiming([this](const size_t upd){ return cfg.TimingFun(upd); });
    sf.AddVar(cfg.SEED(), "seed", "Random generator seed");
    sf.PrintHeaderKeys();
    #endif

  }

}

void DishWorld::Step() {

  // resolve pending state from last update
  for (size_t i = 0; i < GetSize(); ++i) {

    // only process cell interactions every ENV_TRIG_FREQ updates
    if (GetUpdate() % cfg.ENV_TRIG_FREQ()) break;

    if (
      const auto opt_sirepack = man->Priority(i).QueryPendingGenome();
      opt_sirepack
    ) {

      if (
        (opt_sirepack->replev == cfg.NLEV()) &&
        (local_rngs[i]->GetDouble() < cfg.PROPAGULE_MUTATION_RATE())
      ) {
        opt_sirepack->genome->DoMutations(mut,*local_rngs[i]);
      } else if (local_rngs[i]->GetDouble() < cfg.MUTATION_RATE()) {
        opt_sirepack->genome->DoMutations(mut, *local_rngs[i]);
      }

      const size_t dest = (
        cfg.LOCAL_REPRODUCTION()
        ? i
        : (
            local_rngs[i]->GetUInt(1, GetSize())
            + opt_sirepack->par_pos
        ) % GetSize()
      );

      AddOrgAt(opt_sirepack->genome, dest, opt_sirepack->par_pos);

      // disable epigenetic inheritance for propagule-generating reproduction
      // this makes harvesting and restarting propagules more straightforward
      if (opt_sirepack->replev != cfg.NLEV()) {
        frames[dest]->SetRegulators(opt_sirepack->matchbins);
      }

      man->Channel(dest).Inherit(
        opt_sirepack->chanpack,
        opt_sirepack->channel_gens,
        opt_sirepack->replev
      );
      man->Family(dest).Reset(GetUpdate());
      man->Family(dest).SetParentPos(opt_sirepack->par_pos);
      man->Family(dest).SetPrevChan(opt_sirepack->prev_chan);
      man->Family(dest).SetCellGen(opt_sirepack->cell_gen, opt_sirepack->replev);
      // check that parent hasn't been overwritten by a different birth
      if(man->Family(opt_sirepack->par_pos).GetBirthUpdate() != GetUpdate()) {
        man->Family(opt_sirepack->par_pos).AddChildPos(dest);
        // parental generation bump disabled
        // man->Channel(opt_sirepack->par_pos).LogReprGen(opt_sirepack->replev);
      }

      man->Priority(i).ClearPendingGenome();

    } else if (IsOccupied(i)) {
      // this block doesn't get run if a cell was just born here
      if (man->Stockpile(i).IsBankrupt()
          || man->Apoptosis(i).IsMarked()) {
        DoDeath(i);
        if (!man->Apoptosis(i).IsMarkedPartial()) {
          man->Channel(i).ClearIDs();
        }
      }
    }
  }

  for (size_t i = 0; i < GetSize(); ++i) {
    for (size_t r = 0; r < cfg.WAVE_REPLICATES(); ++r) {
      // dead cells with no channels have nullopt so no transmission will occur
      for (size_t l = 0; l < cfg.NLEV(); ++l) man->Wave(r,i,l).ResolveNext();
    }
  }

  // clean up for next cell action step
  #pragma omp parallel for
  for (size_t i = 0; i < GetSize(); ++i) {

    man->Connection(i).SearchAndDevelop();

    if (GetUpdate()%cfg.ENV_TRIG_FREQ()==0) {
      man->Heir(i).DecayHeirs();
      man->Channel(i).DecayCellAgeBoosters();
      man->Priority(i).ResolveUpdate();
      man->Apoptosis(i).Reset();
      man->Sharing(i).ResolveNextResistance(GetUpdate());
      man->Sharing(i).ProcessSharingRequest(GetUpdate());
    }

    for (size_t r = 0; r < cfg.WAVE_REPLICATES(); ++r) {
      for(size_t l = 0; l < cfg.NLEV(); ++l) {
        man->Wave(r,i,l).CalcNext(GetUpdate());
      }
    }

    if (IsOccupied(i)) {
      man->Stockpile(i).ResolveExternalContributions();
      man->Stockpile(i).ApplyBaseInflow();
      man->Stockpile(i).ApplyDecay();
      for (size_t r = 0; r < cfg.WAVE_REPLICATES(); ++r) {
        for(size_t l = 0; l < cfg.NLEV(); ++l) {
          man->Wave(r,i,l).HarvestResource();
        }
      }
      if (GetUpdate() % cfg.COMPUTE_FREQ() == 0) {
        frames[i]->QueueMessages(man->Inbox(i).GetInboxes());
      }

      if (
        cfg.GEN_INCR_FREQ()
        && man->Family(i).GetCellAge(GetUpdate())
        && man->Family(i).GetCellAge(GetUpdate()) % cfg.GEN_INCR_FREQ() == 0
      ) man->Channel(i).IncrGenCounts();
    }

  }

  // do cell action!
  #pragma omp parallel for
  for(size_t i = 0; i < GetSize(); ++i) {
    if (IsOccupied(i)) {
      frames[i]->Process(GetUpdate());
    }
  }

  // prepare for the next update
  // do it here instead of at the top so that DataHelper can view
  if (0 == (GetUpdate()+1) % cfg.ENV_TRIG_FREQ()) {
    for (size_t i = 0; i < GetSize(); ++i) {
      // decide who gets to be parent & make a copy of that genome
      // so that it doesn't get overwritten
      man->Priority(i).ResolveSire(man->Stockpile(i).QueryResource());
    }
  }

}

FrameCell & DishWorld::GetFrame(const size_t loc) {
  return *frames[loc];
}
