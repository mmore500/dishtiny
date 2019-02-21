#pragma once

#include <iostream>
#include <sstream>

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

    InitAttributes();
    InitReference();

    file.createGroup("/Population");
    for(size_t lev = 0; lev < cfg.NLEV(); ++lev) {
      file.createGroup("/Channel_"+emp::to_string(lev));
    }
    file.createGroup("/Stockpile");

    file.flush(H5F_SCOPE_LOCAL);

    dw.OnUpdate([this](size_t _){
      if(dw.GetUpdate() % cfg.SNAPSHOT_FREQUENCY() == 0) {
        Population();
        for(size_t lev = 0; lev < cfg.NLEV(); ++lev) Channel(lev);
        Stockpile();
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
      "SEED", H5::PredType::STD_I32BE, seed_dataspace
    );

    const int seed_data[] = {cfg.SEED()};

    seed_attribute.write(H5::PredType::NATIVE_INT, seed_data);

    const hsize_t git_version_dims[] = { 1 };
    H5::DataSpace git_version_dataspace(1, git_version_dims);

    H5::Attribute git_version_attribute = file.createAttribute(
      "GIT_VERSION", H5::StrType(0, H5T_VARIABLE), git_version_dataspace
    );

    const char *git_version_data[] = {STRINGIFY(GIT_VERSION_)};

    git_version_attribute.write(H5::StrType(0, H5T_VARIABLE), git_version_data);
  }

  void InitReference() {

    const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    const auto tid = H5::PredType::NATIVE_INT;

    H5::DataSet ds_idx = file.createDataSet(
      "/ref_idx",
      tid,
      H5::DataSpace(2,dims)
    );

    int data[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) data[i] = i;

    ds_idx.write((void*)data, tid);

    for(size_t dir = 0; dir < Cardi::Dir::NumDirs; ++dir) {

      H5::DataSet ds_dir = file.createDataSet(
        "/ref_dir_" + emp::to_string(dir),
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
      "/Population/update_"+emp::to_string(dw.GetUpdate()),
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
    static const auto tid = H5::PredType::STD_U64BE;

    H5::DataSet ds = file.createDataSet(
      "/Channel_"+emp::to_string(lev)+"/update_"+emp::to_string(dw.GetUpdate()),
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
      "/Stockpile/update_"+emp::to_string(dw.GetUpdate()),
      tid,
      H5::DataSpace(2,dims)
    );

    double data[dw.GetSize()];

    std::ostringstream buffers[dw.GetSize()];
    std::string strings[dw.GetSize()];

    for (size_t i = 0; i < dw.GetSize(); ++i) {

      data[i] = dw.man->Stockpile(i).QueryResource();

    }

    ds.write((void*)data, tid);

  }

};
