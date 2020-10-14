#pragma once
#ifndef DISH_DATA_DATAHELPER_HPP_INCLUDE
#define DISH_DATA_DATAHELPER_HPP_INCLUDE

#include <chrono>
#include <iostream>
#include <sstream>

#include "H5Cpp.h"

#include "../../../third-party/cereal/include/cereal/archives/json.hpp"

#include "../../../third-party/Empirical/source/data/DataNode.h"
#include "../../../third-party/Empirical/source/polyfill/span.h"
#include "../../../third-party/Empirical/source/tools/keyname_utils.h"
#include "../../../third-party/Empirical/source/tools/QueueCache.h"
#include "../../../third-party/Empirical/source/tools/string_utils.h"

#include "../config/Config.hpp"
#include "../genome/Genome.hpp"
#include "../world/DishWorld.hpp"

#include "datahelper_tools.hpp"

namespace dish {

class DataHelper {

private:

  const Config &cfg;

  DishWorld &dw;

  H5::H5File file;

  H5Utils util;

public:

  DataHelper(DishWorld &dw_, const Config &cfg_)
  : cfg(cfg_)
  , dw(dw_)
  , file(emp::keyname::pack({
      {"title", "grid_snapshots"},
      {"treat", cfg.TREATMENT_DESCRIPTOR()},
      {"seed", emp::to_string(cfg.SEED())},
      {"_emp_hash", STRINGIFY(EMPIRICAL_HASH_)},
      {"_source_hash", STRINGIFY(DISHTINY_HASH_)},
      {"ext", ".h5"}
    })
    , H5F_ACC_TRUNC
    )
  , util(dw, cfg, file)
  {
    enum subdir{dir, lev, lev_1, none};

    std::vector<std::tuple<
      std::string,
      subdir
      >
    > folders {
        {"/RootID", subdir::none}
      , {"/Population", subdir::none}
      , {"/Triggers", subdir::none}
      , {"/Regulators", subdir::dir}
      , {"/Functions", subdir::dir}
      , {"/Index", subdir::none}
      , {"/Channel", subdir::lev}
      , {"/Expiration", subdir::lev}
      , {"/ChannelGeneration", subdir::lev}
      , {"/Stockpile", subdir::none}
      , {"/Live", subdir::none}
      , {"/Apoptosis", subdir::none}
      , {"/InboxActivation", subdir::dir}
      , {"/InboxTraffic", subdir::dir}
      , {"/TrustedInboxTraffic", subdir::dir}
      , {"/SpikeBroadcastTraffic", subdir::none}
      , {"/RepOutgoing", subdir::dir}
      , {"/RepIncoming", subdir::dir}
      , {"/TotalContribute", subdir::none}
      , {"/ResourceContributed", subdir::dir}
      , {"/ResourceHarvested", subdir::lev}
      , {"/PrevChan", subdir::none}
      , {"/InResistance", subdir::dir}
      , {"/OutResistance", subdir::dir}
      , {"/Heir", subdir::dir}
      , {"/ParentPos", subdir::none}
      , {"/CellAge", subdir::none}
      , {"/CellGen", subdir::lev_1}
      , {"/Death", subdir::none}
      , {"/OutgoingConnectionCount", subdir::none}
      , {"/FledglingConnectionCount", subdir::none}
      , {"/IncomingConnectionCount", subdir::none}
    };

    // create groups from vector
    for (const auto& [name, subtype] : folders) {
      // set up folder in file
      file.createGroup(name);

      // set up subfolders, if any
      switch(subtype) {
        case none:
          break;
        case dir:
          for(size_t d = 0; d < Cardi::Dir::NumDirs; ++d) {
            file.createGroup("/" + name + "/dir_" + emp::to_string(d));
          }
          break;
        case lev:
          for(size_t l = 0; l < cfg.NLEV(); ++l) {
            file.createGroup("/" + name + "/lev_" + emp::to_string(l));
          }
          break;
        case lev_1:
          for(size_t l = 0; l < cfg.NLEV() + 1; ++l) {
            file.createGroup("/" + name + "/lev_" + emp::to_string(l));
          }
          break;
      }
    }

    InitAttributes();
    InitReference();
    //util.InitDecoder("/Population/decoder", );
    util.InitDecoder("/Triggers/decoder", memtype::MakeArrayType());
    util.InitDecoder("/Regulators/decoder", memtype::MakeRegulatorData());
    util.InitDecoder("/Functions/decoder", memtype::MakeArrayType());

    file.flush(H5F_SCOPE_LOCAL);

  }

