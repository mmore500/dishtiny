#pragma once

#include <iostream>
#include <sstream>
#include <chrono>

#include "H5Cpp.h"

#include <cereal/archives/json.hpp>

#include "data/DataNode.h"
#include "tools/keyname_utils.h"
#include "tools/string_utils.h"

#include "Config.h"
#include "DishWorld.h"
#include "Genome.h"

class DataHelper {

private:

  const Config &cfg;

  DishWorld &dw;

  H5::H5File file;

//  const hsize_t chunk_dims[2]{cfg.GRIDS_PER_CHUNK() * cfg.GRID_W(), cfg.GRIDS_PER_CHUNK() * cfg.GRID_H()};
  const hsize_t chunk_dims[2]{cfg.GRID_W(), cfg.GRID_H()};
  const size_t compression_level = cfg.CHUNK_COMPRESSION();

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
  {
    enum subdir{dir, lev, lev_1, none};

    std::vector<std::tuple<
      std::string,
      subdir
      >
    > folders {
        {"/RootID", subdir::none}
      , {"/Population", subdir::none}
      , {"/Population/decoder", subdir::none}
      , {"/Triggers", subdir::none}
      , {"/Triggers/decoder", subdir::none}
      , {"/Regulators", subdir::dir}
      , {"/Regulators/decoder", subdir::none}
      , {"/Functions", subdir::dir}
      , {"/Functions/decoder", subdir::none}
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

    file.flush(H5F_SCOPE_LOCAL);

  }

  ~DataHelper() { file.close(); }

  void SnapshotPopulation() {

    // Population();
    // Triggers();

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

    if (dw.GetUpdate() % cfg.COMPUTE_FREQ() == 0) {
      Regulators();
      Functions();
    }

    for(size_t lev = 0; lev < cfg.NLEV(); ++lev) Channel(lev);
    for(size_t lev = 0; lev < cfg.NLEV(); ++lev) ChannelGeneration(lev);
    for(size_t lev = 0; lev < cfg.NLEV(); ++lev) Expiration(lev);
    RootID();
    Stockpile();
    Live();
    Apoptosis();
    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      InboxActivation(dir);
    }
    for(size_t dir = 0; dir <= Cardi::Dir::NumDirs; ++dir) {
      InboxTraffic(dir);
      TrustedInboxTraffic(dir);
    }
    SpikeBroadcastTraffic();
    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      RepOutgoing(dir);
    }
    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      RepIncoming(dir);
    }
    TotalContribute();
    for(size_t dir = 0; dir <= Cardi::Dir::NumDirs; ++dir) {
      ResourceContributed(dir);
    }
    for(size_t lev = 0; lev < cfg.NLEV(); ++lev) ResourceHarvested(lev);
    PrevChan();
    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      InResistance(dir);
    }
    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      OutResistance(dir);
    }
    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      Heir(dir);
    }
    ParentPos();
    CellAge();
    for(size_t lev = 0; lev < cfg.NLEV() + 1; ++lev) CellGen(lev);
    Death();
    OutgoingConnectionCount();
    FledglingConnectionCount();
    IncomingConnectionCount();
    file.flush(H5F_SCOPE_LOCAL);
  }

