#pragma once
#ifndef DISH2_RECORD_TRY_WRITE_ALL_DRAWINGS_HPP_INCLUDE
#define DISH2_RECORD_TRY_WRITE_ALL_DRAWINGS_HPP_INCLUDE

#include <chrono>
#include <utility>

#include "write_all_drawings.hpp"

#include "drawings/DrawerCollection.hpp"

namespace dish2 {

template< typename Spec >
void try_write_all_drawings( const dish2::ThreadWorld< Spec >& thread_world ) {

  dish2::log_msg( "beginning try_write_all_drawings" );

  using namespace std::chrono_literals;

  if ( dish2::try_with_timeout(
    [&](){ write_all_drawings<Spec>( thread_world ); },
    5min
  ) ) dish2::log_msg( "wrote all drawings" );
  else dish2::log_msg( "all drawings write timed out" );

  dish2::log_msg( "try_write_all_drawings complete" );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_TRY_WRITE_ALL_DRAWINGS_HPP_INCLUDE
