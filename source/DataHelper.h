#pragma once

#include <iostream>
#include <sstream>
#include <chrono>

#include "H5Cpp.h"

#include <cereal/archives/json.hpp>

#include "data/DataNode.h"
#include "tools/keyname_utils.h"
#include "tools/string_utils.h"
#include "tools/QueueCache.h"

#include "Config.h"
#include "DishWorld.h"
#include "Genome.h"

class DataHelper {

private:

  const Config &cfg;

  DishWorld &dw;

  H5::H5File file;

  const hsize_t grid_dims[2]{cfg.GRID_H(), cfg.GRID_W()};
  const size_t updates_per_chunk = cfg.UPDATES_PER_CHUNK();
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
    InitDecoder("/Population/decoder");
    InitDecoder("/Triggers/decoder");
    InitDecoder("/Regulators/decoder");
    InitDecoder("/Functions/decoder");

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

    for(size_t lev = 0; lev < cfg.NLEV(); ++lev) {
      Channel(lev);
      ChannelGeneration(lev);
      Expiration(lev);
      ResourceHarvested(lev);
    }

    for(size_t lev = 0; lev < cfg.NLEV() + 1; ++lev) CellGen(lev);

    RootID();
    Stockpile();
    Live();
    Apoptosis();
    TotalContribute();
    PrevChan();
    ParentPos();
    CellAge();
    SpikeBroadcastTraffic();
    Death();
    OutgoingConnectionCount();
    FledglingConnectionCount();
    IncomingConnectionCount();

    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      InboxActivation(dir);
      InboxTraffic(dir);
      TrustedInboxTraffic(dir);
      RepOutgoing(dir);
      RepIncoming(dir);
      ResourceContributed(dir);
      InResistance(dir);
      OutResistance(dir);
      Heir(dir);
    }

    file.flush(H5F_SCOPE_LOCAL);
  }

