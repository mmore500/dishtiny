#pragma once
#ifndef UITSL_DISTRIBUTED_MSGACCUMULATORPACKET_HPP_INCLUDE
#define UITSL_DISTRIBUTED_MSGACCUMULATORPACKET_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/array.hpp"

#include "../utility/NamedArrayElement.hpp"

namespace uitsl {

template<typename T>
class MsgAccumulatorPacket {

  T data{};
  size_t epoch{};

public:

  MsgAccumulatorPacket() = default;

  MsgAccumulatorPacket(const T& data_, const size_t& epoch_) {
    data = data_;
    epoch = epoch_;
  }

  MsgAccumulatorPacket& operator+=(const MsgAccumulatorPacket& rhs) {
    GetData() += rhs.GetData();
    BumpEpoch(rhs.GetEpoch());
    return *this;
  }

  T& GetData() { return data; }

  const T& GetData() const { return data; }

  size_t GetEpoch() const { return epoch; }

  void BumpEpoch(const size_t amt=1) { epoch += amt; }

};

} // namespace uitsl

#endif // #ifndef UITSL_DISTRIBUTED_MSGACCUMULATORPACKET_HPP_INCLUDE
