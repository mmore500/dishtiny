#pragma once
#ifndef DISH2_GENOME_ROOTID_HPP_INCLUDE
#define DISH2_GENOME_ROOTID_HPP_INCLUDE

#include <atomic>
#include <tuple>
#include <ratio>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

namespace dish2 {

class RootID {

  inline static std::atomic< size_t > root_id_counter{};

  size_t id;

public:

  RootID() = default;

  RootID(std::in_place_t) : id( uitsl::sidebyside_hash< std::ratio<3, 4> >(
    uitsl::audit_cast<size_t>( uitsl::get_proc_id() ),
    root_id_counter++
  ) ) {}

  RootID(const size_t id_) : id( id_ ) {}

  bool operator<(const RootID& other) const {
    return id < other.id;
  }

  bool operator==(const RootID& other) const {
    return id == other.id;
  }

  template <class Archive> void serialize( Archive & ar ) { ar( id ); }

  size_t GetID() const { return id; }

};

} // namespace dish2

#endif // #ifndef DISH2_GENOME_ROOTID_HPP_INCLUDE
