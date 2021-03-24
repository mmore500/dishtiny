#pragma once
#ifndef DISH2_CONFIG_DUMP_CONFIG_CSV_HPP_INCLUDE
#define DISH2_CONFIG_DUMP_CONFIG_CSV_HPP_INCLUDE

#include <string>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../utility/mkdir_exists_ok.hpp"
#include "../utility/pare_keyname_filename.hpp"

#include "cfg.hpp"
#include "get_endeavor.hpp"
#include "get_repro.hpp"
#include "get_slurm_job_id.hpp"

namespace dish2 {

void dump_config_csv() {

  auto keyname_attributes = emp::keyname::unpack_t{
    {"a", "asconfigured"},
    {"proc", emp::to_string( uitsl::get_proc_id() ) },
    {"source", EMP_STRINGIFY(DISHTINY_HASH_)},
    {"ext", ".csv"}
  };

  if ( dish2::get_repro() ) keyname_attributes[ "repro" ] = *dish2::get_repro();

  dish2::mkdir_exists_ok( "./outmeta" );

  const std::string outname = dish2::pare_keyname_filename(
    emp::keyname::pack( keyname_attributes ), "outmeta/"
  );
  const std::string outpath = emp::to_string("outmeta/", outname);

  emp::DataFile df( outpath );
  if ( dish2::get_repro() ) df.AddVal( *dish2::get_repro(), "Repro" );
  if ( dish2::get_endeavor() ) df.AddVal( *dish2::get_endeavor(), "Endeavor" );
  if ( dish2::get_slurm_job_id() ) df.AddVal(
    *dish2::get_slurm_job_id(), "Slurm Job ID"
  );

  dish2::cfg.WriteCSV( df );

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_DUMP_CONFIG_CSV_HPP_INCLUDE
