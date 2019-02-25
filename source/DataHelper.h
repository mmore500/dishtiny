#pragma once

#include <iostream>
#include <sstream>
#include <time.h>

#include "H5Cpp.h"

#include "data/DataNode.h"

#include "Config.h"
#include "DishWorld.h"
#include "Genome.h"

class DataHelper {

private:

  const Config &cfg;

  DishWorld &dw;

  H5::H5File file;

public:

  DataHelper(DishWorld &dw_, const Config &cfg_)
  : cfg(cfg_)
  , dw(dw_)
  , file(emp::to_string(cfg.SEED())+".h5", H5F_ACC_TRUNC)
  {

    file.createGroup("/Population");
    file.createGroup("/Index");
    file.createGroup("/Channel");
    for(size_t lev = 0; lev < cfg.NLEV(); ++lev) {
      file.createGroup("/Channel/lev_"+emp::to_string(lev));
    }
    file.createGroup("/Stockpile");
    file.createGroup("/Live");
    file.createGroup("/Apoptosis");
    file.createGroup("/InboxActivation");
    file.createGroup("/InboxTraffic");
    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      file.createGroup("/InboxActivation/dir_"+emp::to_string(dir));
      file.createGroup("/InboxTraffic/dir_"+emp::to_string(dir));
    }

    file.createGroup("/RepCount");
    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      file.createGroup("/RepCount/dir_"+emp::to_string(dir));
      for(size_t replev = 0; replev < cfg.NLEV()+1; ++replev) {
        file.createGroup(
          "/RepCount/dir_"+emp::to_string(dir)
            + "/replev_"+emp::to_string(replev)
        );
      }
    }
    file.createGroup("/ResourceContributed/");
    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
      file.createGroup("/ResourceContributed/dir_"+emp::to_string(dir));
    }
    file.createGroup("/ResourceHarvested/");
    for(size_t lev = 0; lev < cfg.NLEV(); ++lev) {
      file.createGroup("/ResourceHarvested/lev_"+emp::to_string(lev));
    }
    file.createGroup("/PrevChan/");
    file.createGroup("/AcceptSharing/");


    InitAttributes();
    InitReference();

    file.flush(H5F_SCOPE_LOCAL);

    dw.OnUpdate([this](const size_t update){
      if(cfg.TimingFun(update)) {
        Population();
        for(size_t lev = 0; lev < cfg.NLEV(); ++lev) Channel(lev);
        Stockpile();
        Live();
        Apoptosis();
        for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
          InboxActivation(dir);
          InboxTraffic(dir);
        }
        for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
          for(size_t replev = 0; replev < cfg.NLEV()+1; ++replev) {
            RepCount(dir,replev);
          }
        }
        for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {
          ResourceContributed(dir);
        }
        for(size_t lev = 0; lev < cfg.NLEV(); ++lev) ResourceHarvested(lev);
        PrevChan();
        AcceptSharing();
        file.flush(H5F_SCOPE_LOCAL);
      }
    });
  }

  ~DataHelper() { file.close(); }

