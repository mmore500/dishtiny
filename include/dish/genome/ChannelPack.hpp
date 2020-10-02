#pragma once
#ifndef DISH_GENOME_CHANNELPACK_HPP_INCLUDE
#define DISH_GENOME_CHANNELPACK_HPP_INCLUDE

#include "../../../third-party/Empirical/source/base/vector.h"

#include "../config/Config.hpp"

namespace dish {

using ChannelPack = emp::vector<Config::chanid_t>;

} // namespace dish

#endif // #ifndef DISH_GENOME_CHANNELPACK_HPP_INCLUDE
