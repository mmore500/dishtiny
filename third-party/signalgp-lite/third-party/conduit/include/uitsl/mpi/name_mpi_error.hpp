#pragma once
#ifndef UITSL_MPI_NAME_MPI_ERROR_HPP_INCLUDE
#define UITSL_MPI_NAME_MPI_ERROR_HPP_INCLUDE

#include <mpi.h>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

namespace uitsl {

inline std::string name_mpi_error(const int err) {

  switch(err) {

    case MPI_SUCCESS:
      return "MPI_SUCCESS";
    case MPI_ERR_BUFFER:
      return "MPI_ERR_BUFFER";
    case MPI_ERR_COUNT:
      return "MPI_ERR_COUNT";
    case MPI_ERR_TYPE:
      return "MPI_ERR_TYPE";
    case MPI_ERR_TAG:
      return "MPI_ERR_TAG";
    case MPI_ERR_COMM:
      return "MPI_ERR_COMM";
    case MPI_ERR_RANK:
      return "MPI_ERR_RANK";
    case MPI_ERR_ROOT:
      return "MPI_ERR_ROOT";
    case MPI_ERR_GROUP:
      return "MPI_ERR_GROUP";
    case MPI_ERR_OP:
      return "MPI_ERR_OP";
    case MPI_ERR_TOPOLOGY:
      return "MPI_ERR_TOPOLOGY";
    case MPI_ERR_DIMS:
      return "MPI_ERR_DIMS";
    case MPI_ERR_ARG:
      return "MPI_ERR_ARG";
    case MPI_ERR_UNKNOWN:
      return "MPI_ERR_UNKNOWN";
    case MPI_ERR_TRUNCATE:
      return "MPI_ERR_TRUNCATE";
    case MPI_ERR_OTHER:
      return "MPI_ERR_OTHER";
    case MPI_ERR_INTERN:
      return "MPI_ERR_INTERN";
    case MPI_ERR_IN_STATUS:
      return "MPI_ERR_IN_STATUS";
    case MPI_ERR_PENDING:
      return "MPI_ERR_PENDING";
    case MPI_ERR_REQUEST:
      return "MPI_ERR_REQUEST";
    case MPI_ERR_ACCESS:
      return "MPI_ERR_ACCESS";
    case MPI_ERR_AMODE:
      return "MPI_ERR_AMODE";
    case MPI_ERR_BAD_FILE:
      return "MPI_ERR_BAD_FILE";
    case MPI_ERR_CONVERSION:
      return "MPI_ERR_CONVERSION";
    case MPI_ERR_DUP_DATAREP:
      return "MPI_ERR_DUP_DATAREP";
    case MPI_ERR_FILE_EXISTS:
      return "MPI_ERR_FILE_EXISTS";
    case MPI_ERR_FILE_IN_USE:
      return "MPI_ERR_FILE_IN_USE";
    case MPI_ERR_FILE:
      return "MPI_ERR_FILE";
    case MPI_ERR_INFO:
      return "MPI_ERR_INFO";
    case MPI_ERR_INFO_KEY:
      return "MPI_ERR_INFO_KEY";
    case MPI_ERR_INFO_VALUE:
      return "MPI_ERR_INFO_VALUE";
    case MPI_ERR_INFO_NOKEY:
      return "MPI_ERR_INFO_NOKEY";
    case MPI_ERR_IO:
      return "MPI_ERR_IO";
    case MPI_ERR_NAME:
      return "MPI_ERR_NAME";
    case MPI_ERR_NO_MEM:
      return "MPI_ERR_NO_MEM";
    case MPI_ERR_NOT_SAME:
      return "MPI_ERR_NOT_SAME";
    case MPI_ERR_NO_SPACE:
      return "MPI_ERR_NO_SPACE";
    case MPI_ERR_NO_SUCH_FILE:
      return "MPI_ERR_NO_SUCH_FILE";
    case MPI_ERR_PORT:
      return "MPI_ERR_PORT";
    case MPI_ERR_QUOTA:
      return "MPI_ERR_QUOTA";
    case MPI_ERR_READ_ONLY:
      return "MPI_ERR_READ_ONLY";
    case MPI_ERR_SERVICE:
      return "MPI_ERR_SERVICE";
    case MPI_ERR_SPAWN:
      return "MPI_ERR_SPAWN";
    case MPI_ERR_UNSUPPORTED_DATAREP:
      return "MPI_ERR_UNSUPPORTED_DATAREP";
    case MPI_ERR_UNSUPPORTED_OPERATION:
      return "MPI_ERR_UNSUPPORTED_OPERATION";
    case MPI_ERR_WIN:
      return "MPI_ERR_WIN";
    case MPI_ERR_BASE:
      return "MPI_ERR_BASE";
    case MPI_ERR_LOCKTYPE:
      return "MPI_ERR_LOCKTYPE";
    case MPI_ERR_KEYVAL:
      return "MPI_ERR_KEYVAL";
    case MPI_ERR_RMA_CONFLICT:
      return "MPI_ERR_RMA_CONFLICT";
    case MPI_ERR_RMA_SYNC:
      return "MPI_ERR_RMA_SYNC";
    case MPI_ERR_SIZE:
      return "MPI_ERR_SIZE";
    case MPI_ERR_DISP:
      return "MPI_ERR_DISP";
    case MPI_ERR_ASSERT:
      return "MPI_ERR_ASSERT";
    case MPI_ERR_LASTCODE:
      return "MPI_ERR_LASTCODE";
    default:
      return "unknown";

  }

}

} // namespace uitsl

#endif // #ifndef UITSL_MPI_NAME_MPI_ERROR_HPP_INCLUDE