private:

  void InitAttributes() {
    const hsize_t seed_dims[] = { 1 };
    H5::DataSpace seed_dataspace(1, seed_dims);

    H5::Attribute seed_attribute = file.createAttribute(
      "SEED", H5::PredType::NATIVE_INT, seed_dataspace
    );

    const int seed_data[] = {cfg.SEED()};

    seed_attribute.write(H5::PredType::NATIVE_INT, seed_data);

    const hsize_t timestamp_dims[] = { 1 };
    H5::DataSpace timestamp_dataspace(1, timestamp_dims);

    H5::Attribute timestamp_attribute = file.createAttribute(
      "START_TIMESTAMP", H5::PredType::NATIVE_INT, timestamp_dataspace
    );

    const long timestamp_data[] = {time(0)};

    timestamp_attribute.write(H5::PredType::NATIVE_LONG, timestamp_data);

    const hsize_t git_version_dims[] = { 1 };
    H5::DataSpace git_version_dataspace(1, git_version_dims);

    H5::Attribute git_version_attribute = file.createAttribute(
      "GIT_VERSION", H5::StrType(0, H5T_VARIABLE), git_version_dataspace
    );

    const char *git_version_data[] = {STRINGIFY(GIT_VERSION_)};

    git_version_attribute.write(H5::StrType(0, H5T_VARIABLE), git_version_data);

    const hsize_t live_key_dims[] = { 1 };
    H5::DataSpace live_key_dataspace(1, live_key_dims);

    H5::Attribute live_key_attribute = file.openGroup("/Live").createAttribute(
      "KEY", H5::StrType(0, H5T_VARIABLE), live_key_dataspace
    );

    const char *live_key_data[] = {"0: dead, 1: live"};

    live_key_attribute.write(H5::StrType(0, H5T_VARIABLE), live_key_data);

    const hsize_t apoptosis_key_dims[] = { 1 };
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
      const hsize_t inbox_activation_key_dims[] = { 1 };
      H5::DataSpace inbox_activation_key_dataspace(1, inbox_activation_key_dims);

      H5::Attribute inbox_activation_key_attribute = file.openGroup("/InboxActivation/dir_"+emp::to_string(dir)).createAttribute(
        "KEY", H5::StrType(0, H5T_VARIABLE), inbox_activation_key_dataspace
      );

      const char *inbox_activation_key_data[] = {"0: off, 1: on"};

      inbox_activation_key_attribute.write(H5::StrType(0, H5T_VARIABLE), inbox_activation_key_data);
    }

    const hsize_t acceptsharing_key_dims[] = { 1 };
    H5::DataSpace acceptsharing_key_dataspace(1, acceptsharing_key_dims);

    H5::Attribute acceptsharing_key_attribute = file.openGroup("/AcceptSharing").createAttribute(
      "KEY", H5::StrType(0, H5T_VARIABLE), acceptsharing_key_dataspace
    );

    const char *acceptsharing_key_data[] = {"0: false, 1: true"};

    acceptsharing_key_attribute.write(H5::StrType(0, H5T_VARIABLE), acceptsharing_key_data);

  }

  void InitReference() {

    const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    const auto tid = H5::PredType::NATIVE_INT;

    H5::DataSet ds_idx = file.createDataSet(
      "/Index/own",
      tid,
      H5::DataSpace(2,dims)
    );

    int data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) data[i] = i;

    ds_idx.write((void*)data, tid);

    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {

      H5::DataSet ds_dir = file.createDataSet(
        "/Index/dir_" + emp::to_string(dir),
        tid,
        H5::DataSpace(2,dims)
      );

      for (size_t i = 0; i < dw.GetSize(); ++i) {
        data[i] = GeometryHelper(cfg).CalcLocalNeighs(i)[dir];
      }
      ds_dir.write((void*)data, tid);

    }

  }

  void Population() {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static const H5::StrType tid(0, H5T_VARIABLE);

    H5::DataSet ds = file.createDataSet(
      "/Population/upd_"+emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(2,dims)
    );

    const char *data[dw.GetSize()];

    std::ostringstream buffers[dw.GetSize()];
    std::string strings[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {

      if(dw.IsOccupied(i)) {
        Genome & g = dw.GetOrg(i);
        g.program.PrintProgramFull(buffers[i]);
        strings[i] = buffers[i].str();
      }
      data[i] = strings[i].c_str();

    }

    ds.write((void*)data, tid);

  }

  void Channel(const size_t lev) {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static const auto tid = H5::PredType::NATIVE_UINT64;

    H5::DataSet ds = file.createDataSet(
      "/Channel/lev_"+emp::to_string(lev)+"/upd_"+emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(2,dims)
    );

    Config::chanid_t data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {

      const auto res = dw.man->Channel(i).GetID(lev);
      data[i] = res ? *res : 0;

    }

    ds.write((void*)data, tid);

  }

  void Stockpile() {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static const auto tid = H5::PredType::NATIVE_DOUBLE;

    H5::DataSet ds = file.createDataSet(
      "/Stockpile/upd_"+emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(2,dims)
    );

    double data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {

      data[i] = dw.man->Stockpile(i).QueryResource();

    }

    ds.write((void*)data, tid);

  }

  void Live() {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static const auto tid = H5::PredType::NATIVE_INT;

    H5::DataSet ds = file.createDataSet(
      "/Live/upd_"+emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(2,dims)
    );

    int data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {
      data[i] = dw.IsOccupied(i);
    }

    ds.write((void*)data, tid);

  }

  void Apoptosis() {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static const auto tid = H5::PredType::NATIVE_INT;

    H5::DataSet ds = file.createDataSet(
      "/Apoptosis/upd_"+emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(2,dims)
    );

    int data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {
      data[i] = dw.man->Apoptosis(i).GetState();
    }

    ds.write((void*)data, tid);

  }

  void InboxActivation(const size_t dir) {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static const auto tid = H5::PredType::NATIVE_INT;

    H5::DataSet ds = file.createDataSet(
      "/InboxActivation/dir_" + emp::to_string(dir)
        + "/upd_" + emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(2,dims)
    );

    int data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {
      data[i] = dw.frames[i]->GetFrameHardware(dir).CheckInboxActivity();
    }

    ds.write((void*)data, tid);

  }

  void InboxTraffic(const size_t dir) {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static const auto tid = H5::PredType::NATIVE_UINT;

    H5::DataSet ds = file.createDataSet(
      "/InboxTraffic/dir_" + emp::to_string(dir)
        + "/upd_" + emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(2,dims)
    );

    size_t data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {
      data[i] = dw.man->Inbox(i).GetTraffic(dir);
    }

    ds.write((void*)data, tid);

  }

  void RepCount(const size_t dir, const size_t replev) {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static const auto tid = H5::PredType::NATIVE_UINT;

    H5::DataSet ds = file.createDataSet(
      "/RepCount/dir_" + emp::to_string(dir)
        + "/replev_" + emp::to_string(replev)
        + "/upd_" + emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(2,dims)
    );

    size_t data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {
      data[i] = dw.man->Priority(i).GetCount(dir,replev);
    }

    ds.write((void*)data, tid);

  }

  void ResourceContributed(const size_t dir) {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static const auto tid = H5::PredType::NATIVE_DOUBLE;

    H5::DataSet ds = file.createDataSet(
      "/ResourceContributed/dir_" + emp::to_string(dir)
        + "/upd_" + emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(2,dims)
    );

    double data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {
      data[i] = dw.man->Stockpile(i).QueryExternalContribute(dir);
    }

    ds.write((void*)data, tid);

  }

  void ResourceHarvested(const size_t lev) {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static const auto tid = H5::PredType::NATIVE_DOUBLE;

    H5::DataSet ds = file.createDataSet(
      "/ResourceHarvested/lev_" + emp::to_string(lev)
        + "/upd_" + emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(2,dims)
    );

    double data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {
      data[i] = dw.man->Wave(i,lev).GetLastHarvest();
    }

    ds.write((void*)data, tid);

  }

  void PrevChan() {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static const auto tid = H5::PredType::NATIVE_UINT64;

    H5::DataSet ds = file.createDataSet(
      "/PrevChan/upd_"+emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(2,dims)
    );

    Config::chanid_t data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {

      data[i] = dw.man->Family(i).GetPrevChan();

    }

    ds.write((void*)data, tid);

  }

  void AcceptSharing() {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static const auto tid = H5::PredType::NATIVE_INT;

    H5::DataSet ds = file.createDataSet(
      "/AcceptSharing/upd_"+emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(2,dims)
    );

    int data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {
      data[i] = dw.man->Stockpile(i).CheckAcceptSharing();
    }

    ds.write((void*)data, tid);

  }

};
