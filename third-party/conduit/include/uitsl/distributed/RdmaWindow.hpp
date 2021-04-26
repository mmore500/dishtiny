#pragma once
#ifndef UITSL_DISTRIBUTED_RDMAWINDOW_HPP_INCLUDE
#define UITSL_DISTRIBUTED_RDMAWINDOW_HPP_INCLUDE

#include <stddef.h>

#include <mpi.h>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../mpi/audited_routines.hpp"
#include "../mpi/mpi_types.hpp"
#include "../mpi/mpi_utils.hpp"

namespace uitsl {

// TODO is it possible to have a seperate window/communicator
// between each pair of procs?
class RdmaWindow {

  std::byte *buffer;

  emp::optional<MPI_Win> window;

  emp::vector<std::byte> initialization_bytes;

  // this is relative to the window communicator
  // rank where window is located
  proc_id_t local_rank;

public:

  ~RdmaWindow() {
    if (IsInitialized()) {
      UITSL_Win_free(&window.value());
      UITSL_Free_mem(buffer);
    }
  }

  bool IsInitialized() const { return window.has_value(); }

  bool IsUninitialized() const { return !window.has_value(); }

  // returns index
  size_t Acquire(const emp::vector<std::byte>& initial_bytes) {
    emp_assert(IsUninitialized());

    const size_t address = initialization_bytes.size();
    initialization_bytes.insert(
      std::end(initialization_bytes),
      std::begin(initial_bytes),
      std::end(initial_bytes)
    );
    // TODO cache line alignment?
    return address;

  }

  std::byte *GetBytes(const size_t byte_offset) {
    emp_assert( IsInitialized() );

    return std::next(
      reinterpret_cast<std::byte *>(buffer),
      byte_offset
    );

  }

  const MPI_Win & GetWindow() {
    emp_assert( IsInitialized() );

    return window.value();
  }

  void LockExclusive() {

    emp_assert( IsInitialized() );

    UITSL_Win_lock(
      MPI_LOCK_EXCLUSIVE, // int lock_type
      // Indicates whether other processes may access the target window at the
      // same time (if MPI_LOCK_SHARED) or not (MPI_LOCK_EXCLUSIVE)
      local_rank, // int rank
      // rank of locked window (nonnegative integer)
      0, // int assert TODO optimize?
      // Used to optimize this call; zero may be used as a default.
      window.value() // MPI_Win win
      // window object (handle)
    );

  }

  void LockShared() {

    emp_assert( IsInitialized() );

    UITSL_Win_lock(
      MPI_LOCK_SHARED, // int lock_type
      // Indicates whether other processes may access the target window at the
      // same time (if MPI_LOCK_SHARED) or not (MPI_LOCK_EXCLUSIVE)
      local_rank, // int rank
      // rank of locked window (nonnegative integer)
      0, // int assert TODO optimize?
      // Used to optimize this call; zero may be used as a default.
      window.value() // MPI_Win win
      // window object (handle)
    );

  }

  void Unlock() {

    emp_assert( IsInitialized() );

    UITSL_Win_unlock(
      local_rank, // int rank
      // rank of window (nonnegative integer)
      window.value() // MPI_Win win
      // window object (handle)
    );

  }

  void Put(
    const std::byte *origin_addr,
    const size_t num_bytes,
    const MPI_Aint target_disp
  ) {

    emp_assert( IsInitialized() );

    UITSL_Put(
      origin_addr, // const void *origin_addr
      num_bytes, // int origin_count
      MPI_BYTE, // MPI_Datatype origin_datatype
      local_rank, // int target_rank
      target_disp, // MPI_Aint target_disp
      num_bytes, // int target_count
      MPI_BYTE, // MPI_Datatype target_datatype
      window.value() // MPI_Win win
    );

  }

  void Rput(
    const std::byte *origin_addr,
    const size_t num_bytes,
    const MPI_Aint target_disp,
    MPI_Request *request
  ) {

    emp_assert( IsInitialized() );
    emp_assert( *request );

    UITSL_Rput(
      origin_addr, // const void *origin_addr
      num_bytes, // int origin_count
      MPI_BYTE, // MPI_Datatype origin_datatype
      local_rank, // int target_rank
      target_disp, // MPI_Aint target_disp
      num_bytes, // int target_count
      MPI_BYTE, // MPI_Datatype target_datatype
      window.value(), // MPI_Win win
      request // MPI_Request* request (handle)
    );

  }