private:

  void InitAttributes() {
    constexpr hsize_t seed_dims[] = { 1 };
    H5::DataSpace seed_dataspace(1, seed_dims);

    H5::Attribute seed_attribute = file.createAttribute(
      "SEED", H5::PredType::NATIVE_INT, seed_dataspace
    );

    const int seed_data[] = {cfg.SEED()};

    seed_attribute.write(H5::PredType::NATIVE_INT, seed_data);

    constexpr hsize_t nlev_dims[] = { 1 };
    H5::DataSpace nlev_dataspace(1, nlev_dims);

    H5::Attribute nlev_attribute = file.createAttribute(
      "NLEV", H5::PredType::NATIVE_INT, nlev_dataspace
    );

    const int nlev_data[] = {(int)cfg.NLEV()};

    nlev_attribute.write(H5::PredType::NATIVE_INT, nlev_data);

    constexpr hsize_t timestamp_dims[] = { 1 };
    H5::DataSpace timestamp_dataspace(1, timestamp_dims);

    H5::Attribute timestamp_attribute = file.createAttribute(
      "START_TIMESTAMP", H5::PredType::NATIVE_INT, timestamp_dataspace
    );

    const long timestamp_data[] = { 0 };

    timestamp_attribute.write(H5::PredType::NATIVE_LONG, timestamp_data);

    constexpr hsize_t dishtiny_hash_dims[] = { 1 };
    H5::DataSpace dishtiny_hash_dataspace(1, dishtiny_hash_dims);

    H5::Attribute dishtiny_hash_attribute = file.createAttribute(
      "SOURCE_HASH", H5::StrType(0, H5T_VARIABLE), dishtiny_hash_dataspace
    );

    const char *dishtiny_hash_data[] = {STRINGIFY(DISHTINY_HASH_)};

    dishtiny_hash_attribute.write(H5::StrType(0, H5T_VARIABLE), dishtiny_hash_data);

    constexpr hsize_t empirical_hash_dims[] = { 1 };
    H5::DataSpace empirical_hash_dataspace(1, empirical_hash_dims);

    H5::Attribute empirical_hash_attribute = file.createAttribute(
      "EMP_HASH", H5::StrType(0, H5T_VARIABLE), empirical_hash_dataspace
    );

    const char *empirical_hash_data[] = {STRINGIFY(EMPIRICAL_HASH_)};

    empirical_hash_attribute.write(H5::StrType(0, H5T_VARIABLE), empirical_hash_data);

    constexpr hsize_t config_dims[] = { 1 };
    H5::DataSpace config_dataspace(1,config_dims);

    H5::Attribute config_attribute = file.createAttribute(
      "config", H5::StrType(0, H5T_VARIABLE),config_dataspace
    );

    std::ostringstream config_stream;
    cfg.WriteMe(config_stream);
    std::string config_string = config_stream.str();

    const char *config_data[] = { config_string.c_str() };

   config_attribute.write(H5::StrType(0, H5T_VARIABLE),config_data);

   constexpr hsize_t expiration_key_dims[] = { 1 };
   H5::DataSpace expiration_key_dataspace(1, expiration_key_dims);

   H5::Attribute expiration_key_attribute = file.openGroup("/Expiration").createAttribute(
     "KEY", H5::StrType(0, H5T_VARIABLE), expiration_key_dataspace
   );

   const char *expiration_key_data[] = {"0: unexpired, 1: within grace period, 2: expired"};

   expiration_key_attribute.write(H5::StrType(0, H5T_VARIABLE), expiration_key_data);

    constexpr hsize_t live_key_dims[] = { 1 };
    H5::DataSpace live_key_dataspace(1, live_key_dims);

    H5::Attribute live_key_attribute = file.openGroup("/Live").createAttribute(
      "KEY", H5::StrType(0, H5T_VARIABLE), live_key_dataspace
    );

    const char *live_key_data[] = {"0: dead, 1: live"};

    live_key_attribute.write(H5::StrType(0, H5T_VARIABLE), live_key_data);

    constexpr hsize_t apoptosis_key_dims[] = { 1 };
    H5::DataSpace apoptosis_key_dataspace(1, apoptosis_key_dims);

    H5::Attribute apoptosis_key_attribute = file.openGroup(
      "/Apoptosis"
    ).createAttribute(
      "KEY", H5::StrType(0, H5T_VARIABLE), apoptosis_key_dataspace
    );

    const char *apoptosis_key_data[] = {"0: none, 1: partial, 2: complete"};

    apoptosis_key_attribute.write(
      H5::StrType(0, H5T_VARIABLE), apoptosis_key_data
    );

    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      constexpr hsize_t inbox_activation_key_dims[] = { 1 };
      H5::DataSpace inbox_activation_key_dataspace(1, inbox_activation_key_dims);

      H5::Attribute inbox_activation_key_attribute = file.openGroup("/InboxActivation/dir_"+emp::to_string(dir)).createAttribute(
        "KEY", H5::StrType(0, H5T_VARIABLE), inbox_activation_key_dataspace
      );

      const char *inbox_activation_key_data[] = {"0: off, 1: on"};

      inbox_activation_key_attribute.write(H5::StrType(0, H5T_VARIABLE), inbox_activation_key_data);
    }

    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      constexpr hsize_t acceptsharing_key_dims[] = { 1 };
      H5::DataSpace acceptsharing_key_dataspace(1, acceptsharing_key_dims);

      H5::Attribute acceptsharing_key_attribute = file.openGroup("/InResistance/dir_"+emp::to_string(dir)).createAttribute(
        "KEY", H5::StrType(0, H5T_VARIABLE), acceptsharing_key_dataspace
      );

      const char *acceptsharing_key_data[] = {"0: false, 1: true"};

      acceptsharing_key_attribute.write(H5::StrType(0, H5T_VARIABLE), acceptsharing_key_data);
    }

    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      constexpr hsize_t acceptsharing_key_dims[] = { 1 };
      H5::DataSpace acceptsharing_key_dataspace(1, acceptsharing_key_dims);

      H5::Attribute acceptsharing_key_attribute = file.openGroup("/OutResistance/dir_"+emp::to_string(dir)).createAttribute(
        "KEY", H5::StrType(0, H5T_VARIABLE), acceptsharing_key_dataspace
      );

      const char *acceptsharing_key_data[] = {"0: false, 1: true"};

      acceptsharing_key_attribute.write(H5::StrType(0, H5T_VARIABLE), acceptsharing_key_data);
    }

    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      constexpr hsize_t heir_key_dims[] = { 1 };
      H5::DataSpace heir_key_dataspace(1, heir_key_dims);

      H5::Attribute heir_key_attribute = file.openGroup("/Heir/dir_"+emp::to_string(dir)).createAttribute(
        "KEY", H5::StrType(0, H5T_VARIABLE), heir_key_dataspace
      );

      const char *heir_key_data[] = {"0: false, 1: true"};

      heir_key_attribute.write(H5::StrType(0, H5T_VARIABLE), heir_key_data);
    }

    constexpr hsize_t parentpos_key_dims[] = { 1 };
    H5::DataSpace parentpos_key_dataspace(1, parentpos_key_dims);

    H5::Attribute parentpos_key_attribute = file.openGroup("/ParentPos").createAttribute(
      "KEY", H5::StrType(0, H5T_VARIABLE), parentpos_key_dataspace
    );

    const char *parentpos_key_data[] = {"-1: no current parent, >=0: parent index"};

    parentpos_key_attribute.write(H5::StrType(0, H5T_VARIABLE), parentpos_key_data);

    constexpr hsize_t death_key_dims[] = { 1 };
    H5::DataSpace death_key_dataspace(1, death_key_dims);

    H5::Attribute death_key_attribute = file.openGroup(
      "/Death"
    ).createAttribute(
      "KEY", H5::StrType(0, H5T_VARIABLE), death_key_dataspace
    );

    const char *death_key_data[] = {"0: none, 1: apoptosis, 2: bankrupt, 3: trampled"};

    death_key_attribute.write(
      H5::StrType(0, H5T_VARIABLE), death_key_data
    );

  }

  void InitReference() {

    const auto tid = H5::PredType::NATIVE_UINT32;

    H5::DSetCreatPropList plist;
    H5Pset_obj_track_times(plist.getId(), false);

    H5::DataSet ds_idx = file.createDataSet(
      "/Index/own",
      tid,
      H5::DataSpace(2, chunk_dims),
      plist
    );

    uint32_t data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) data[i] = i;

    ds_idx.write((void*)data, tid);

    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {

      H5::DataSet ds_dir = file.createDataSet(
        "/Index/dir_" + emp::to_string(dir),
        tid,
        H5::DataSpace(2, chunk_dims),
        plist
      );

      for (size_t i = 0; i < dw.GetSize(); ++i) {
        data[i] = GeometryHelper(cfg).CalcLocalNeighs(i)[dir];
      }
      ds_dir.write((void*)data, tid);

    }

  }

  // idea taken from https://forum.hdfgroup.org/t/templatized-instantiation-of-h5-native-xxx-types/4168/2
  H5::PredType hid_from_type_t(const char &) { return H5::PredType::NATIVE_CHAR; }
  H5::PredType hid_from_type_t(const uint32_t &) { return H5::PredType::NATIVE_UINT32; }
  H5::PredType hid_from_type_t(const uint64_t &) { return H5::PredType::NATIVE_UINT64; }
  H5::PredType hid_from_type_t(const int &) { return H5::PredType::NATIVE_INT; }
  H5::PredType hid_from_type_t(const double &) { return H5::PredType::NATIVE_DOUBLE; }

  template <typename T>
  H5::PredType hid_from_type() { return hid_from_type_t(T()); }

  template <typename T, typename Function>
  void WriteTemplate(const std::string& str, Function&& getData) {
    H5::DSetCreatPropList plist;
    H5Pset_obj_track_times(plist.getId(), false);
    plist.setChunk(2, chunk_dims);
    plist.setDeflate(compression_level);

    const H5::PredType tid = hid_from_type<T>();

    H5::DataSet ds = file.createDataSet(
      str,
      tid,
      H5::DataSpace(2, chunk_dims),
      plist
    );

    T data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {
      data[i] = getData(i);
    }

    ds.write((void*)data, tid);
 }

  void RootID() {
    WriteTemplate<uint32_t>(
      "/RootID/upd_" + emp::to_string(dw.GetUpdate()),
      [this](const size_t i){
        if(dw.IsOccupied(i)) {
          return dw.GetOrg(i).GetRootID();
        } else {
          return 0UL;
        }
      }
    );
  }

  void Population() {

    // goal: reduce redundant data by giving each observed value a UID
    // then storing UIDs positionally & providing a UID-to-value map
    using uid_map_t = std::unordered_map<std::string, size_t>;
    uid_map_t uids;
    emp::vector<uid_map_t::const_iterator> decoder;

    emp::vector<uint32_t> decoder_ids;
    decoder_ids.reserve(dw.GetSize());

    for (size_t i = 0; i < dw.GetSize(); ++i) {

      std::ostringstream buffer;

      if(dw.IsOccupied(i)) {
        Genome & g = dw.GetOrg(i);
        g.GetProgram().PrintProgramFull(buffer);
      }

      const auto & [node, did_insert] = uids.insert(
        {buffer.str(), uids.size()}
      );
      if (did_insert) {
        decoder.push_back(node);
      }
      decoder_ids.push_back(node->second);

    }

    WriteTemplate<uint32_t>(
      "/Population/upd_"+emp::to_string(dw.GetUpdate()),
      [&decoder_ids](const size_t i) {
        return decoder_ids[i];
      }
    );


    {
    const hsize_t dims[] = { decoder.size() };

    H5::DSetCreatPropList plist;
    H5Pset_obj_track_times(plist.getId(), false);

    plist.setChunk(1, dims);
    plist.setDeflate(compression_level);

    const H5::StrType tid(0, H5T_VARIABLE);
    H5::DataSet ds = file.createDataSet(
      "/Population/decoder/upd_"+emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(1,dims),
      plist
    );

    std::vector<const char*> res;
    std::transform(
      std::begin(decoder),
      std::end(decoder),
      std::back_inserter(res),
      [](const auto & it){ return it->first.c_str(); }
    );
    ds.write((void*)res.data(), tid);
    }

  }

  void Triggers() {

    // goal: reduce redundant data by giving each observed value a UID
    // then storing UIDs positionally & providing a UID-to-value map
    using uid_map_t = std::unordered_map<std::string, size_t>;
    uid_map_t uids;
    emp::vector<uid_map_t::const_iterator> decoder;

    emp::vector<uint32_t> decoder_ids;
    decoder_ids.reserve(dw.GetSize());

    for (size_t i = 0; i < dw.GetSize(); ++i) {

      std::ostringstream buffer;

      if(dw.IsOccupied(i)) {
        cereal::JSONOutputArchive oarchive(
          buffer,
          cereal::JSONOutputArchive::Options::NoIndent()
        );
        oarchive(dw.GetOrg(i).GetTags());
      }

      std::string string = buffer.str();
      emp::remove_whitespace(string);

      const auto & [node, did_insert] = uids.insert({string, uids.size()});
      if (did_insert) {
        decoder.push_back(node);
      }
      decoder_ids[i] = node->second;

    }

    WriteTemplate<uint32_t>(
      "/Triggers/upd_"+emp::to_string(dw.GetUpdate()),
      [&decoder_ids](const size_t i) {
        return decoder_ids[i];
      }
    );

    {
    const hsize_t dims[] = { decoder.size() };

    H5::DSetCreatPropList plist;
    H5Pset_obj_track_times(plist.getId(), false);

    plist.setChunk(1, dims);
    plist.setDeflate(compression_level);

    const H5::StrType tid(0, H5T_VARIABLE);
    H5::DataSet ds = file.createDataSet(
      "/Triggers/decoder/upd_"+emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(1,dims),
      plist
    );

    std::vector<const char*> res;
    std::transform(
      std::begin(decoder),
      std::end(decoder),
      std::back_inserter(res),
      [](const auto & it){ return it->first.c_str(); }
    );
    ds.write((void*)res.data(), tid);
    }

  }

  void Regulators() {

    // goal: reduce redundant data by giving each observed value a UID
    // then storing UIDs positionally & providing a UID-to-value map
    using uid_map_t = std::unordered_map<std::string, size_t>;
    uid_map_t uids;
    emp::vector<uid_map_t::const_iterator> decoder;

    emp::vector<emp::vector<uint32_t>> decoder_ids(Cardi::Dir::NumDirs);

    for (auto& x : decoder_ids) {
      x.reserve(dw.GetSize());
    }

    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      for (size_t i = 0; i < dw.GetSize(); ++i) {

        std::ostringstream buffer;

        if(dw.IsOccupied(i)) {
          cereal::JSONOutputArchive oarchive(
            buffer,
            cereal::JSONOutputArchive::Options::NoIndent()
          );
          oarchive(
            dw.frames[i]->GetFrameHardware(
              dir
            ).GetHardware().GetMatchBin().GetState()
          );
        }

        std::string string = buffer.str();
        emp::remove_whitespace(string);

        const auto & [node, did_insert] = uids.insert({string, uids.size()});
        if (did_insert) {
          decoder.push_back(node);
        }
        decoder_ids[dir].push_back(node->second);

      }
    }

    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      WriteTemplate<uint32_t>(
        "/Regulators/dir_" + emp::to_string(dir)
          + "/upd_"+emp::to_string(dw.GetUpdate()),
        [&decoder_ids, &dir](const size_t i) {
          return decoder_ids[dir][i];
        }
      );
    }


    {
    const hsize_t dims[] = { decoder.size() };

    H5::DSetCreatPropList plist;
    H5Pset_obj_track_times(plist.getId(), false);

    plist.setChunk(1, dims);
    plist.setDeflate(compression_level);

    const H5::StrType tid(0, H5T_VARIABLE);
    H5::DataSet ds = file.createDataSet(
      "/Regulators/decoder/upd_"+emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(1,dims),
      plist
    );

    std::vector<const char*> res;
    std::transform(
      std::begin(decoder),
      std::end(decoder),
      std::back_inserter(res),
      [](const auto & it){ return it->first.c_str(); }
    );
    ds.write((void*)res.data(), tid);
    }

  }

  void Functions() {

    // goal: reduce redundant data by giving each observed value a UID
    // then storing UIDs positionally & providing a UID-to-value map
    using uid_map_t = std::unordered_map<std::string, size_t>;
    uid_map_t uids;
    emp::vector<uid_map_t::const_iterator> decoder;

    emp::vector<emp::vector<uint32_t>> decoder_ids(Cardi::Dir::NumDirs);

    for (auto& x: decoder_ids) {
      x.reserve(dw.GetSize());
    }

    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      for (size_t i = 0; i < dw.GetSize(); ++i) {

        std::ostringstream buffer;

        if(dw.IsOccupied(i)) {
          cereal::JSONOutputArchive oarchive(
            buffer,
            cereal::JSONOutputArchive::Options::NoIndent()
          );
          const auto & hw = dw.frames[i]->GetFrameHardware(
            dir
          ).GetHardware();
          emp::vector<Config::tag_t> res;
          for (const auto & stack : hw.GetCores()) {
            if (stack.size()) res.push_back(
              hw.GetMatchBin().GetState().tags.find(
                stack.back().GetFP()
              )->second
            );
            std::sort(std::begin(res), std::end(res));
          }
          oarchive(res);
        }

        std::string string = buffer.str();
        emp::remove_whitespace(string);

        const auto & [node, did_insert] = uids.insert({string, uids.size()});
        if (did_insert) {
          decoder.push_back(node);
        }
        decoder_ids[dir].push_back(node->second);

      }
    }

    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      WriteTemplate<uint32_t>(
        "/Functions/dir_" + emp::to_string(dir)
          + "/upd_"+emp::to_string(dw.GetUpdate()),
        [&decoder_ids, &dir](const size_t i) {
          return decoder_ids[dir][i];
        }
      );
    }


    {
    const hsize_t dims[] = { decoder.size() };

    H5::DSetCreatPropList plist;
    H5Pset_obj_track_times(plist.getId(), false);

    plist.setChunk(1, dims);
    plist.setDeflate(compression_level);

    const H5::StrType tid(0, H5T_VARIABLE);
    H5::DataSet ds = file.createDataSet(
      "/Functions/decoder/upd_"+emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(1,dims),
      plist
    );

    std::vector<const char*> res;
    std::transform(
      std::begin(decoder),
      std::end(decoder),
      std::back_inserter(res),
      [](const auto & it){ return it->first.c_str(); }
    );
    ds.write((void*)res.data(), tid);
    }

  }

  void Channel(const size_t lev) {
    WriteTemplate<uint64_t>(
      "/Channel/lev_"+emp::to_string(lev)+"/upd_"+emp::to_string(dw.GetUpdate()),
      [this, &lev](const size_t i){
        const auto res = dw.man->Channel(i).GetID(lev);
        return res ? *res : 0;
      }
    );
  }

  void Expiration(const size_t lev) {
    WriteTemplate<char>(
      "/Expiration/lev_"+emp::to_string(lev)+"/upd_"+emp::to_string(dw.GetUpdate()),
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
    WriteTemplate<uint32_t>(
      "/ChannelGeneration/lev_"+emp::to_string(lev)+"/upd_"+emp::to_string(dw.GetUpdate()),
      [this, &lev](const size_t i){
        return dw.man->Channel(i).GetGeneration(lev);
      }
    );
  }

  void Stockpile() {
    WriteTemplate<double>(
      "/Stockpile/upd_"+emp::to_string(dw.GetUpdate()),
      [this](const size_t i){
        return dw.man->Stockpile(i).QueryResource();
      }
    );
  }

  void Live() {
    WriteTemplate<char>(
      "/Live/upd_"+emp::to_string(dw.GetUpdate()),
      [this](const size_t i){
        return dw.IsOccupied(i);
      }
    );
  }

  void Apoptosis() {
    WriteTemplate<char>(
      "/Apoptosis/upd_"+emp::to_string(dw.GetUpdate()),
      [this](const size_t i){
        return dw.man->Apoptosis(i).GetState();
      }
    );
  }

  void InboxActivation(const size_t dir) {
    WriteTemplate<char>(
      "/InboxActivation/dir_" + emp::to_string(dir)
        + "/upd_" + emp::to_string(dw.GetUpdate()),
      [this, &dir](const size_t i){
        return dw.frames[i]->GetFrameHardware(dir).CheckInboxActivity();
      }
    );
  }

  void SpikeBroadcastTraffic() {
    WriteTemplate<uint32_t>(
      "/SpikeBroadcastTraffic/upd_" + emp::to_string(dw.GetUpdate()),
      [this](const size_t i){
        return dw.man->Inbox(i).GetSpikeBroadcastTraffic();
      }
    );
  }

  void InboxTraffic(const size_t dir) {
    WriteTemplate<uint32_t>(
      "/InboxTraffic/dir_" + emp::to_string(dir)
        + "/upd_" + emp::to_string(dw.GetUpdate()),
      [this, &dir](const size_t i){
        return dw.man->Inbox(i).GetTraffic(dir);
      }
    );
  }

  void TrustedInboxTraffic(const size_t dir) {
    WriteTemplate<uint32_t>(
      "/TrustedInboxTraffic/dir_" + emp::to_string(dir)
        + "/upd_" + emp::to_string(dw.GetUpdate()),
      [this, &dir](const size_t i){
        return dw.man->Inbox(i).GetTrustedTraffic(dir);
      }
    );
  }

  void RepOutgoing(const size_t dir) {
    GeometryHelper gh(cfg);

    WriteTemplate<int>(
      "/RepOutgoing/dir_" + emp::to_string(dir)
        + "/upd_" + emp::to_string(dw.GetUpdate()),
      [this, &gh, &dir](const size_t i){
        return dw.man->Priority(
            gh.CalcLocalNeighs(i)[dir]
          ).ViewRepState(Cardi::Opp[dir]);
      }
    );
  }

  void RepIncoming(const size_t dir) {
    WriteTemplate<int>(
      "/RepIncoming/dir_" + emp::to_string(dir)
        + "/upd_" + emp::to_string(dw.GetUpdate()),
      [this, &dir](const size_t i){
        return dw.man->Priority(i).ViewRepStateDup(dir);
      }
    );
  }

 void TotalContribute() {
    WriteTemplate<double>(
      "/TotalContribute/upd_" + emp::to_string(dw.GetUpdate()),
      [this](const size_t i){
        return dw.man->Stockpile(i).QueryTotalContribute();
      }
    );
 }

  void ResourceContributed(const size_t dir) {
    WriteTemplate<double>(
      "/ResourceContributed/dir_" + emp::to_string(dir)
        + "/upd_" + emp::to_string(dw.GetUpdate()),
      [this, &dir](const size_t i){
        return dw.man->Stockpile(i).QueryExternalContribute(dir);
      }
    );
  }

  void ResourceHarvested(const size_t lev) {
    WriteTemplate<double>(
      "/ResourceHarvested/lev_" + emp::to_string(lev)
        + "/upd_" + emp::to_string(dw.GetUpdate()),
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
    WriteTemplate<Config::chanid_t>(
      "/PrevChan/upd_"+emp::to_string(dw.GetUpdate()),
      [this](const size_t i){
        return dw.man->Family(i).GetPrevChan();
      }
    );
  }

  void InResistance(const size_t dir) {
    WriteTemplate<double>(
      "/InResistance/dir_" + emp::to_string(dir)
      + "/upd_"+emp::to_string(dw.GetUpdate()),
      [this, &dir](const size_t i){
        return dw.man->Sharing(i).CheckInResistance(dir);
      }
    );
  }

  void OutResistance(const size_t dir) {
    WriteTemplate<double>(
      "/OutResistance/dir_" + emp::to_string(dir)
      + "/upd_"+emp::to_string(dw.GetUpdate()),
      [this, &dir](const size_t i){
        return dw.man->Sharing(i).CheckOutResistance(dir);
      }
    );
  }

  void Heir(const size_t dir) {
    WriteTemplate<char>(
      "/Heir/dir_" + emp::to_string(dir)
        + "/upd_" + emp::to_string(dw.GetUpdate()),
      [this, &dir](const size_t i){
        return dw.man->Heir(i).IsHeir(dir);
      }
    );
  }

  void ParentPos() {
    WriteTemplate<uint32_t>(
      "/ParentPos/upd_"+emp::to_string(dw.GetUpdate()),
      [this](const size_t i){
        return dw.man->Family(dw.man->Family(i).GetParentPos()).HasChildPos(i)
          ? dw.man->Family(i).GetParentPos() : -1;
      }
    );
  }

  void CellAge() {
    WriteTemplate<uint32_t>(
      "/CellAge/upd_"+emp::to_string(dw.GetUpdate()),
      [this](const size_t i){
        return dw.GetUpdate() - dw.man->Family(i).GetBirthUpdate();
      }
    );
  }

  void CellGen(const size_t lev) {
    WriteTemplate<uint32_t>(
      "/CellGen/lev_" + emp::to_string(lev)
        + "/upd_"+emp::to_string(dw.GetUpdate()),
      [this, &lev](const size_t i){
        return dw.man->Family(i).GetCellGen()[lev];
      }
    );
  }

  void Death() {
    WriteTemplate<char>(
      "/Death/upd_"+emp::to_string(dw.GetUpdate()),
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
    WriteTemplate<uint32_t>(
      "/OutgoingConnectionCount/upd_"+emp::to_string(dw.GetUpdate()),
      [this](const size_t i){
        return dw.man->Connection(i).ViewDeveloped().size();
      }
    );
  }

  void FledglingConnectionCount() {
    WriteTemplate<uint32_t>(
      "/FledglingConnectionCount/upd_"+emp::to_string(dw.GetUpdate()),
      [this](const size_t i){
        return dw.man->Connection(i).ViewFledgling().size();
      }
    );
  }

  void IncomingConnectionCount() {
    WriteTemplate<uint32_t>(
      "/IncomingConnectionCount/upd_"+emp::to_string(dw.GetUpdate()),
      [this](const size_t i){
        return dw.frames[i]->GetIncomingConectionCount();
      }
    );
  }

};
