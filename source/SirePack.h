#pragma once

#include "base/Ptr.h"
#include "base/vector.h"

#include "Config.h"
#include "ChannelPack.h"

struct SirePack {

  size_t par_pos;
  size_t dir;
  size_t rep_lev;
  emp::vector<size_t> channel_gens;
  ChannelPack chanpack;

};