  template<typename T>
  void Accumulate(
    const std::byte *origin_addr,
    const size_t num_bytes,
    const MPI_Aint target_disp
  ) {

    emp_assert( IsInitialized() );

    UITSL_Accumulate(
      // const void *origin_addr: initial address of buffer (choice)
      origin_addr,
      // int origin_count: number of entries in buffer (nonnegative integer)
      num_bytes / sizeof(T),
      // MPI_Datatype origin_datatype: datatype of each buffer entry (handle)
      uitsl::datatype_from_type<T>(),
      // int target_rank: rank of target (nonnegative integer)
      local_rank,
      // MPI_Aint target_disp
      // displacement from start of window to beginning of target buffer
      // (nonnegative integer)
      target_disp,
      // int target_count
      // number of entries in target buffer (nonnegative integer)
      num_bytes / sizeof(T),
      // MPI_Datatype target_datatype
      // datatype of each entry in target buffer (handle)
      uitsl::datatype_from_type<T>(),
      // MPI_Op op: predefined reduce operation (handle)
      MPI_SUM,
      // MPI_Win win: window object (handle)
      window.value()
    );

  }

  template<typename T>
  void Raccumulate(
    const std::byte *origin_addr,
    const size_t num_bytes,
    const MPI_Aint target_disp,
    MPI_Request *request
  ) {

    emp_assert( IsInitialized() );

    UITSL_Raccumulate(
      // const void *origin_addr: initial address of buffer (choice)
      origin_addr,
      // int origin_count: number of entries in buffer (nonnegative integer)
      num_bytes / sizeof(T),
      // MPI_Datatype origin_datatype: datatype of each buffer entry (handle)
      uitsl::datatype_from_type<T>(),
      // int target_rank: rank of target (nonnegative integer)
      local_rank,
      // MPI_Aint target_disp
      // displacement from start of window to beginning of target buffer
      // (nonnegative integer)
      target_disp,
      // int target_count
      // number of entries in target buffer (nonnegative integer)
      num_bytes / sizeof(T),
      // MPI_Datatype target_datatype
      // datatype of each entry in target buffer (handle)
      uitsl::datatype_from_type<T>(),
      // MPI_Op op: predefined reduce operation (handle)
      MPI_SUM,
      // MPI_Win win: window object (handle)
      window.value(),
      // MPI_Request* request:
      request // RMA request (handle)
    );

  }


  void Initialize(const proc_id_t target, MPI_Comm comm=MPI_COMM_WORLD) {
    emp_assert(IsUninitialized());

    local_rank = target;

    UITSL_Alloc_mem(
      initialization_bytes.size(),
      MPI_INFO_NULL,
      &buffer
    );

    // initialize allocated memory
    std::memcpy(
      buffer,
      initialization_bytes.data(),
      initialization_bytes.size()
    );

    window.emplace();

    // all procs must make this call
    UITSL_Win_create(
      buffer, // base: initial address of window (choice)
      initialization_bytes.size(), // size
      // size of window in bytes (nonnegative integer)
      1, // disp_unit: local unit size for displacements, in bytes
      //   (positive integer)
      MPI_INFO_NULL, // info: info argument (handle)
      comm, // comm: communicator (handle)
      &window.value() // win: window object returned by the call (handle)
    );

    // ensure that RputDucts have received target offsets
    UITSL_Barrier(comm);

    emp_assert( IsInitialized() );

  }

  size_t GetSize() const { return initialization_bytes.size(); }

  proc_id_t GetLocalRank() const { return local_rank; }

  std::string ToString() const {

    std::stringstream ss;
    ss << uitsl::format_member("IsInitialized()", emp::to_string(IsInitialized()))
      << '\n';
    ss << uitsl::format_member("IsUninitialized()", emp::to_string(IsUninitialized()))
      << '\n';
    // TODO add print function for MPI_Win
    ss << uitsl::format_member("std::byte *buffer", static_cast<const void *>(buffer))
      << '\n';
    ss << uitsl::format_member("GetSize()", GetSize()) << '\n';
    ss << uitsl::format_member("proc_id_t local_rank", local_rank);

    return ss.str();

  }


};

} // namespace uitsl

#endif // #ifndef UITSL_DISTRIBUTED_RDMAWINDOW_HPP_INCLUDE
