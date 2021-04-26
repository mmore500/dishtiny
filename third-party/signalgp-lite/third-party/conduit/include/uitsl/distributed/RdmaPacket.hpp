#pragma once
#ifndef UITSL_DISTRIBUTED_RDMAPACKET_HPP_INCLUDE
#define UITSL_DISTRIBUTED_RDMAPACKET_HPP_INCLUDE

#include <tuple>
#include <type_traits>

namespace uitsl {

template<typename T>
class RdmaPacket {

  static_assert(std::is_trivially_copyable<T>::value);

  T data{};
  size_t epoch{};

public:

  RdmaPacket() = default;

  RdmaPacket(const T& data_, const size_t& epoch_)
  : data(data_), epoch(epoch_)
  { ; }

  const T& GetData() const { return data; }

  T& GetData() { return data; }

  const size_t& GetEpoch() const { return epoch; }

};

} // namespace uitsl

#endif // #ifndef UITSL_DISTRIBUTED_RDMAPACKET_HPP_INCLUDE
