#pragma once
#ifndef UITSL_DISTRIBUTED_MSGACCUMULATORBUNDLE_HPP_INCLUDE
#define UITSL_DISTRIBUTED_MSGACCUMULATORBUNDLE_HPP_INCLUDE

#include <algorithm>
#include <cstring>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/polyfill/span.hpp"

#include "../math/divide_utils.hpp"

namespace uitsl {

template<typename T>
class MsgAccumulatorBundle {

  constexpr static size_t epoch_embed_items {
     uitsl::div_ceil(sizeof(size_t), sizeof(T))
  };

  size_t data_size;
  // holds data within size then epoch past the end
  // within reserved capacity
  emp::vector<T> buff;

  size_t const size() const { return buff.size(); }

public:

  T* data() { return buff.data(); }

  const T* data() const { return buff.data(); }

  size_t byte_size() const {
    return sizeof(T) * data_size + sizeof(size_t);
  }

  emp::vector<T>& GetData() { return buff; }

  const emp::vector<T>& GetData() const { return buff; }

  void Reset() {
    buff.reserve(data_size + epoch_embed_items);
    buff.resize(data_size);
    std::fill(
      std::begin(buff),
      std::end(buff),
      T{}
    );
    SetEpoch( size_t{} );
  }

  size_t GetEpoch() const {
    emp_assert( buff.size() == data_size );
    size_t res;
    std::memcpy(
      &res,
      buff.data() + data_size,
      sizeof(size_t)
    );
    return res;
  }

  void SetEpoch(const size_t val) {
    emp_assert( buff.size() == data_size );
    std::memcpy(
      buff.data() + data_size,
      &val,
      sizeof(size_t)
    );
  }

  void BumpEpoch(const size_t amt=1) {
    SetEpoch( GetEpoch() + amt );
  }

  void BumpData(const emp::vector<T>& bumps) {
    emp_assert( buff.size() == data_size );
    emp_assert( bumps.size() == data_size  );
    std::transform(
      std::begin(bumps),
      std::end(bumps),
      std::begin(buff),
      std::begin(buff),
      std::plus<T>{}
    );
  }

  explicit MsgAccumulatorBundle(const size_t data_size_)
  : data_size( data_size_ )
  { Reset(); }

  explicit MsgAccumulatorBundle(const emp::vector<T>& data)
  : data_size( data.size() )
  , buff( data )
  {
    buff.reserve( data_size + epoch_embed_items);
    SetEpoch( size_t{} );
  }

  explicit MsgAccumulatorBundle(emp::vector<T>&& data)
  : data_size( data.size() )
  , buff( std::move(data) )
  {
    buff.reserve( data_size + epoch_embed_items);
    SetEpoch( size_t{} );
  }

  MsgAccumulatorBundle(MsgAccumulatorBundle &&) = default;
  MsgAccumulatorBundle& operator=(MsgAccumulatorBundle &&) = default;

  MsgAccumulatorBundle& operator+=(const MsgAccumulatorBundle& rhs) {
    BumpData( rhs.GetData() );
    BumpEpoch( rhs.GetEpoch() );
    return *this;
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_DISTRIBUTED_MSGACCUMULATORBUNDLE_HPP_INCLUDE
