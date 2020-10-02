#pragma once
#ifndef DISH_CHANNELPACK_HPP_INCLUDE
#define DISH_CHANNELPACK_HPP_INCLUDE

#include "../../third-party/Empirical/source/base/vector.h"

#include "Config.hpp"

namespace dish {

using ChannelPack = emp::vector<Config::chanid_t>;

} // namespace dish

#endif // #ifndef DISH_CHANNELPACK_HPP_INCLUDE
