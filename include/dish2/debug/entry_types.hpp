#pragma once
#ifndef DISH2_DEBUG_ENTRY_TYPES_HPP_INCLUDE
#define DISH2_DEBUG_ENTRY_TYPES_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "LogEntryType.hpp"

namespace dish2 {

// { "🐛", "🦋" },
// { "🎨", "🖼" },
// { "🏗", "🏢" },
// { "🛫", "🛬" },
// { "🌱", "💐" },
// { "🏎", "🏁" },
// { "👶", "🧓" },
// { "🌧", "⛅" },

inline static const dish2::LogEntryType wait{
  9,
  "💤",
  "WAIT"
};

inline static const dish2::LogEntryType scope_begin{
  2,
  "🥚",
  "BEGIN"
};

inline static const dish2::LogEntryType scope_end{
  2,
  "🐣",
  "END"
};


inline static const dish2::LogEntryType result_success{
  1,
  "✅",
  "SUCCESS"
};

inline static const dish2::LogEntryType result_fail{
  1,
  "❌",
  "FAIL"
};

inline static const dish2::LogEntryType result_skip{
  1,
  "↪️",
  "SKIP"
};

inline static const dish2::LogEntryType info{
  0,
  "📝",
  "INFO"
};

inline static const dish2::LogEntryType warning{
  -1,
  "⚠️",
  "WARNING"
};

inline static const dish2::LogEntryType error{
  -2,
  "💣",
  "ERROR"
};

inline static const dish2::LogEntryType fatal{
  -3,
  "💥",
  "FATAL"
};

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_ENTRY_TYPES_HPP_INCLUDE
