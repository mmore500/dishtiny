#pragma once

#include "uitsl/debug/benchmark_utils.hpp"

#include "uit/ducts/Duct.hpp"

#include "grid_utils.hpp"
#include "State.hpp"
#include "Tile.hpp"

using grid_t = std::vector<Tile>;
using handle_t = grid_t::iterator;
using chunk_t = std::vector<handle_t>;

chunk_t make_chunk(handle_t begin, handle_t end, const size_t num_chunks) {

  chunk_t res(std::distance(begin, end));
  std::iota(
    std::begin(res),
    std::end(res),
    begin
  );

  return res;

}

chunk_t checkout_chunk(chunk_t & source) {

  auto copy = new grid_t;

  std::transform(
    std::begin(source),
    std::end(source),
    std::back_inserter(*copy),
    [](const auto & handle){ return *handle; }
  );

  chunk_t res(copy->size());
  std::iota(
    std::begin(res),
    std::end(res),
    std::begin(*copy)
  );

  return res;

}

void checkin_chunk(chunk_t & source, const chunk_t & checkedout) {

  emp_assert(source.size() == checkedout.size());

  for (size_t i = 0; i < source.size(); ++i) {
    *source[i] = *checkedout[i];
  }
}


emp::vector<chunk_t> make_chunks(grid_t & grid, const size_t num_chunks) {

  const size_t chunk_width = grid.size() / num_chunks;

  assert(grid.size() % num_chunks == 0);

  emp::vector<chunk_t> res;

  for (
    auto [chunk_begin, chunk_end] = std::tuple{std::begin(grid), handle_t{}};
    chunk_begin != std::end(grid);
    chunk_begin = chunk_end
  ) {

    chunk_end = (
      uitsl::safe_less(std::distance(chunk_end, std::end(grid)), chunk_width)
      ? std::end(grid)
      : std::next(chunk_begin, chunk_width)
    );

    res.push_back(
      make_chunk(chunk_begin, chunk_end, num_chunks)
    );

  }

  assert(res.size() == num_chunks);

  return res;
}

void update_chunk(
  chunk_t & chunk,
  const bool verbose,
  const bool shuffle,
  const size_t resistance
) {


  if (shuffle) std::random_shuffle(
    std::begin(chunk),
    std::end(chunk)
  );

  for (auto & handle : chunk) {

    uitsl::do_compute_work(resistance);

    if (verbose) std::cout << handle->GetState();
    handle->Update();
  }
  if (verbose) std::cout << '\n';


}