private:
  template <typename T>
  void WriteAttribute(const std::string& name, const T& data, const std::string group="/") {
    constexpr hsize_t dataspace_dims[] = { 1 };

    H5::DataSpace dspace(1, dataspace_dims);
    H5::PredType pred = hid_from_type<T>();

    H5::Attribute attr = file.openGroup(group).createAttribute(
      name, pred, dspace
    );

    attr.write(pred, &data);
  }

  void WriteAttribute(const std::string& name, const std::string& data, const std::string group="/") {
    constexpr hsize_t dataspace_dims[] = { 1 };

    H5::DataSpace dspace(1, dataspace_dims);
    H5::StrType pred(0, H5T_VARIABLE);

    H5::Attribute attr = file.openGroup(group).createAttribute(
      name, pred, dspace
    );

    attr.write(pred, &data);
  }

  void InitAttributes() {
    WriteAttribute(
      "SEED",
      cfg.SEED()
    );

    WriteAttribute(
      "NLEV",
      cfg.NLEV()
    );

    WriteAttribute(
      "START_TIMESTAMP",
      static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count())
    );

    WriteAttribute(
      "SOURCE_HASH",
      std::string{STRINGIFY(DISHTINY_HASH_)}
    );

    WriteAttribute(
      "EMP_HASH",
      std::string{STRINGIFY(EMPIRICAL_HASH_)}
    );

    std::ostringstream config_stream;
    cfg.WriteMe(config_stream);
    WriteAttribute(
      "config",
      config_stream.str()
    );

    WriteAttribute(
      "KEY",
      std::string{"0: unexpired, 1: within grace period, 2: expired"},
      "/Expiration"
    );

    WriteAttribute(
      "KEY",
      std::string{"0: dead, 1: live"},
      "/Live"
    );

    WriteAttribute(
      "KEY",
      std::string{"0: none, 1: partial, 2: complete"},
      "/Apoptosis"
    );

    WriteAttribute(
      "KEY",
      std::string{"-1: no current parent, >=0: parent index"},
      "/ParentPos"
    );

    WriteAttribute(
      "KEY",
      std::string{"0: none, 1: apoptosis, 2: bankrupt, 3: trampled"},
      "/Death"
    );

    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      WriteAttribute(
        "KEY",
        std::string{"0: off, 1: on"},
        "/InboxActivation/dir_" + emp::to_string(dir)
      );
      WriteAttribute(
        "KEY",
        std::string{"0: false, 1: true"},
        "/InResistance/dir_" + emp::to_string(dir)
      );
      WriteAttribute(
        "KEY",
        std::string{"0: false, 1: true"},
        "/OutResistance/dir_" + emp::to_string(dir)
      );
      WriteAttribute(
        "KEY",
        std::string{"0: false, 1: true"},
        "/Heir/dir_" + emp::to_string(dir)
      );
    }
  }

  void InitReference() {
    WriteTemplate<uint32_t>(
     "/Index/own",
      [](const size_t i){
        return i;
      }
    );

    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      WriteTemplate<uint32_t>(
        "/Index/dir_" + emp::to_string(dir),
        [this, &dir](const size_t i){
          return GeometryHelper(cfg).CalcLocalNeighs(i)[dir];
        }
      );
    }
  }

  /// Create a full path for a dataset
  std::string makeFullPath(const std::string& path) {
    const size_t dataset_index = dw.GetUpdate() / updates_per_chunk;
    const size_t dataset_update = dataset_index * updates_per_chunk;
    return emp::to_string(path, dataset_update);
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
  void WriteTemplate(const std::string& path, Function&& getData) {
    std::string full_path = makeFullPath(path);

    T data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {
      data[i] = getData(i);
    }

    const H5::PredType tid = hid_from_type<T>();

    const hsize_t single_update[3] = {cfg.GRID_H(), cfg.GRID_W(), 1};

    if (!file.nameExists(full_path)) {
      // set properties for dataset
      H5::DSetCreatPropList plist;
      H5Pset_obj_track_times(plist.getId(), false);
      plist.setLayout(H5D_CHUNKED);

      //hsize_t chunk_dims[2] = {updates_per_chunk * grid_dims[0], grid_dims[1]};
      hsize_t chunk_dims[3] = {cfg.GRID_H(), cfg.GRID_W(), updates_per_chunk};
      plist.setChunk(3, chunk_dims);
      plist.setDeflate(compression_level);


      // create dataspace for dataset
      const hsize_t max_dims[3]{cfg.GRID_H(), cfg.GRID_W(), H5S_UNLIMITED};
      const hsize_t start_dims[3] = {0, 0, 0};

      H5::DataSpace memspace(3, start_dims, max_dims);

      // create dataset
      H5::DataSet ds = file.createDataSet(
        full_path,
        tid,
        memspace,
        plist
      );
    }
    // get current dataset as ds
    H5::DataSet ds = file.openDataSet(full_path);

    // get dataspace from dataset
    H5::DataSpace file_space = ds.getSpace();

    // figure out what region we are writing to now.
    // this is done by getting the current number of points written and
    // dividing it by the width of a single grid.
    const hsize_t updates_written = file_space.getSimpleExtentNpoints()
          / (cfg.GRID_W() * cfg.GRID_H());

    // we then subtract the height of a single grid to write inside the extent.
    hsize_t start[3]{
      0,
      0,
      updates_written
    };

    // extend dataset by one grid height
    hsize_t extent[3] = {
      cfg.GRID_H(),
      cfg.GRID_W(),
      updates_written + 1
    };
    ds.extend(extent);

    // refresh the dataspace
    file_space = ds.getSpace();

    // create new dataspace with dimensions of grid to store our data in memory
    H5::DataSpace memspace(3, single_update);

    // the following links explain what a hyperslab is.
    // basically, it is an n-dimensional selection of a space
    // in this case, it is simply a 2D selection.
    // https://support.hdfgroup.org/HDF5/Tutor/phypecont.html
    // https://support.hdfgroup.org/HDF5/Tutor/select.html
    file_space.selectHyperslab(H5S_SELECT_SET, single_update, start);

    // finally, write data inside selected hyperslab
    ds.write((void*)data, tid, memspace, file_space);
  }

  void RootID() {
    WriteTemplate<uint32_t>(
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
  void InitDecoder(const std::string& path) {
    const hsize_t chunk_dims[1] = { 100 };
    const hsize_t start_dims[1] = { 0 };
    const hsize_t max_dims[1] = { H5S_UNLIMITED };

    H5::DSetCreatPropList plist;
    H5Pset_obj_track_times(plist.getId(), false);
    plist.setLayout(H5D_CHUNKED);

    plist.setChunk(1, chunk_dims);
    plist.setDeflate(compression_level);

    H5::DataSpace memspace(1, start_dims, max_dims);

    const H5::StrType tid(0, H5T_VARIABLE);

    H5::DataSet ds = file.createDataSet(
      path,
      tid,
      memspace,
      plist
    );
  }
  void WriteBuffer(const std::ostringstream& buffer, const std::string& path) {
    H5::DataSet ds = file.openDataSet(path);

    // get dataspace from dataset
    H5::DataSpace file_space = ds.getSpace();

    const hsize_t entries_written = file_space.getSimpleExtentNpoints();

    // we then subtract the height of a single grid to write inside the extent.
    hsize_t start[]{
      entries_written
    };

    // extend dataset by one grid height
    hsize_t extent[] = {
      entries_written + 1
    };

    ds.extend(extent);

    // refresh the dataspace
    file_space = ds.getSpace();

    const hsize_t single_update[1] = {1};

    // create new dataspace with dimensions of grid to store our data in memory
    H5::DataSpace memspace(1, single_update);

    file_space.selectHyperslab(H5S_SELECT_SET, single_update, start);


    /*
    std::vector<const char*> res;
    std::transform(
      std::begin(decoder),
      std::end(decoder),
      std::back_inserter(res),
      [](const auto & it){ return it->first.c_str(); }
    );
    */
    std::string temp_data{buffer.str()};
    const char* data[] = {temp_data.c_str()};

    const H5::StrType tid(0, H5T_VARIABLE);

    ds.write((void*)data, tid, memspace, file_space);
  }

  using uid_map_t = emp::QueueCache<std::string, size_t, 10000>;
  std::unordered_map<std::string, uid_map_t> decoders;
  std::unordered_map<std::string, size_t> counters;

  void Population() {
    // goal: reduce redundant data by giving each observed value a UID
    // then storing UIDs positionally & providing a UID-to-value map
    emp::vector<uid_map_t::const_iterator> decoder;

    emp::vector<uint32_t> decoder_ids;
    decoder_ids.reserve(dw.GetSize());

    for (size_t i = 0; i < dw.GetSize(); ++i) {
      std::ostringstream buffer;

      // put data in buffer
      if(dw.IsOccupied(i)) {
        Genome & g = dw.GetOrg(i);
        g.GetProgram().PrintProgramFull(buffer);
      }

      if (!decoders["Population"].Contains(buffer.str())) {
        const auto& node = decoders["Population"].Put(
          buffer.str(), counters["Population"]
        );
        counters["Population"]++;
        decoder.push_back(node);

        WriteBuffer(buffer, "/Population/decoder");
      }
      const auto& node = decoders["Population"].Get(buffer.str());
      decoder_ids.push_back(node);
    }

    WriteTemplate<uint32_t>(
      "/Population/upd_",
      [&decoder_ids](const size_t i) {
        return decoder_ids[i];
      }
    );
  }

  void Triggers() {
    // goal: reduce redundant data by giving each observed value a UID
    // then storing UIDs positionally & providing a UID-to-value map
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

      if (!decoders["Triggers"].Contains(string)) {
        const auto& node = decoders["Triggers"].Put(
          string, counters["Triggers"]
        );
        counters["Triggers"]++;
        decoder.push_back(node);

        WriteBuffer(buffer, "/Triggers/decoder/");
      }
      const auto& node = decoders["Triggers"].Get(buffer.str());
      decoder_ids.push_back(node);

    }

    WriteTemplate<uint32_t>(
      "/Triggers/upd_",
      [&decoder_ids](const size_t i) {
        return decoder_ids[i];
      }
    );
  }

  void Regulators() {
    // goal: reduce redundant data by giving each observed value a UID
    // then storing UIDs positionally & providing a UID-to-value map
    emp::vector<uid_map_t::const_iterator> decoder;

    emp::vector<emp::vector<uint32_t>> decoder_ids(Cardi::Dir::NumDirs);

    for (auto& x : decoder_ids) {
      x.reserve(dw.GetSize());
    }

    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      for (size_t i = 0; i < dw.GetSize(); ++i) {
        std::ostringstream buffer;

        // put data in buffer
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


        if (!decoders["Regulators"].Contains(string)) {
          const auto& node = decoders["Regulators"].Put(
            string, counters["Regulators"]
          );
          counters["Regulators"]++;
          decoder.push_back(node);

          WriteBuffer(buffer, "/Regulators/decoder");
        }

        const auto& node = decoders["Regulators"].Get(string);
        decoder_ids[dir].push_back(node);
      }
    }

    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      WriteTemplate<uint32_t>(
        emp::to_string("/Regulators/dir_", dir, "/upd_"),
        [&decoder_ids, &dir](const size_t i) {
          return decoder_ids[dir][i];
        }
      );
    }
  }

  void Functions() {
    // goal: reduce redundant data by giving each observed value a UID
    // then storing UIDs positionally & providing a UID-to-value map
    emp::vector<uid_map_t::const_iterator> decoder;

    emp::vector<emp::vector<uint32_t>> decoder_ids(Cardi::Dir::NumDirs);

    for (auto& x: decoder_ids) {
      x.reserve(dw.GetSize());
    }

    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      for (size_t i = 0; i < dw.GetSize(); ++i) {
        std::ostringstream buffer;

        // put data in buffer
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

        if (!decoders["Functions"].Contains(string)) {
          const auto& node = decoders["Functions"].Put(
            string, counters["Functions"]
          );

          counters["Functions"]++;
          decoder.push_back(node);

          WriteBuffer(buffer, "/Functions/decoder");
        }
        const auto& node = decoders["Functions"].Get(string);
        decoder_ids[dir].push_back(node);

      }
    }

    for (size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      WriteTemplate<uint32_t>(
        emp::to_string("/Functions/dir_", dir, "/upd_"),
        [&decoder_ids, &dir](const size_t i) {
          return decoder_ids[dir][i];
        }
      );
    }
  }

  void Channel(const size_t lev) {
    WriteTemplate<uint64_t>(
      emp::to_string("/Channel/lev_", lev, "/upd_"),
      [this, &lev](const size_t i){
        const auto res = dw.man->Channel(i).GetID(lev);
        return res ? *res : 0;
      }
    );
  }

  void Expiration(const size_t lev) {
    WriteTemplate<char>(
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
    WriteTemplate<uint32_t>(
      emp::to_string("/ChannelGeneration/lev_", lev, "/upd_"),
      [this, &lev](const size_t i){
        return dw.man->Channel(i).GetGeneration(lev);
      }
    );
  }

  void Stockpile() {
    WriteTemplate<double>(
      "/Stockpile/upd_",
      [this](const size_t i){
        return dw.man->Stockpile(i).QueryResource();
      }
    );
  }

  void Live() {
    WriteTemplate<char>(
      "/Live/upd_",
      [this](const size_t i){
        return dw.IsOccupied(i);
      }
    );
  }

  void Apoptosis() {
    WriteTemplate<char>(
      "/Apoptosis/upd_",
      [this](const size_t i){
        return dw.man->Apoptosis(i).GetState();
      }
    );
  }

  void InboxActivation(const size_t dir) {
    WriteTemplate<char>(
      emp::to_string("/InboxActivation/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.frames[i]->GetFrameHardware(dir).CheckInboxActivity();
      }
    );
  }

  void SpikeBroadcastTraffic() {
    WriteTemplate<uint32_t>(
      "/SpikeBroadcastTraffic/upd_",
      [this](const size_t i){
        return dw.man->Inbox(i).GetSpikeBroadcastTraffic();
      }
    );
  }

  void InboxTraffic(const size_t dir) {
    WriteTemplate<uint32_t>(
      emp::to_string("/InboxTraffic/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.man->Inbox(i).GetTraffic(dir);
      }
    );
  }

  void TrustedInboxTraffic(const size_t dir) {
    WriteTemplate<uint32_t>(
      emp::to_string("/TrustedInboxTraffic/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.man->Inbox(i).GetTrustedTraffic(dir);
      }
    );
  }

  void RepOutgoing(const size_t dir) {
    GeometryHelper gh(cfg);

    WriteTemplate<int>(
      emp::to_string("/RepOutgoing/dir_", dir, "/upd_"),
      [this, &gh, &dir](const size_t i){
        return dw.man->Priority(
            gh.CalcLocalNeighs(i)[dir]
          ).ViewRepState(Cardi::Opp[dir]);
      }
    );
  }

  void RepIncoming(const size_t dir) {
    WriteTemplate<int>(
      emp::to_string("/RepIncoming/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.man->Priority(i).ViewRepStateDup(dir);
      }
    );
  }

 void TotalContribute() {
    WriteTemplate<double>(
      "/TotalContribute/upd_",
      [this](const size_t i){
        return dw.man->Stockpile(i).QueryTotalContribute();
      }
    );
 }

  void ResourceContributed(const size_t dir) {
    WriteTemplate<double>(
      emp::to_string("/ResourceContributed/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.man->Stockpile(i).QueryExternalContribute(dir);
      }
    );
  }

  void ResourceHarvested(const size_t lev) {
    WriteTemplate<double>(
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
    WriteTemplate<Config::chanid_t>(
      "/PrevChan/upd_",
      [this](const size_t i){
        return dw.man->Family(i).GetPrevChan();
      }
    );
  }

  void InResistance(const size_t dir) {
    WriteTemplate<double>(
      emp::to_string("/InResistance/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.man->Sharing(i).CheckInResistance(dir);
      }
    );
  }

  void OutResistance(const size_t dir) {
    WriteTemplate<double>(
      emp::to_string("/OutResistance/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.man->Sharing(i).CheckOutResistance(dir);
      }
    );
  }

  void Heir(const size_t dir) {
    WriteTemplate<char>(
      emp::to_string("/Heir/dir_", dir, "/upd_"),
      [this, &dir](const size_t i){
        return dw.man->Heir(i).IsHeir(dir);
      }
    );
  }

  void ParentPos() {
    WriteTemplate<uint32_t>(
      "/ParentPos/upd_",
      [this](const size_t i){
        return dw.man->Family(dw.man->Family(i).GetParentPos()).HasChildPos(i)
          ? dw.man->Family(i).GetParentPos() : -1;
      }
    );
  }

  void CellAge() {
    WriteTemplate<uint32_t>(
      "/CellAge/upd_",
      [this](const size_t i){
        return dw.GetUpdate() - dw.man->Family(i).GetBirthUpdate();
      }
    );
  }

  void CellGen(const size_t lev) {
    WriteTemplate<uint32_t>(
      emp::to_string("/CellGen/lev_", lev, "/upd_"),
      [this, &lev](const size_t i){
        return dw.man->Family(i).GetCellGen()[lev];
      }
    );
  }

  void Death() {
    WriteTemplate<char>(
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
    WriteTemplate<uint32_t>(
      "/OutgoingConnectionCount/upd_",
      [this](const size_t i){
        return dw.man->Connection(i).ViewDeveloped().size();
      }
    );
  }

  void FledglingConnectionCount() {
    WriteTemplate<uint32_t>(
      "/FledglingConnectionCount/upd_",
      [this](const size_t i){
        return dw.man->Connection(i).ViewFledgling().size();
      }
    );
  }

  void IncomingConnectionCount() {
    WriteTemplate<uint32_t>(
      "/IncomingConnectionCount/upd_",
      [this](const size_t i){
        return dw.frames[i]->GetIncomingConectionCount();
      }
    );
  }

};
