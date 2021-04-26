#pragma once

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "uitsl/debug/compare_streams.hpp"
#include "uitsl/math/math_utils.hpp"
#include "uitsl/utility/exec_utils.hpp"

const std::string base_directory = "assets/";

using dim_t = emp::vector<size_t>;

inline emp::vector<dim_t> find_assets(const std::string& name, const std::string& ext){
  emp::vector<dim_t> files;
  for (const auto& p : std::filesystem::directory_iterator(base_directory)) {
    const auto file = emp::keyname::unpack(p.path());
    if (file.count("name") && file.count("ext") &&
        file.at("name") == name && file.at("ext") == ext
       ) {
      // matching file found
      emp_assert(file.count("ndims"));
      // get ndims
      const size_t ndims = uitsl::stoszt(file.at("ndims"));

      // put all dims into dim_t
      dim_t dims;
      for (size_t i = 0; i < ndims; ++i) {
        emp_assert(file.count("dim" + emp::to_string(i)));
        const size_t dim = uitsl::stoszt(file.at("dim" + emp::to_string(i)));
        dims.push_back(dim);
      }
      // store the resulting data
      files.push_back(dims);
    }
  }
  return files;
}

inline std::string make_filename(const std::string& name, const emp::vector<size_t>& dims, const std::string& ext) {
  std::unordered_map<std::string, std::string> filename;

  filename["name"] = name;
  filename["ext"] = ext;

  // figure out ndims
  const size_t ndims = dims.size();
  filename["ndims"] = std::to_string(ndims);

  for (size_t i = 0; i < ndims; ++i) {
    filename["dim" + emp::to_string(i)] = std::to_string(dims[i]);
  }

  return base_directory + emp::keyname::pack(filename);
}

inline std::string make_filename(const std::string& name, const size_t dim, const std::string& ext) {
  return make_filename(name, emp::vector<size_t>{dim}, ext);
}

inline bool test_isomorphic(const std::string& str, const std::string& filename) {
  // write ss to file
  std::cout << str << '\n';

  std::ofstream file_out("staging/" + filename, std::ifstream::binary|std::ifstream::ate);
  file_out << str;
  file_out.close();

  std::cout << "isomorphic?" << '\n';
  // use python utlity to compare
  std::string res = uitsl::exec(emp::to_string(
    "scripts/compare_graphs.py ",
    "staging/" + filename + " ",
    filename
  ).c_str());

  std::cout << res << '\n';

  emp::remove_whitespace(res);

  return (res == "isomorphic");
}

template <typename Factory, typename T>
bool test_adjacency_output(const Factory& factory, const T dims) {
  // test adj list
  std::stringstream adj_stream;
  factory(dims).PrintAdjacencyList(adj_stream);
  std::string adj_str(adj_stream.str());

  std::string filename = make_filename(
    factory.GetSlug(),
    dims,
    ".adj"
  );

  std::cout << "testing: " << filename << '\n';

  std::ifstream adj_file(filename, std::ifstream::binary|std::ifstream::ate);

  return (
    uitsl::compare_streams(adj_stream, adj_file)
    || test_isomorphic(adj_str, filename)
  );
}

template <typename Fun, typename T>
bool test_edge_output(const Fun&& factory, const T dims) {
  // test edge list
  std::stringstream edge_stream;
  factory(dims).PrintEdgeList(edge_stream);

  std::string filename = make_filename(
    factory.GetSlug(),
    dims,
    ".edg"
  );

  std::cout << "testing: " << filename << '\n';

  std::ifstream edge_file(filename, std::ifstream::binary|std::ifstream::ate);
  return uitsl::compare_streams(edge_stream, edge_file);
}

template <typename Factory>
bool test_all_adj(const Factory&& factory) {
  std::cout << uitsl::exec("pwd") << '\n';
  const emp::vector<dim_t> assets = find_assets(factory.GetSlug(), ".adj");

  // no matching assets found
  if ( assets.empty() ) return false;

  // breaks on first false
  // akin to python's 'for ... else'
  return std::all_of(
    assets.begin(),
    assets.end(),
    [factory](const dim_t& dim) {
      return test_adjacency_output(factory, dim);
    }
  );
}

template <typename Fun>
bool test_all_edg(const Fun&& factory) {
  const auto assets = find_assets(factory.GetSlug());

  return std::all_of(
    assets.begin(),
    assets.end(),
    [&factory](const auto& dim) {
      return test_edge_output(std::move(factory), dim);
    }
  );
}
