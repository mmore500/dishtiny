#pragma once
#ifndef DISH2_DEBUG_LOGTEE_HPP_INCLUDE
#define DISH2_DEBUG_LOGTEE_HPP_INCLUDE

#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>

#include "../record/make_filename/make_log_filename.hpp"
#include "../record/make_filename/make_meta_path.hpp"
#include "../utility/mkdir_exists_ok.hpp"
#include "../utility/pare_keyname_filename_impl.hpp"

#include "make_log_entry_boilerplate.hpp"

namespace dish2 {

// adapted from https://stackoverflow.com/a/1761027
class LogTee : public std::ostream
{

  struct ComposeBuffer: public std::streambuf {

    void add_buffer( std::streambuf* buf ) { bufs.push_back(buf); }

    virtual int overflow( const int c ) {
      std::for_each(
        std::begin( bufs ), std::end( bufs ),
        [c]( auto& buf ){ buf->sputc( c ); }
      );
      return c;
    }

    emp::vector<std::streambuf*> bufs;

  };

  ComposeBuffer buffer;
  std::ofstream logfile_out;

  void link_stream( std::ostream& out ) {
    out.flush();
    buffer.add_buffer( out.rdbuf() );
  }

  public:

  LogTee() : std::ostream(nullptr) {

    std::ostream::rdbuf(&buffer);

    dish2::mkdir_exists_ok( "outmeta" );
    const std::string out_filename = dish2::pare_keyname_filename_impl(
      dish2::make_log_filename(),
      dish2::make_meta_path()
    );
    const std::string out_path = dish2::make_meta_path( out_filename );

    logfile_out = std::ofstream( out_path, std::ios_base::app );

    link_stream( logfile_out );
    link_stream( std::clog );

    *this << dish2::make_log_entry_boilerplate();
    *this << "set up logfile at " << out_path << '\n' << '\n';

  }

};

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOGTEE_HPP_INCLUDE
