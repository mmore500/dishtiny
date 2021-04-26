#pragma once
#ifndef UITSL_DISTRIBUTED_CACHEPACKET_HPP_INCLUDE
#define UITSL_DISTRIBUTED_CACHEPACKET_HPP_INCLUDE

#include <cstddef>

#include "../../../third-party/cereal/include/cereal/types/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"

namespace uitsl {

template<typename T>
class CachePacket {

  size_t cache_id{};

  emp::optional<T> data{ T{} };

public:

  CachePacket() = default;

  explicit CachePacket(const size_t cache_id_)
  : cache_id(cache_id_)
  , data{ std::nullopt }
  { ; }

  CachePacket(
    const size_t cache_id_,
    const T& data_
  ) : cache_id(cache_id_)
  , data(data_)
  { ; }

  template<typename P>
  CachePacket(
    const size_t cache_id_,
    P&& data_
  ) : cache_id(cache_id_)
  , data( std::forward<P>(data_) )
  { ; }

  bool HasData() const { return data.has_value(); }

  T& GetData() { return *data; }

  const T& GetData() const { return *data; }

  size_t GetID() const { return cache_id; }

  template<class Archive>
  void serialize(Archive & archive) { archive( cache_id, data ); }

};

} // namespace uitsl

#endif // #ifndef UITSL_DISTRIBUTED_CACHEPACKET_HPP_INCLUDE
