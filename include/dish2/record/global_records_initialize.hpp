#pragma once
#ifndef DISH2_RECORD_GLOBAL_RECORDS_INITIALIZE_HPP_INCLUDE
#define DISH2_RECORD_GLOBAL_RECORDS_INITIALIZE_HPP_INCLUDE

#include <cstdlib>
#include <string>

#include "../../../third-party/conduit/include/uitsl/mpi/audited_routines.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/base/array.hpp"

#include "../utility/mkdir_exists_ok.hpp"

namespace dish2 {

void global_records_initialize() {

  const emp::array<std::string, 6> paths{
    "./outartifacts", "./outdata", "./outdrawings",
    "./outmeta", "./outvideo", "./outzips"
  };
  if ( uitsl::is_root() ) for ( const auto& path : paths ) {
    dish2::mkdir_exists_ok( path );
  }

  UITSL_Barrier( MPI_COMM_WORLD );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_GLOBAL_RECORDS_INITIALIZE_HPP_INCLUDE
