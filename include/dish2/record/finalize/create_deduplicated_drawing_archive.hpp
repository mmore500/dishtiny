#pragma once
#ifndef DISH2_RECORD_FINALIZE_CREATE_DEDUPLICATED_DRAWING_ARCHIVE_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_CREATE_DEDUPLICATED_DRAWING_ARCHIVE_HPP_INCLUDE

#include <cstdlib>
#include <iostream>
#include <string>

#include "../../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../debug/log_msg.hpp"

#include "../make_filename/make_drawing_archive_filename.hpp"
#include "../make_filename/make_zip_path.hpp"

namespace dish2 {

void create_deduplicated_drawing_archive() {

  uitsl::err_verify( std::system(
    "rdfind -makesymlinks true -makeresultsfile false outdrawings/"
  ) );
  dish2::log_msg( "rdfind deduplication complete" );

  const std::string zip_command( emp::to_string(
    "tar -cJf ",
    dish2::make_zip_path( dish2::make_drawing_archive_filename() ),
    " outdrawings/"
  ) );
  uitsl::err_verify( std::system( zip_command.c_str() ) );
  dish2::log_msg( "drawing archive zip complete" );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_CREATE_DEDUPLICATED_DRAWING_ARCHIVE_HPP_INCLUDE
