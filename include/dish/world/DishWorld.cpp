#pragma once
#ifndef DISHTINY_DISHWORLD_CPP_INCLUDE
#define DISHTINY_DISHWORLD_CPP_INCLUDE

#include <limits>
#include <stdexcept>
#include <tuple>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/source/base/vector.h"
#include "../../../third-party/Empirical/source/Evolve/World.h"
#include "../../../third-party/Empirical/source/tools/BitSet.h"
#include "../../../third-party/Empirical/source/tools/hash_utils.h"
#include "../../../third-party/Empirical/source/tools/keyname_utils.h"
#include "../../../third-party/Empirical/source/tools/Random.h"
#include "../../../third-party/Empirical/source/tools/random_utils.h"
#include "../../../third-party/Empirical/source/tools/string_utils.h"

#include "../config/Config.hpp"
#include "../genome/Genome.hpp"
#include "../genome/Mutator.hpp"
#include "../hardware/LibraryEvent.hpp"
#include "../hardware/LibraryInstruction.hpp"
#include "../hardware/LibraryInstructionSpiker.hpp"
#include "../manager/Manager.hpp"
#include "../topology/Cardi.hpp"
#include "../trait/FrameCell.hpp"

#include "DishWorld.hpp"

namespace dish {

DishWorld::DishWorld(const Config &cfg_, size_t uid_offset/*=0*/)
: cfg(cfg_)
, mut(cfg_)
, inst_lib(LibraryInstruction::Make(cfg))
, inst_lib_spiker(LibraryInstructionSpiker::Make(cfg))
, event_lib(LibraryEvent::Make(cfg))
{
  SetPopStruct_Grid(cfg.GRID_W(), cfg.GRID_H());

  emp::Random rand_init(cfg.SEED());
  for (size_t i = 0; i < GetSize(); ++i) {
    local_rngs.push_back(
      emp::NewPtr<emp::Random>(rand_init.GetInt(
        1,
        1000000000
      ))
    );
    global_rngs.push_back(emp::NewPtr<emp::Random>(cfg.SEED()));
  }

  man = emp::NewPtr<Manager>(*this, local_rngs, global_rngs, cfg);

  for (size_t i = 0; i < GetSize(); ++i) {
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

    // resolve any pending external contributions
    man->Stockpile(pos).ResolveExternalContributions();

    double amt = std::max(0.0, man->Stockpile(pos).QueryResource());

    if (man->Apoptosis(pos).IsMarked()) {
      amt += cfg.REP_THRESH() * cfg.APOP_RECOVERY_FRAC();
    }

    const auto neighs = GeometryHelper(cfg).CalcLocalNeighs(pos);

    // automatically set channelmates as heirs
    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      if (man->Channel(pos).CheckMatch(
        man->Channel(neighs[dir]),
        cfg.NLEV() - 1
      )) man->Heir(pos).SetHeir(dir, 2);
    }

    const size_t h_count = man->Heir(pos).HeirCount();
    if (h_count) {
      for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
        if (man->Heir(pos).IsHeir(dir)) {
          man->Stockpile(neighs[dir]).ExternalContribute(
            amt/h_count,
            Cardi::Opp[dir]
          );
        }
      }
    }
    man->Stockpile(pos).Reset();
    man->Sharing(pos).Reset();
    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      man->Priority(neighs[dir]).ClearPauses(Cardi::Opp[dir]);
    }
    frames[pos]->Reset(); // clear incoming connections
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
    man->Connection(pos).Reset();
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
    g.SetRootID(i);
    InjectAt(g, emp::WorldPosition(i));
    emp_assert(GetOrg(i).GetProgram().GetSize());
    man->Stockpile(i).InternalAcceptResource(cfg.START_RESOURCE());
  }
}