  ~DataHelper() { file.close(); }

  void SnapshotPopulation() {
    const size_t update = dw.GetUpdate();

    if (update % cfg.POPULATION() == 0) Population();
    if (update % cfg.TRIGGERS() == 0) Triggers();

    // save population best
    if (dw.GetNumOrgs()) {
      const auto org_count = dw.GetDominantInfo();
      std::ofstream genome_stream(
        emp::keyname::pack({
          {"component", "genome"},
          {"count", emp::to_string(org_count.second)},
          {"title", "dominant"},
          {"update", emp::to_string(dw.GetUpdate())},
          {"treat", cfg.TREATMENT_DESCRIPTOR()},
          {"seed", emp::to_string(cfg.SEED())},
          {"_emp_hash", STRINGIFY(EMPIRICAL_HASH_)},
          {"_source_hash", STRINGIFY(DISHTINY_HASH_)},
          {"ext", ".json"}
        })
      );
      cereal::JSONOutputArchive genome_archive(genome_stream);
      genome_archive(org_count.first);
    }

    emp::Random rand(cfg.SEED());
    emp::vector<size_t> shuffler(dw.GetSize());
    std::iota(std::begin(shuffler), std::end(shuffler), 0);
    emp::Shuffle(rand, shuffler);

    // save the entire population
    std::ofstream population_stream(
      emp::keyname::pack({
        {"title", "population"},
        {"count", emp::to_string(dw.GetNumOrgs())},
        {"component", "genomes"},
        {"update", emp::to_string(dw.GetUpdate())},
        {"treat", cfg.TREATMENT_DESCRIPTOR()},
        {"seed", emp::to_string(cfg.SEED())},
        {"_emp_hash", STRINGIFY(EMPIRICAL_HASH_)},
        {"_source_hash", STRINGIFY(DISHTINY_HASH_)},
        {"ext", ".json.cereal"}
      })
    );
    cereal::JSONOutputArchive population_archive(population_stream);
    for (const size_t i : shuffler) {
      if (dw.IsOccupied(i)) {
        population_archive(dw.GetOrg(i));
      }
    }

  }

  void SnapshotPhenotypes() {
    const size_t update = dw.GetUpdate();

    if (update % cfg.COMPUTE_FREQ() == 0) {
      // Regulators();
      Functions();
    }

    for (size_t lev = 0; lev < cfg.NLEV(); ++lev) {
      if (update % cfg.CHANNEL() == 0) Channel(lev);
      if (update % cfg.CHANNEL_GENERATION() == 0) ChannelGeneration(lev);
      if (update % cfg.EXPIRATION() == 0) Expiration(lev);
      if (update % cfg.RESOURCE_HARVESTED() == 0) ResourceHarvested(lev);
    }

    for (size_t lev = 0; lev < cfg.NLEV() + 1; ++lev) {
      if (update % cfg.CELL_GEN() == 0) CellGen(lev);
    }

    if (update % cfg.ROOT_ID() == 0) RootID();
    if (update % cfg.STOCKPILE() == 0) Stockpile();
    if (update % cfg.LIVE() == 0) Live();
    if (update % cfg.APOPTOSIS() == 0) Apoptosis();
    if (update % cfg.TOTAL_CONTRIBUTE() == 0) TotalContribute();
    if (update % cfg.PREV_CHAN() == 0) PrevChan();
    if (update % cfg.PARENT_POS() == 0) ParentPos();
    if (update % cfg.CELL_AGE() == 0) CellAge();
    if (update % cfg.SPIKE_BROADCAST_TRAFFIC() == 0) SpikeBroadcastTraffic();
    if (update % cfg.DEATH() == 0) Death();
    if (update % cfg.OUTGOING_CONNECTION_COUNT() == 0) OutgoingConnectionCount();
    if (update % cfg.FLEDGING_CONNECTION_COUNT() == 0) FledglingConnectionCount();
    if (update % cfg.INCOMING_CONNECTION_COUNT() == 0) IncomingConnectionCount();

    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      if (update % cfg.INBOX_ACTIVATION() == 0) InboxActivation(dir);
      if (update % cfg.INBOX_TRAFFIC() == 0) InboxTraffic(dir);
      if (update % cfg.TRUSTED_INBOX_TRAFFIC() == 0) TrustedInboxTraffic(dir);
      if (update % cfg.REP_OUTGOING() == 0) RepOutgoing(dir);
      if (update % cfg.REP_INCOMING() == 0) RepIncoming(dir);
      if (update % cfg.RESOURCE_CONTRIBUTED() == 0) ResourceContributed(dir);
      if (update % cfg.IN_RESISTANCE() == 0) InResistance(dir);
      if (update % cfg.OUT_RESISTANCE() == 0) OutResistance(dir);
      if (update % cfg.HEIR() == 0) Heir(dir);
    }

