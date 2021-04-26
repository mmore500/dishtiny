#pragma once
#ifndef UITSL_DATASTRUCTS_MIRROREDRINGBUFFER_HPP_INCLUDE
#define UITSL_DATASTRUCTS_MIRROREDRINGBUFFER_HPP_INCLUDE

#include <sys/mman.h>
#include <unistd.h>

#include "../../../third-party/Empirical/include/emp/polyfill/span.hpp"

#include "../debug/err_audit.hpp"
#include "../debug/safe_cast.hpp"
#include "../math/divide_utils.hpp"

// adapted from https://medium.com/@abhinavagarwal1996/a-fast-circular-ring-buffer-4d102ef4d4a3
// see also https://lo.calho.st/posts/black-magic-buffer/

// TODO fix undefined behavior

namespace uitsl {

template<typename T, size_t N>
class MirroredRingBuffer {

  const size_t byte_size { uitsl::safe_cast<size_t>(
    uitsl::div_ceil(N * sizeof(T), getpagesize()) * getpagesize()
  ) };
  const size_t allocation_size { 2 * byte_size };
  const int file_descriptor { fileno( tmpfile() ) };
  std::byte *buffer;

  std::byte *tail;
  size_t num_items{};

public:

  MirroredRingBuffer() {

    emp_assert( byte_size % getpagesize() == 0 );

    uitsl_err_audit(ftruncate(
      file_descriptor, // int fd
      byte_size // off_t length
    ));

    // get virtual address space of 2 * byte_size for our buffer
    buffer = reinterpret_cast<std::byte*>( mmap(
      nullptr, // void *addr
      allocation_size, // size_t length
      PROT_NONE, // int prot
      MAP_PRIVATE | MAP_ANONYMOUS, // int flags
      -1, // int fd
      0 // off_t offset
    ) );

    emp_assert( buffer != MAP_FAILED ); emp_assert( buffer != nullptr );

    // map front half of our buffer to underlying buffer
    { const auto res = mmap(
      buffer, // void *addr
      byte_size, // size_t length
      PROT_READ | PROT_WRITE, // int prot
      MAP_SHARED | MAP_FIXED, // int flags
      file_descriptor, // int fd
      0 // off_t offset
    ); emp_assert( res != MAP_FAILED ); }

    // map back half of our buffer to underlying buffer
    { const auto res = mmap(
      buffer + byte_size, // void *addr
      byte_size, // size_t length
      PROT_READ | PROT_WRITE, // int prot
      MAP_SHARED | MAP_FIXED, // int flags
      file_descriptor, // int fd
      0 // off_t offset
    ); emp_assert( res != MAP_FAILED ); }

    // point tail beginning of buffer
    tail = buffer;

  }

  ~MirroredRingBuffer() {
    uitsl_err_audit(munmap(
      buffer, // void *addr
      allocation_size // size_t length
    ));
  }

  std::byte* step_pointer(std::byte* curr, size_t num_steps=1) const {
    emp_assert( num_steps <= N );
    curr += num_steps * sizeof(T);
    // wraparound if off the end
    curr -= (std::distance(buffer, curr) / byte_size) * byte_size;
    emp_assert( std::distance(buffer, curr) >= 0 );
    emp_assert( static_cast<size_t>(std::distance(buffer, curr)) < byte_size );
    return curr;
  }

  size_t GetSize() const { return num_items; }

  bool PushHead(const T& t=T{}) {
    if (GetSize() == N) return false;
    else {
      std::memcpy( GetPastHeadPtr(), &t, sizeof(T) );
      ++num_items;
      return true;
    }
  }

  size_t PopTail(const size_t n=1) {
    const size_t num_popped = std::min(n, GetSize());
    tail = step_pointer(tail, num_popped);
    num_items -= num_popped;
    return num_popped;
  }

  void Fill(const T& t=T{}) { while( PushHead(t) ); }

  T* GetPastHeadPtr() { return GetTailPtr() + GetSize(); }

  const T* GetPastHeadPtr() const { return GetTailPtr() + GetSize(); }

  T* GetHeadPtr() {
    emp_assert( GetSize() );
    return GetPastHeadPtr() - 1;
  }

  const T* GetHeadPtr() const {
    emp_assert( GetSize() );
    return GetPastHeadPtr() - 1;
  }

  T* GetTailPtr() { return reinterpret_cast<T*>(tail); }

  const T* GetTailPtr() const { return reinterpret_cast<const T*>(tail); }

  T Get(const size_t i) const {
    emp_assert( i < GetSize() );
    T res;
    std::memcpy( &res, GetTailPtr() + i, sizeof(T) );
    return res;
  }

  T GetHead() const {
    emp_assert( GetSize() );
    return Get( GetSize() - 1 );
  }

  T GetTail() const {
    emp_assert( GetSize() );
    return Get(0);
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_DATASTRUCTS_MIRROREDRINGBUFFER_HPP_INCLUDE
