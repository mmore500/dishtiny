#pragma once

#include "base/vector.h"

#include "Config.h"

class ChannelPack : public emp::vector<Config::chanid_t> { };
