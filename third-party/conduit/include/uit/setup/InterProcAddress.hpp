#pragma once
#ifndef UIT_SETUP_INTERPROCADDRESS_HPP_INCLUDE
#define UIT_SETUP_INTERPROCADDRESS_HPP_INCLUDE

#include <tuple>

#include <mpi.h>

#include "../../uitsl/mpi/mpi_utils.hpp"
#include "../../uitsl/parallel/thread_utils.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
class InterProcAddress {

  uitsl::proc_id_t outlet_proc;
  uitsl::proc_id_t inlet_proc;
  uitsl::thread_id_t outlet_thread;
  uitsl::thread_id_t inlet_thread;
  int tag;
  MPI_Comm comm;

public:

  InterProcAddress(
    const uitsl::proc_id_t outlet_proc_=0,
    const uitsl::proc_id_t inlet_proc_=0,
    const uitsl::thread_id_t outlet_thread_=0,
    const uitsl::thread_id_t inlet_thread_=0,
    const int tag_=0,
    const MPI_Comm comm_=MPI_COMM_WORLD
  ) : outlet_proc(outlet_proc_)
  , inlet_proc(inlet_proc_)
  , outlet_thread(outlet_thread_)
  , inlet_thread(inlet_thread_)
  , tag(tag_)
  , comm(comm_)
  { ; }

  uitsl::proc_id_t GetOutletProc() const { return outlet_proc; }

  uitsl::proc_id_t GetInletProc() const { return inlet_proc; }

  uitsl::thread_id_t GetOutletThread() const { return outlet_thread; }

  uitsl::thread_id_t GetInletThread() const { return inlet_thread; }

  int GetTag() const { return tag; }

  MPI_Comm GetComm() const { return comm; }

  bool operator<(const InterProcAddress& other) const {
    return std::make_tuple(
      outlet_proc,
      inlet_proc,
      outlet_thread,
      inlet_thread,
      tag,
      comm
    ) < std::make_tuple(
      other.outlet_proc,
      other.inlet_proc,
      other.outlet_thread,
      other.inlet_thread,
      other.tag,
      other.comm
    );
  }

  bool operator==(const InterProcAddress& other) const {
    return std::make_tuple(
      outlet_proc,
      inlet_proc,
      outlet_thread,
      inlet_thread,
      tag,
      comm
    ) == std::make_tuple(
      other.outlet_proc,
      other.inlet_proc,
      other.outlet_thread,
      other.inlet_thread,
      other.tag,
      other.comm
    );
  }

  bool operator!=(const InterProcAddress& other) const {
    return !operator==(other);
  }

  std::string ToString() const {

    std::stringstream ss;
    ss << uitsl::format_member("uitsl::proc_id_t outlet_proc", outlet_proc) << '\n';
    ss << uitsl::format_member("uitsl::proc_id_t inlet_proc", inlet_proc) << '\n';
    ss << uitsl::format_member(
      "uitsl::thread_id_t outlet_thread",
      outlet_thread
    ) << '\n';
    ss << uitsl::format_member(
      "uitsl::thread_id_t inlet_thread",
      inlet_thread
    ) << '\n';
    ss << uitsl::format_member("int tag", tag) << '\n';
    ss << uitsl::format_member("MPI_Comm comm", uitsl::comm_to_string(comm));

    return ss.str();
  }

  size_t GetHash() const {
    return
      std::hash<decltype(outlet_proc)>{}(outlet_proc)
      ^ std::hash<decltype(inlet_proc)>{}(inlet_proc)
      ^ std::hash<decltype(outlet_thread)>{}(outlet_thread)
      ^ std::hash<decltype(inlet_thread)>{}(inlet_thread)
      ^ std::hash<decltype(tag)>{}(tag)
      ^ std::hash<decltype(comm)>{}(comm)
    ;
  }

  auto WhichProcsThreads() const {
    return std::tuple{
      GetInletProc(),
      GetInletThread(),
      GetOutletProc(),
      GetOutletThread(),
    };
  }

};

} // namespace uit

namespace std {

template<>
struct hash<uit::InterProcAddress> {

  size_t operator()(uit::InterProcAddress const& address) const {
    return address.GetHash();
  }

};

} // namespace std

#endif // #ifndef UIT_SETUP_INTERPROCADDRESS_HPP_INCLUDE
