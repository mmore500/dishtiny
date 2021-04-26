#pragma once
#ifndef UIT_SETUP_DEFAULTS_HPP_INCLUDE
#define UIT_SETUP_DEFAULTS_HPP_INCLUDE

#include "../ducts/intra/put=dropping+get=stepping+type=any/a::SerialPendingDuct.hpp"
#include "../ducts/mock/NopDuct.hpp"
#include "../ducts/proc/put=dropping+get=stepping+type=trivial/inlet=RingIsend+outlet=RingIrecv_t::IriOriDuct.hpp"
#include "../ducts/thread/put=dropping+get=stepping+type=any/a::AtomicPendingDuct.hpp"
#include "../spouts/wrappers/TrivialSpoutWrapper.hpp"

namespace uit {

constexpr static size_t DEFAULT_BUFFER = 64;

template<typename Spec>
using DefaultSpoutWrapper = uit::TrivialSpoutWrapper<Spec>;

template<typename Spec>
using DefaultIntraDuct = uit::a::SerialPendingDuct<Spec>;

template<typename Spec>
using DefaultThreadDuct = uit::a::AtomicPendingDuct<Spec>;

template<typename Spec>
using DefaultProcDuct = uit::t::IriOriDuct<Spec>;

template<typename Spec>
using DefaultMockDuct = uit::NopDuct<Spec>;

} // namespace uit

#endif // #ifndef UIT_SETUP_DEFAULTS_HPP_INCLUDE
