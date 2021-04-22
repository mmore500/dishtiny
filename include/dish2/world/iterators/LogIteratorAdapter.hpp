#pragma once
#ifndef DISH2_WORLD_ITERATORS_LOGITERATORADAPTER_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_LOGITERATORADAPTER_HPP_INCLUDE

#include <tuple>

#include "../../runninglog/RunningLog.hpp"
#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Event, typename CellIterator >
class LogIteratorAdapter {

  using log_t = dish2::RunningLog<Event>;
  using LogIterator = typename log_t::const_iterator;
  LogIterator log_iterator;
  CellIterator cell_iterator;
  CellIterator end_cell_iterator;

  void Rectify() {
    while (
      cell_iterator + 1 != end_cell_iterator
      && log_iterator == cell_iterator->running_logs.template GetLog<
        Event
      >().end()
    ) {
      ++cell_iterator;
      log_iterator = cell_iterator->running_logs.template GetLog<
        Event
      >().begin();
    }
  }

  LogIteratorAdapter(
    LogIterator cardinal_iterator_,
    CellIterator cell_iterator_,
    CellIterator end_cell_iterator_
  ) : log_iterator( cardinal_iterator_ )
  , cell_iterator( cell_iterator_ )
  , end_cell_iterator( end_cell_iterator_ )
  {}

  static LogIteratorAdapter make_begin( CellIterator begin, CellIterator end ) {
    auto res = LogIteratorAdapter{
      begin->running_logs.template GetLog< Event >().begin(),
      begin,
      end
    };
    res.Rectify();
    return res;
  }

  static LogIteratorAdapter make_end( CellIterator end ) {
    // ideally, just use std::prev()
    // but for some reason that's calling operator++ instead of operator--
    // see backtrace at https://pastebin.com/4nGpEcCy
    --end;
    return LogIteratorAdapter{
      end->running_logs.template GetLog< Event >().end(),
      end,
      std::next(end)
    };
  }


public:

  static auto make( CellIterator begin, CellIterator end ) {
    return std::pair{
      make_begin(begin, end), make_end(end)
    };
  }

  using value_type = typename LogIterator::value_type;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;

  LogIteratorAdapter() = default;

  const value_type& operator*() { Rectify(); return *log_iterator; }

  const value_type* operator->() { return &operator*(); }

  LogIteratorAdapter& operator++() {

    ++log_iterator;

    Rectify();

    return *this;
  }

  LogIteratorAdapter operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  bool operator==(const LogIteratorAdapter& other) const {
    return std::tuple{
      cell_iterator,
      log_iterator
    } == std::tuple{
      other.cell_iterator,
      other.log_iterator
    };
  }

  bool operator!=(const LogIteratorAdapter& other) const {
    return !operator==( other );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_LOGITERATORADAPTER_HPP_INCLUDE
