#pragma once
#ifndef SGPL_PROGRAM_PROGRAM_HPP_INCLUDE
#define SGPL_PROGRAM_PROGRAM_HPP_INCLUDE

#include <algorithm>
#include <cstddef>

#include "../../../third-party/cereal/include/cereal/types/vector.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"
#include "../../../third-party/Empirical/include/emp/math/Random.hpp"
#include "../../../third-party/Empirical/include/emp/polyfill/span.hpp"

#include "../algorithm/mutate_bits.hpp"
#include "../utility/ThreadLocalRandom.hpp"

#include "Instruction.hpp"
#include "OpCodeRectifier.hpp"

namespace sgpl {

template<typename Spec>
class Program : public emp::vector<sgpl::Instruction<Spec>> {

  using parent_t = emp::vector<sgpl::Instruction<Spec>>;
  using library_t = typename Spec::library_t;
  using rectifier_t = sgpl::OpCodeRectifier<library_t>;

  size_t size_bytes() const {
    return this->size() * sizeof( typename parent_t::value_type );
  }

public:

  /// Default constructor.
  Program() = default;

  Program( const size_t n ) : parent_t( n ) {
    sgpl::tlrand.Get().RandFill(
      reinterpret_cast<unsigned char*>( this->data() ),
      size_bytes()
    );
    Rectify();
  }

  /// Copy constructor.
  Program( const Program& other ) {
    // copy all bytes, including padding bytes so that we can
    // reliably perform a bytewise hash
    parent_t::resize( other.size() );
    std::memcpy(
      reinterpret_cast< std::byte* >( parent_t::data() ),
      reinterpret_cast< const std::byte* >( other.data() ),
      parent_t::size() * sizeof( other.front() )
    );
  }

  /// Move constructor.
  Program( Program&& other ) : parent_t( std::move(other) ) {}

  /// Raw copy constructor.
  Program( const parent_t& other ) {
    // copy all bytes, including padding bytes so that we can
    // reliably perform a bytewise hash
    parent_t::resize( other.size() );
    std::memcpy(
      reinterpret_cast< std::byte* >( parent_t::data() ),
      reinterpret_cast< const std::byte* >( other.data() ),
      parent_t::size() * sizeof( other.front() )
    );
  }

  /// Raw move constructor.
  Program( parent_t&& other ) : parent_t( std::move(other) ) {}

  /// Copy assignment operator.
  Program& operator=(const Program& other) {
    // copy all bytes, including padding bytes so that we can
    // reliably perform a bytewise hash
    parent_t::resize( other.size() );
    std::memcpy(
      reinterpret_cast< std::byte* >( parent_t::data() ),
      reinterpret_cast< const std::byte* >( other.data() ),
      parent_t::size() * sizeof( other.front() )
    );
    return *this;
  }

  /// Move assignment operator.
  Program& operator=(Program&& other) {
    parent_t::operator=( std::move(other) );
    return *this;
 }

  /// Raw move assignment operator.
  Program& operator=(parent_t&& other) {
    parent_t::operator=( std::move(other) );
    return *this;
 }

  size_t ApplyPointMutations(
    const float p_bit_toggle, const rectifier_t& rectifier=rectifier_t{}
  ) {

    // bail early for p_bit_toggle == 0
    if ( p_bit_toggle == 0 ) return 0;

    // ideally, we would draw from the binomial distn,
    // but that's expensive with varying n...
    // so approximate with the poisson distribution instead
    // they're similar-ish, e.g., https://www.researchgate.net/figure/Poisson-versus-binomial-distribution-from-number-of-heads-in-a-coin-toss-The-Poisson_fig3_255717571
    // (they become more similar for large n)
    const size_t n_muts = sgpl::tlrand.Get().GetRandPoisson(
      size_bytes() * CHAR_BIT,
      p_bit_toggle
    );

    sgpl::mutate_bits(
      std::span<std::byte>(
        reinterpret_cast<std::byte*>( this->data() ),
        size_bytes()
      ),
      n_muts
    );

    Rectify(rectifier);

    return n_muts;

  }

  void RotateGlobalAnchorToFront() {

    const auto first_anchor = std::find_if(
      this->begin(),
      this->end(),
      []( const auto& instruction ) {
        return library_t::IsAnchorGlobalOpCode( instruction.op_code );
      }
    );

    if ( first_anchor != this->end() ) std::rotate(
      this->begin(),
      first_anchor,
      this->end()
    );

  }

  void Rectify(const rectifier_t& rectifier=rectifier_t{}) {
    for (auto& inst : *this) inst.Rectify(rectifier);
  }


};

} // namespace sgpl

namespace std {

template <typename Spec>
struct hash<sgpl::Program<Spec>> {

  size_t operator()( const sgpl::Program<Spec>& program ) const {
    return emp::murmur_hash( std::span<const std::byte>(
      reinterpret_cast<const std::byte*>( program.data() ),
      program.size() * sizeof( program.front() )
    ) );
  }

};

} // namespace std


#endif // #ifndef SGPL_PROGRAM_PROGRAM_HPP_INCLUDE
