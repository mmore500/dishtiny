#pragma once
#ifndef UITSL_MPI_MPI_TYPES_HPP_INCLUDE
#define UITSL_MPI_MPI_TYPES_HPP_INCLUDE

#include <cstddef>

#include <mpi.h>

namespace uitsl {

  // adapted from from https://forum.hdfgroup.org/t/templatized-instantiation-of-h5-native-xxx-types/4168/2
  inline MPI_Datatype datatype_from_type_t( char ) { return MPI_CHAR; }

  inline MPI_Datatype datatype_from_type_t( signed char ) {
    return MPI_SIGNED_CHAR;
  }

  inline MPI_Datatype datatype_from_type_t( unsigned char ) {
    return MPI_UNSIGNED_CHAR;
  }

  inline MPI_Datatype datatype_from_type_t( std::byte ) { return MPI_BYTE; }

  inline MPI_Datatype datatype_from_type_t( wchar_t ) { return MPI_WCHAR; }

  inline MPI_Datatype datatype_from_type_t( short ) { return MPI_SHORT; }

  inline MPI_Datatype datatype_from_type_t( unsigned short ) {
    return MPI_UNSIGNED_SHORT;
  }

  inline MPI_Datatype datatype_from_type_t( int ) { return MPI_INT; }

  inline MPI_Datatype datatype_from_type_t( unsigned int ) { return MPI_UNSIGNED; }

  inline MPI_Datatype datatype_from_type_t( long ) { return MPI_LONG; }

  inline MPI_Datatype datatype_from_type_t( unsigned long ) {
    return MPI_UNSIGNED_LONG;
  }

  inline MPI_Datatype datatype_from_type_t( long long ) { return MPI_LONG_LONG_INT; }

  inline MPI_Datatype datatype_from_type_t( unsigned long long ) {
    return MPI_UNSIGNED_LONG_LONG;
  }

  inline MPI_Datatype datatype_from_type_t( float ) { return MPI_FLOAT; }

  inline MPI_Datatype datatype_from_type_t( double ) { return MPI_DOUBLE; }

  inline MPI_Datatype datatype_from_type_t( long double ) { return MPI_LONG_DOUBLE; }

  // these conflict with char, int, long, etc.
  //
  // inline MPI_Datatype datatype_from_type_t( int8_t ) { return MPI_INT8_T; }
  //
  // inline MPI_Datatype datatype_from_type_t( int16_t ) { return MPI_INT16_T; }
  //
  // inline MPI_Datatype datatype_from_type_t( int32_t ) { return MPI_INT32_T; }
  //
  // inline MPI_Datatype datatype_from_type_t( int64_t ) { return MPI_INT64_T; }
  //
  // inline MPI_Datatype datatype_from_type_t( uint8_t ) { return MPI_UINT8_T; }
  //
  // inline MPI_Datatype datatype_from_type_t( uint16_t ) { return MPI_UINT16_T; }
  //
  // inline MPI_Datatype datatype_from_type_t( uint32_t ) { return MPI_UINT32_T; }
  //
  // inline MPI_Datatype datatype_from_type_t( uint64_t ) { return MPI_UINT64_T; }

  inline MPI_Datatype datatype_from_type_t( bool ) { return MPI_C_BOOL; }

  inline MPI_Datatype datatype_from_type_t( float _Complex ) {
    return MPI_C_FLOAT_COMPLEX;
  }

  inline MPI_Datatype datatype_from_type_t( double _Complex ) {
    return MPI_C_DOUBLE_COMPLEX;
  }

  inline MPI_Datatype datatype_from_type_t( long double _Complex ) {
    return MPI_C_LONG_DOUBLE_COMPLEX;
  }

  /// Determine inline MPI_Datatype type
  template <typename T>
  inline MPI_Datatype datatype_from_type() { return datatype_from_type_t( T{} ); }

} // namespace uitsl

#endif // #ifndef UITSL_MPI_MPI_TYPES_HPP_INCLUDE