    file.flush(H5F_SCOPE_LOCAL);
  }

private:
  void InitAttributes() {
    util.WriteAttribute(
      "SEED",
      cfg.SEED()
    );

    util.WriteAttribute(
      "NLEV",
      cfg.NLEV()
    );

    util.WriteAttribute(
      "START_TIMESTAMP",
      static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count())
    );

    util.WriteAttribute(
      "SOURCE_HASH",
      std::string{STRINGIFY(DISHTINY_HASH_)}
    );

    util.WriteAttribute(
      "EMP_HASH",
      std::string{STRINGIFY(EMPIRICAL_HASH_)}
    );

    std::ostringstream config_stream;
    cfg.WriteMe(config_stream);
    util.WriteAttribute(
      "config",
      config_stream.str()
    );

    util.WriteAttribute(
      "KEY",
      std::string{"0: unexpired, 1: within grace period, 2: expired"},
      "/Expiration"
    );

    util.WriteAttribute(
      "KEY",
      std::string{"0: dead, 1: live"},
      "/Live"
    );

    util.WriteAttribute(
      "KEY",
      std::string{"0: none, 1: partial, 2: complete"},
      "/Apoptosis"
    );

    util.WriteAttribute(
      "KEY",
      std::string{"-1: no current parent, >=0: parent index"},
      "/ParentPos"
    );

