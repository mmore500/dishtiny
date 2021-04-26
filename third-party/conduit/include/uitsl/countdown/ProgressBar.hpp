#pragma once
#ifndef UITSL_COUNTDOWN_PROGRESSBAR_HPP_INCLUDE
#define UITSL_COUNTDOWN_PROGRESSBAR_HPP_INCLUDE

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "../chrono/chrono_utils.hpp"
#include "../chrono/CoarseClock.hpp"

#include "impl/CountdownIterator.hpp"

namespace uitsl {

template<typename CountdownType>
class ProgressBar : public CountdownType {

  using this_t = uitsl::ProgressBar<CountdownType>;
  using parent_t = CountdownType;

  const size_t bar_width{ 50 };
  const std::string complete = "█";
  const std::string divider = "|";
  const std::string incomplete = "░";

  mutable std::string last{};
  mutable size_t max_width{};

  std::ostream& os;

  using clock_t = uitsl::CoarseClock;

  const decltype(clock_t::now()) start_time = clock_t::now();

  void DrawBar(std::stringstream& ss) const {

    const double progress = parent_t::GetFractionComplete();
    // min protects against progress > 100%
    const size_t position = std::min(
      static_cast<size_t>(progress * bar_width), bar_width
    );

    ss << "|";
    for (size_t i = 0; i < position; ++i) ss << complete;
    if (position < bar_width) {
      ss << divider;
      for (size_t i = 1; i < bar_width - position; ++i) ss << incomplete;
    }
    ss << "| ";

  }

  void DrawPercent(std::stringstream& ss) const {

    const double progress = parent_t::GetFractionComplete();

    std::string percent = std::to_string(static_cast<size_t>(progress * 100.0));

    // pad string
    percent.insert(
      std::begin(percent),
      3 - percent.size(),
      ' '
    );

    ss << percent << "% ";

  }

  void DrawEta(std::stringstream& ss) const {

    const double progress = parent_t::GetFractionComplete();

    const double elapsed = GetElapsedTime().count();
    const double projected = elapsed / progress;
    const int remaining = projected - elapsed;

    const auto fixup = [](std::string eta){
      return eta.size() ? eta : "0s";
    };

    ss << "[";
    ss << fixup( uitsl::beautify_duration(std::chrono::seconds{
      static_cast<int>( elapsed )
    } ) );
    ss << "<";
    ss <<  fixup( uitsl::beautify_duration( std::chrono::seconds{ remaining } ) );
    ss << "]";
  }

  decltype(auto) GetElapsedTime() const {
      return std::chrono::duration_cast<std::chrono::seconds>(
        uitsl::CoarseClock::now() - start_time
      );
    }


public:

  template<typename ...Args>
  ProgressBar(std::ostream& os_, Args&&... args)
  : parent_t(std::forward<Args>(args)...)
  , os(os_) {
    Draw();
  }

  template<typename ...Args>
  ProgressBar(Args&&... args)
  : parent_t(std::forward<Args>(args)...)
  , os(std::cout) {
    Draw();
  }

  ~ProgressBar() {
    Draw();
    os << '\n';
  }

  void Draw() const {
    std::stringstream ss;
    DrawPercent(ss);
    DrawBar(ss);
    DrawEta(ss);

    std::string res{ ss.str() };
    const size_t max_width = std::max( res.size(), last.size() );

    // pad to max_width
    res.append(max_width - res.size(), ' ');

    if ( res != last ) {
      os << res;
      os << "\r";
      os.flush();
    }

    std::swap(res, last);

  }

  this_t& operator++() {
    parent_t::operator++();
    Draw();
    return *this;
  }

  using iterator = uitsl::CountdownIterator<this_t>;

  iterator begin() { return iterator{ *this }; }

  iterator end() { return iterator{}; }

};

} // namespace uitsl

#endif // #ifndef UITSL_COUNTDOWN_PROGRESSBAR_HPP_INCLUDE
