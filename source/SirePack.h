#pragma once

#include "base/Ptr.h"
#include "base/vector.h"

#include "Config.h"
#include "ChannelPack.h"

struct SirePack {

  size_t par_pos;
  size_t outgoing_dir;
  size_t replev;
  emp::vector<size_t> channel_gens;
  ChannelPack chanpack;
  Config::chanid_t prev_chan;

};
