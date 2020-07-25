#pragma once

#include <iostream>
#include <sstream>
#include <chrono>

#include "H5Cpp.h"

#include "cereal/archives/json.hpp"

#include "data/DataNode.h"
#include "tools/keyname_utils.h"
#include "tools/string_utils.h"
#include "tools/QueueCache.h"

#include "Config.h"
#include "DishWorld.h"
#include "Genome.h"

class H5Utils {
  private:
    const DishWorld& dw;
    const Config& cfg;
    H5::H5File& file;

    const hsize_t grid_dims[2];
    const size_t updates_per_chunk;
    const size_t compression_level;

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


  public:

    H5Utils(const DishWorld& dw_, const Config& cfg_, H5::H5File& file_)
      : dw(dw_)
      , cfg(cfg_)
      , file(file_)
      , grid_dims{cfg.GRID_H(), cfg.GRID_W()}
      , updates_per_chunk(cfg.UPDATES_PER_CHUNK())
      , compression_level(cfg.CHUNK_COMPRESSION())
    { ; }

    /// Determine HID type
    template <typename T>
    H5::PredType hid_from_type() { return hid_from_type_t(T()); }

    template <typename T, typename Function>
    void WriteToDataset(const std::string& path, Function&& getData) {
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

    template <typename T, typename Function>
    void WriteSet(const std::string& path, Function&& getData) {
      std::string full_path = makeFullPath(path);

      T data[dw.GetSize()];

      for (size_t i = 0; i < dw.GetSize(); ++i) {
        data[i] = getData(i);
      }


      const hsize_t single_update[3] = {cfg.GRID_H(), cfg.GRID_W(), 1};

      if (!file.nameExists(full_path)) {
        // set properties for dataset
        H5::DSetCreatPropList plist;
        H5Pset_obj_track_times(plist.getId(), false);

        /*
        plist.setLayout(H5D_CHUNKED);

        hsize_t chunk_dims[3] = {cfg.GRID_H(), cfg.GRID_W(), updates_per_chunk};
        plist.setChunk(3, chunk_dims);
        plist.setDeflate(compression_level);
        */

        // create dataspace for dataset
        const hsize_t max_dims[2]{H5S_UNLIMITED, 1};
        const hsize_t start_dims[2] = {0, 1};

        H5::DataSpace memspace(2, start_dims, max_dims);

        auto tid = H5::ArrayType(H5T_STD_U8BE, 1, 10000);

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

      auto tid = H5::ArrayType(H5T_NATIVE_UCHAR, 1, 10000);

      // figure out what region we are writing to now.
      // this is done by getting the current number of points written and
      // dividing it by the width of a single grid.
      const hsize_t updates_written = file_space.getSimpleExtentNpoints();

      // we then subtract the height of a single grid to write inside the extent.
      hsize_t start[2]{
        updates_written,
        0
      };

      // extend dataset by one grid height
      hsize_t extent[2] = {
        updates_written + 1,
        1
      };

      ds.extend(extent);

      // refresh the dataspace
      file_space = ds.getSpace();

      // create new dataspace with dimensions of grid to store our data in memory
      H5::DataSpace memspace(2, single_update);

      // the following links explain what a hyperslab is.
      // basically, it is an n-dimensional selection of a space
      // in this case, it is simply a 2D selection.
      // https://support.hdfgroup.org/HDF5/Tutor/phypecont.html
      // https://support.hdfgroup.org/HDF5/Tutor/select.html
      file_space.selectHyperslab(H5S_SELECT_SET, single_update, start);

      // finally, write data inside selected hyperslab
      ds.write((void*)data, tid, memspace, file_space);
    }
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

    void InitDecoder(const std::string& path) {
      const hsize_t chunk_dims[1] = { 1000 };
      const hsize_t start_dims[1] = { 0 };
      const hsize_t max_dims[1] = { H5S_UNLIMITED };

      H5::DSetCreatPropList plist;
      H5Pset_obj_track_times(plist.getId(), false);
      plist.setLayout(H5D_CHUNKED);
      plist.setChunk(1, chunk_dims);
      plist.setDeflate(compression_level);

      H5::DataSpace memspace(1, start_dims, max_dims);

      hsize_t dims[1]{Config::tag_t::GetNumBytes()};
      const H5::ArrayType arr(H5::PredType::STD_U8BE, 1, dims);
      const H5::VarLenType tid(&arr);

      H5::DataSet ds = file.createDataSet(
        path,
        tid,
        memspace,
        plist
      );
    }

    using uid_map_t = emp::QueueCache<std::multiset<Config::tag_t>, size_t, 100000, emp::ContainerHash<std::multiset<Config::tag_t>>>;
    std::unordered_map<std::string, uid_map_t> decoders;
    std::unordered_map<std::string, size_t> counters;
    void WriteMultiset(
      const std::function<std::multiset<Config::tag_t>(const size_t)>& getter,
      const std::string& data_path,
      std::string decoder_path = ""
    ) {
      auto WriteBuffer = [this](const std::multiset<Config::tag_t>& str, const std::string& path) {
        H5::DataSet ds = file.openDataSet(path);

        // get dataspace from dataset
        H5::DataSpace file_space = ds.getSpace();

        const hsize_t entries_written = file_space.getSimpleExtentNpoints();

        // we then subtract the height of a single grid to write inside the extent.
        hsize_t start[]{entries_written};

        // extend dataset by one grid height
        hsize_t extent[] = {entries_written + 1};

        ds.extend(extent);

        // refresh the dataspace
        file_space = ds.getSpace();

        const hsize_t single_update[1] = {1};

        // create new dataspace with dimensions of grid to store our data in memory
        H5::DataSpace memspace(1, single_update);

        file_space.selectHyperslab(H5S_SELECT_SET, single_update, start);

        const size_t len = Config::tag_t::GetNumBytes();
        emp::vector<std::array<unsigned char, len>> data;
        std::transform(
          str.begin(),
          str.end(),
          std::back_inserter(data),
          [](const Config::tag_t& a) {
            std::array<unsigned char, len> temp;

            std::copy(
              (char*)a.GetBytes().begin(),
              (char*)a.GetBytes().end(),
              temp.begin()
            );
            return temp;
          }
        );
        hvl_t hdf_buffer;
        hdf_buffer.p = data.data();
        hdf_buffer.len = data.size();

        hsize_t dims[1]{len};
        const H5::ArrayType arr(H5::PredType::STD_U8BE, 1, dims);
        const H5::VarLenType tid(&arr);

        ds.write(&hdf_buffer, tid, memspace, file_space);
      };

      if (!decoder_path.size()) {
        decoder_path = data_path;
      }

      emp::vector<uint32_t> decoder_ids;
      decoder_ids.reserve(dw.GetSize());

      for (size_t i = 0; i < dw.GetSize(); ++i) {
        auto ret = getter(i);

        if (!decoders[decoder_path].Contains(ret)) {
          decoders[decoder_path].Put(
            ret, counters[decoder_path]
          );
          counters[decoder_path]++;
          WriteBuffer(ret, decoder_path + "/decoder");
        }
        decoder_ids.push_back(
          decoders[decoder_path].Get(ret)
        );
      }

      WriteToDataset<uint32_t>(
        data_path + "/upd_",
        [&decoder_ids](const size_t i) {
          return decoder_ids[i];
        }
      );
    }
    /*
    void WriteDecoder(
      const std::function<std::string(const size_t)>& getter,
      const std::string& data_path,
      std::string decoder_path = ""
    ) {
      auto WriteBuffer = [this](const std::string& str, const std::string& path) {
        H5::DataSet ds = file.openDataSet(path);

        // get dataspace from dataset
        H5::DataSpace file_space = ds.getSpace();

        const hsize_t entries_written = file_space.getSimpleExtentNpoints();

        // we then subtract the height of a single grid to write inside the extent.
        hsize_t start[]{entries_written};

        // extend dataset by one grid height
        hsize_t extent[] = {entries_written + 1};

        ds.extend(extent);

        // refresh the dataspace
        file_space = ds.getSpace();

        const hsize_t single_update[1] = {1};

        // create new dataspace with dimensions of grid to store our data in memory
        H5::DataSpace memspace(1, single_update);

        file_space.selectHyperslab(H5S_SELECT_SET, single_update, start);

        const char* data[] = {str.c_str()};

        const H5::StrType tid(0, H5T_VARIABLE);

        ds.write((void*)data, tid, memspace, file_space);
      };

      if (!decoder_path.size()) {
        decoder_path = data_path;
      }

      emp::vector<uint32_t> decoder_ids;
      decoder_ids.reserve(dw.GetSize());

      for (size_t i = 0; i < dw.GetSize(); ++i) {
        std::string string = getter(i);

        if (!decoders[decoder_path].Contains(string)) {
          decoders[decoder_path].Put(
            string, counters[decoder_path]
          );
          counters[decoder_path]++;
          WriteBuffer(string, decoder_path + "/decoder");
        }
        decoder_ids.push_back(
          decoders[decoder_path].Get(string)
        );
      }

      WriteToDataset<uint32_t>(
        data_path + "/upd_",
        [&decoder_ids](const size_t i) {
          return decoder_ids[i];
        }
      );
    }
    */
};
