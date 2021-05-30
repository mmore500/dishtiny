#pragma once
#ifndef DISH2_RECORD_STINTROOTIDCHANGEOVERRECORDER_HPP_INCLUDE
#define DISH2_RECORD_STINTROOTIDCHANGEOVERRECORDER_HPP_INCLUDE

#include <string>

#include "../../../third-party/bxzstr/include/bxzstr.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"
#include "../config/thread_idx.hpp"
#include "../debug/log_msg.hpp"
#include "../utility/pare_keyname_filename.hpp"

#include "make_filename/make_data_path.hpp"
#include "make_filename/make_stint_root_id_changeover_filename.hpp"


namespace dish2 {

class StintRootIDChangeoverRecorder {

  std::string outpath;
  bxz::ofstream ofstream;
  emp::DataFile file;

  // thread_local so that copying/moving doesn't break DataFile refs
  inline static thread_local uint64_t root_id_;
  inline static thread_local uint64_t incoming_stint_root_id_;
  inline static thread_local uint64_t outgoing_stint_root_id_;

public:

  StintRootIDChangeoverRecorder()
  : outpath(
    dish2::make_data_path(
      dish2::pare_keyname_filename(
        dish2::make_stint_root_id_changeover_filename(),
        dish2::make_data_path()
      )
    )
  )
  , ofstream(
    outpath, bxz::lzma, 6
  )
  , file( ofstream ) {

    if ( dish2::has_stint() ) file.AddVal(cfg.STINT(), "Stint");
    if ( dish2::has_series() ) file.AddVal(cfg.SERIES(), "Series");
    if ( dish2::has_replicate() ) file.AddVal(cfg.REPLICATE(), "Replicate");
    file.AddVal(cfg.TREATMENT(), "Treatment");
    if ( cfg.TREATMENT().find('=') != std::string::npos ) {
      for ( const auto& [k, v] : emp::keyname::unpack( cfg.TREATMENT() ) ) {
        file.AddVal( v, emp::to_string("Treatment ", k) );
      }
    }
    file.AddVal( uitsl::get_proc_id(), "proc" );
    file.AddVal( dish2::thread_idx, "thread" );

    file.AddVar(root_id_, "Root ID");
    file.AddVar(incoming_stint_root_id_, "Incoming Stint Root ID");
    file.AddVar(outgoing_stint_root_id_, "Outgoing Stint Root ID");

    file.PrintHeaderKeys();

    dish2::log_msg( "initialized stint_root_id record at ", outpath );

  }

  void record_changeover(
    const uint64_t root_id,
    const uint64_t incoming_stint_root_id,
    const uint64_t outgoing_stint_root_id
  ) {

    root_id_ = root_id;
    incoming_stint_root_id_ = incoming_stint_root_id;
    outgoing_stint_root_id_ = outgoing_stint_root_id;

    file.Update();

  }

};

} // namespace dish2

#endif // #ifndef DISH2_RECORD_STINTROOTIDCHANGEOVERRECORDER_HPP_INCLUDE
