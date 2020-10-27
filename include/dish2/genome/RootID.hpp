#pragma once
#ifndef DISH2_GENOME_ROOTID_HPP_INCLUDE
#define DISH2_GENOME_ROOTID_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

#include <atomic>
#include <tuple>
#include <utility>

namespace dish2 {

class RootID {

  inline static std::atomic< size_t > root_id_counter{};

  size_t id;

public:

  RootID() = default;

  RootID(std::in_place_t) : id( uitsl::sidebyside_hash(
    root_id_counter++,
    uitsl::audit_cast<size_t>( uitsl::get_proc_id() )
  ) ) {}

  RootID(const size_t id_) : id( id_ ) {}

  bool operator==(const RootID& other) const {
    return root_id_counter == other.root_id_counter;
  }

  template <class Archive> void serialize( Archive & ar ) { ar( id ); }

  size_t GetID() const { return id; }

};

} // namespace dish2

#endif // #ifndef DISH2_GENOME_ROOTID_HPP_INCLUDE