void DishWorld::LoadPopulation() {

  auto directory = std::filesystem::directory_iterator("./seedpop/");
  emp::vector<std::string> filenames;
  std::transform(
    std::filesystem::begin(directory),
    std::filesystem::end(directory),
    std::back_inserter(filenames),
    [](const auto & entry){ return entry.path(); }
  );

  if (!filenames.size()) {
    throw std::runtime_error("seedpop directory missing or empty");
  }

  // for consistency
  std::sort(std::begin(filenames), std::end(filenames));

  // filter just for filenames with ids
  std::remove_if(
    std::begin(filenames),
    std::end(filenames),
    [](const auto &f){ return emp::keyname::unpack(f).count("id") == 0; }
  );

  // get ids of seeded cells
  emp::vector<size_t> ids;
  std::transform(
    std::begin(filenames),
    std::end(filenames),
    std::back_inserter(ids),
    [](const auto &filename) {
      size_t id;
      std::stringstream(emp::keyname::unpack(filename).at("id")) >> id;
      return id;
    }
  );

  if (!ids.size()) {
    throw std::runtime_error("no ids found in seedpop directory");
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

  if (!ids.size()) {
    throw std::runtime_error("target id not found in seedpop directory");
  } else if (ids.size() > GetSize()) {
    throw std::runtime_error("too many ids found in seedpop directory");
  }

  // pick where to put seeded cells
  emp::vector<size_t> targets(GetSize());
  std::iota(targets.begin(), targets.end(), 0);

  emp::Random rand(cfg.SEED());
  emp::Shuffle(rand, targets);

  auto target = std::begin(targets);

  const size_t per_id_genome_quota = GetSize() / ids.size();

  // sort filenames by ids
  std::sort(
    std::begin(filenames),
    std::end(filenames),
    [](const auto &filename_a, const auto &filename_b){
      size_t id_a;
      size_t id_b;
      std::stringstream(emp::keyname::unpack(filename_a).at("id")) >> id_a;
      std::stringstream(emp::keyname::unpack(filename_b).at("id")) >> id_b;
      return id_a < id_b;
    }
  );

  for (const auto & id : ids) {

    std::cout << "id: " << id << std::endl;

    struct compare {
      bool operator()(const std::string& filename_a, const size_t id_b) const {
        size_t id_a;
        std::stringstream(emp::keyname::unpack(filename_a).at("id")) >> id_a;
        return id_a < id_b;
      }
      bool operator()(const size_t id_a, const std::string& filename_b) const {
        size_t id_b;
        std::stringstream(emp::keyname::unpack(filename_b).at("id")) >> id_b;
        return id_a < id_b;
      }
    };
    const auto cur_filenames = std::equal_range(
      std::begin(filenames),
      std::end(filenames),
      id,
      compare{}
    );
    emp_assert(cur_filenames.first != cur_filenames.second);

    const size_t per_file_genome_quota = per_id_genome_quota / std::distance(
      cur_filenames.first,
      cur_filenames.second
    );
    size_t leftover_genome_quota = per_id_genome_quota % std::distance(
      cur_filenames.first,
      cur_filenames.second
    );

    for (
      const auto & filename
      : emp::vector<std::string>{cur_filenames.first, cur_filenames.second}
    ) {

      const size_t genome_count = std::stoi(
        emp::keyname::unpack(filename).at("count")
      );

      if (genome_count == 0) {
        throw std::runtime_error("empty population file");
      }

      std::ifstream genomes_stream(filename);

      cereal::JSONInputArchive genomes_archive(genomes_stream);

      const size_t load_quota = (
        per_file_genome_quota
        + (leftover_genome_quota ? 1 : 0)
      );
      if (leftover_genome_quota) --leftover_genome_quota;

      const size_t base_load_copies = load_quota / genome_count;
      size_t leftover_load_copies = load_quota % genome_count;

      const size_t load_start = (
        load_quota * cfg.SEED_POP()
      ) % genome_count;

      std::cout
        << "   file " << filename
        << "   with " << genome_count << " genomes, "
        << " loading " << base_load_copies << "x "
        << " plus " << leftover_load_copies
        << " copies totaling " << load_quota
        << " genomes between population positions "
        << load_start << " and " << (load_start + load_quota) % genome_count
        << std::endl;

      for (size_t i = 0; i < genome_count; ++i) {
        Genome genome(
          cfg,
          LibraryInstruction::Make(cfg),
          LibraryInstructionSpiker::Make(cfg)
        );
        genomes_archive(genome);
        genome.SetRootID(id);

        if (rand.P(cfg.SEED_MUTATIONS_P())) genome.DoMutations(mut, rand);

        // decide whether to inject
        const size_t distance_from_start = (
          i >= load_start
          ? i - load_start
          : (genome_count - load_start) + i
        );

        if (distance_from_start < load_quota) {
          const size_t load_copies = (
            base_load_copies
            + (leftover_load_copies ? 1 : 0)
          );
          if (leftover_load_copies) --leftover_load_copies;

          for (size_t c = 0; c < load_copies; ++c) InjectAt(
            genome,
            emp::WorldPosition(
              *(target++)
            )
          );
        }

      } // end for loop over available genomes in file

    } // end for loop over available files for id

    emp_assert(
      static_cast<size_t>(std::distance(std::begin(targets), target))
      % per_id_genome_quota == 0
    );

  } // end for loop over available ids

  emp_assert(
    static_cast<size_t>(std::distance(std::begin(targets), target))
    == GetSize() - GetSize() % ids.size()
  );

  // clear everything that wasn't injected
  for (size_t i = 0; i < GetSize(); ++i) {
    if (!IsOccupied(i)) {
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

    #ifndef __EMSCRIPTEN__
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

    sf.SetTiming( [](const size_t update){ return update % 1024 == 0; } );
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
        frames[dest]->SetImprint(opt_sirepack->imprint);
        if (opt_sirepack->matchbins) {
          frames[dest]->SetRegulators(*(opt_sirepack->matchbins));
        }
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
      if (man->Family(opt_sirepack->par_pos).GetBirthUpdate() != GetUpdate()) {
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
      frames[i]->DoPrune();
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
      man->Connection(i).DoPrune();
      man->Connection(i).DecayQueries();
    }

    for (size_t r = 0; r < cfg.WAVE_REPLICATES(); ++r) {
      for (size_t l = 0; l < cfg.NLEV(); ++l) {
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
        frames[i]->QueueTrustedMessages(man->Inbox(i).GetTrustedInboxes());
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
  for (size_t i = 0; i < GetSize(); ++i) {
    if (IsOccupied(i)) {
      frames[i]->Process(GetUpdate());
    }
  }

  // prepare for the next update
  // do it here instead of at the top so that DataHelper can view
  for (size_t i = 0; i < GetSize(); ++i) {
    man->Sharing(i).ProcessSharingRequest(GetUpdate()+1);
    if (0 == (GetUpdate()+1) % cfg.ENV_TRIG_FREQ()) {
      // decide who gets to be parent & make a copy of that genome
      // so that it doesn't get overwritten
      man->Priority(i).ResolveSire(man->Stockpile(i).QueryResource());
    }
  }

}

FrameCell & DishWorld::GetFrame(const size_t loc) {
  return *frames[loc];
}

const Manager & DishWorld::GetManager() const {
  return *man;
}

} // namespace dish

#endif // #ifndef DISHTINY_DISHWORLD_CPP_INCLUDE