    util.WriteAttribute(
      "KEY",
      std::string{"0: none, 1: apoptosis, 2: bankrupt, 3: trampled"},
      "/Death"
    );

    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      util.WriteAttribute(
        "KEY",
        std::string{"0: off, 1: on"},
        "/InboxActivation/dir_" + emp::to_string(dir)
      );
      util.WriteAttribute(
        "KEY",
        std::string{"0: false, 1: true"},
        "/InResistance/dir_" + emp::to_string(dir)
      );
      util.WriteAttribute(
        "KEY",
        std::string{"0: false, 1: true"},
        "/OutResistance/dir_" + emp::to_string(dir)
      );
      util.WriteAttribute(
        "KEY",
        std::string{"0: false, 1: true"},
        "/Heir/dir_" + emp::to_string(dir)
      );
    }
  }

  void InitReference() {
    util.WriteToDataset<uint32_t>(
     "/Index/own",
      [](const size_t i){
        return i;
      }
    );

    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      util.WriteToDataset<uint32_t>(
        "/Index/dir_" + emp::to_string(dir),
        [this, &dir](const size_t i){
          return GeometryHelper(cfg).CalcLocalNeighs(i)[dir];
        }
      );
    }
  }
  void RootID() {
    util.WriteToDataset<uint32_t>(
      "/RootID/upd_",
      [this](const size_t i){
        if(dw.IsOccupied(i)) {
          return dw.GetOrg(i).GetRootID();
        } else {
          return 0UL;
        }
      }
    );
  }

  // TODO: use serialize() to store serialized data
  // TODO: write utility to de-serialize data, feed it to a SignalGP program
  //       and finally call PrintProgramFull().
  // TODO: note this as a string attribute on H5 file
  void Population() {
    // goal: reduce redundant data by giving each observed value a UID
    // then storing UIDs positionally & providing a UID-to-value map
    util.WriteDecoder(
      [this](const size_t i) {
        std::ostringstream buffer;
        if(dw.IsOccupied(i)) {
          Genome & g = dw.GetOrg(i);
          g.GetProgram().PrintProgramFull(buffer);
        }
        return buffer.str();
      },
      "Population"
    );
  }

  void Triggers() {
    // goal: reduce redundant data by giving each observed value a UID
    // then storing UIDs positionally & providing a UID-to-value map
    util.WriteDecoder(
      [this](const size_t i) {
        const auto& tag_vector = dw.GetOrg(i).GetTags();

        return std::multiset<Config::tag_t>(
          tag_vector.begin(),
          tag_vector.end()
        );
      },
      memtype::MakeArrayType(),
      "Triggers"
    );
  }

  void Regulators() {
    // goal: reduce redundant data by giving each observed value a UID
    // then storing UIDs positionally & providing a UID-to-value map
    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      util.WriteDecoder(
        [this, dir](const size_t i) {
          // GetState() returns a MatchBinState
          const auto& state = dw.frames[i]->GetFrameHardware(
            dir
          ).GetHardware().GetMatchBin().GetState();

          std::vector<bundles::RegulatorData> data;
          // todo: use tid-style lambas to figure out internal H5 type for T
          for (const auto& [uid, entry] : state.data) {
            data.emplace_back(
              entry.val,
              entry.regulator.View(),
              entry.tag
            );
          }

          return data;
        },
        memtype::MakeRegulatorData(),
        emp::to_string("/Regulators/dir_", dir),
        "Regulators"
      );
    }
  }


  void Functions() {
    // goal: reduce redundant data by giving each observed value a UID
    // then storing UIDs positionally & providing a UID-to-value map
    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      util.WriteDecoder(
        [this, dir](  const size_t i) {
          const auto & hw = dw.frames[i]->GetFrameHardware(
            dir
          ).GetHardware();

          std::multiset<Config::tag_t> res;
          for (const auto & stack : hw.GetCores()) {
            if (stack.size()) res.insert(
              hw.GetMatchBin().GetState().data.find(
                stack.back().GetFP()
              )->second.tag
            );
          }

          return res;
        },
        memtype::MakeArrayType(),
        emp::to_string("/Functions/dir_", dir),
        "Functions"
      );
    }
  }
  void Channel(const size_t lev) {
    util.WriteToDataset<uint64_t>(
      emp::to_string("/Channel/lev_", lev, "/upd_"),
      [this, &lev](const size_t i){
        const auto res = dw.man->Channel(i).GetID(lev);
        return res ? *res : 0;
      }
    );
  }

  void Expiration(const size_t lev) {
    util.WriteToDataset<char>(
      emp::to_string("/Expiration/lev_", lev, "/upd_"),
      [this, &lev](const size_t i){
        const auto res = dw.man->Channel(i).IsExpired(lev);
        if (!res) {
          return 0;
        } else if (res <= cfg.EXP_GRACE_PERIOD()) {
          return 1;
        } else {
          return 2;
        }
      }
    );
  }

  void ChannelGeneration(const size_t lev) {
    util.WriteToDataset<uint32_t>(
      emp::to_string("/ChannelGeneration/lev_", lev, "/upd_"),
      [this, &lev](const size_t i){
        return dw.man->Channel(i).GetGeneration(lev);
      }
    );
  }

  void Stockpile() {
    util.WriteToDataset<double>(
      "/Stockpile/upd_",
      [this](const size_t i){
        return dw.man->Stockpile(i).QueryResource();
      }
    );
  }

  void Live() {
    util.WriteToDataset<char>(
      "/Live/upd_",
      [this](const size_t i){
        return dw.IsOccupied(i);
      }
    );
  }

  void Apoptosis() {
    util.WriteToDataset<char>(
      "/Apoptosis/upd_",
      [this](const size_t i){
        return dw.man->Apoptosis(i).GetState();
      }
    );
  }

  void InboxActivation(const size_t dir) {
    util.WriteToDataset<char>(
      emp::to_string("/InboxActivation/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.frames[i]->GetFrameHardware(dir).CheckInboxActivity();
      }
    );
  }

  void SpikeBroadcastTraffic() {
    util.WriteToDataset<uint32_t>(
      "/SpikeBroadcastTraffic/upd_",
      [this](const size_t i){
        return dw.man->Inbox(i).GetSpikeBroadcastTraffic();
      }
    );
  }

  void InboxTraffic(const size_t dir) {
    util.WriteToDataset<uint32_t>(
      emp::to_string("/InboxTraffic/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.man->Inbox(i).GetTraffic(dir);
      }
    );
  }

  void TrustedInboxTraffic(const size_t dir) {
    util.WriteToDataset<uint32_t>(
      emp::to_string("/TrustedInboxTraffic/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.man->Inbox(i).GetTrustedTraffic(dir);
      }
    );
  }

  void RepOutgoing(const size_t dir) {
    GeometryHelper gh(cfg);

    util.WriteToDataset<int>(
      emp::to_string("/RepOutgoing/dir_", dir, "/upd_"),
      [this, &gh, &dir](const size_t i){
        return dw.man->Priority(
            gh.CalcLocalNeighs(i)[dir]
          ).ViewRepState(Cardi::Opp[dir]);
      }
    );
  }

  void RepIncoming(const size_t dir) {
    util.WriteToDataset<int>(
      emp::to_string("/RepIncoming/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.man->Priority(i).ViewRepStateDup(dir);
      }
    );
  }

 void TotalContribute() {
    util.WriteToDataset<double>(
      "/TotalContribute/upd_",
      [this](const size_t i){
        return dw.man->Stockpile(i).QueryTotalContribute();
      }
    );
 }

  void ResourceContributed(const size_t dir) {
    util.WriteToDataset<double>(
      emp::to_string("/ResourceContributed/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.man->Stockpile(i).QueryExternalContribute(dir);
      }
    );
  }

  void ResourceHarvested(const size_t lev) {
    util.WriteToDataset<double>(
      emp::to_string("/ResourceHarvested/lev_", lev, "/upd_"),
      [this, &lev](const size_t i){
        double ret = 0.0;
        for (size_t r = 0; r < cfg.WAVE_REPLICATES(); ++r) {
          ret += dw.man->Wave(r, i,lev).GetLastHarvest();
        }
        return ret;
      }
    );
  }

  void PrevChan() {
    util.WriteToDataset<Config::chanid_t>(
      "/PrevChan/upd_",
      [this](const size_t i){
        return dw.man->Family(i).GetPrevChan();
      }
    );
  }

  void InResistance(const size_t dir) {
    util.WriteToDataset<double>(
      emp::to_string("/InResistance/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.man->Sharing(i).CheckInResistance(dir);
      }
    );
  }

  void OutResistance(const size_t dir) {
    util.WriteToDataset<double>(
      emp::to_string("/OutResistance/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.man->Sharing(i).CheckOutResistance(dir);
      }
    );
  }

  void Heir(const size_t dir) {
    util.WriteToDataset<char>(
      emp::to_string("/Heir/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.man->Heir(i).IsHeir(dir);
      }
    );
  }

  void ParentPos() {
    util.WriteToDataset<uint32_t>(
      "/ParentPos/upd_",
      [this](const size_t i){
        return dw.man->Family(dw.man->Family(i).GetParentPos()).HasChildPos(i)
          ? dw.man->Family(i).GetParentPos() : -1;
      }
    );
  }

  void CellAge() {
    util.WriteToDataset<uint32_t>(
      "/CellAge/upd_",
      [this](const size_t i){
        return dw.GetUpdate() - dw.man->Family(i).GetBirthUpdate();
      }
    );
  }

  void CellGen(const size_t lev) {
    util.WriteToDataset<uint32_t>(
      emp::to_string("/CellGen/lev_", lev, "/upd_"),
      [this, &lev](const size_t i){
        return dw.man->Family(i).GetCellGen()[lev];
      }
    );
  }

  void Death() {
    util.WriteToDataset<char>(
      "/Death/upd_",
      [this](const size_t i){
        // trampled = 3;
        // bankrupt = 2;
        // apoptosis = 1;
        // none = 0;
        if (0 == (dw.GetUpdate() + 1) % cfg.ENV_TRIG_FREQ()) {
          if (dw.man->Apoptosis(i).IsMarked()) { return 1; }
          else if (dw.man->Stockpile(i).IsBankrupt()) { return 2; }
          else if (dw.man->Priority(i).QueryPendingGenome()) { return 3; }
        }
        return 0;
      }
    );
  }

  void OutgoingConnectionCount() {
    util.WriteToDataset<uint32_t>(
      "/OutgoingConnectionCount/upd_",
      [this](const size_t i){
        return dw.man->Connection(i).ViewDeveloped().size();
      }
    );
  }

  void FledglingConnectionCount() {
    util.WriteToDataset<uint32_t>(
      "/FledglingConnectionCount/upd_",
      [this](const size_t i){
        return dw.man->Connection(i).ViewFledgling().size();
      }
    );
  }

  void IncomingConnectionCount() {
    util.WriteToDataset<uint32_t>(
      "/IncomingConnectionCount/upd_",
      [this](const size_t i){
        return dw.frames[i]->GetIncomingConectionCount();
      }
    );
  }

};

} // namespace dish

#endif // #ifndef DISH_DATA_DATAHELPER_HPP_INCLUDE
