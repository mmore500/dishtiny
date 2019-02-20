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


    file.flush(H5F_SCOPE_LOCAL);

    dw.OnUpdate([this](size_t _){
      if(dw.GetUpdate() % cfg.SNAPSHOT_FREQUENCY() == 0) {
        Census();
        file.flush(H5F_SCOPE_LOCAL);
      }
    });
  }

  ~DataHelper() { file.close(); }

private:

  void Census() {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static H5::DataSpace sid(2,dims);
    static H5::StrType tid(0, H5T_VARIABLE);
    static H5::Group group(file.createGroup("/Census"));

    H5::DataSet ds = file.createDataSet(
      "/Census/update_"+emp::to_string(dw.GetUpdate()), tid, sid
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

  void Channel() {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static H5::DataSpace sid(2,dims);
    static H5::StrType tid(0, H5T_VARIABLE);
    static H5::Group group(file.createGroup("/Census"));

    H5::DataSet ds = file.createDataSet(
      "/Census/update_"+emp::to_string(dw.GetUpdate()), tid, sid
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

  void Stockpile() {

    static const hsize_t dims[] = {cfg.GRID_W(), cfg.GRID_H()};
    static H5::DataSpace sid(2,dims);
    static H5::StrType tid(0, H5T_VARIABLE);
    static H5::Group group(file.createGroup("/Census"));

    H5::DataSet ds = file.createDataSet(
      "/Census/update_"+emp::to_string(dw.GetUpdate()), tid, sid
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

};
