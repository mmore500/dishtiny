#ifndef DIST_H
#define DIST_H

#include <stdlib.h>
#include <tuple>

#include "base/vector.h"

inline size_t calc_1d_tor_dist(size_t z1, size_t z2, size_t dim) {
  // absolute value
  size_t dz = z2 > z1 ? z2 - z1 : z1 - z2;
  if (dz > dim / 2) dz = dim - dz;
  return dz;
}

size_t calc_1d_centroid(emp::vector<size_t> *zs) {
  size_t sum = 0;
  for (size_t i = 0; i < (*zs).size(); i++) {
    sum += (*zs)[i];
  }
  return sum / (*zs).size();
}

inline size_t eval_1d_centroid(size_t cz, emp::vector<size_t> *zs, size_t dim) {
  size_t sum = 0;
  for (size_t i = 0; i < (*zs).size(); i++) {
    sum += calc_1d_tor_dist((*zs)[i], cz, dim);
  }
  return sum;
}


inline size_t calc_taxi_dist(size_t x1, size_t y1, size_t x2, size_t y2) {
  return calc_1d_tor_dist(x1, x2, GRID_W) + calc_1d_tor_dist(y1, y2, GRID_H);
}

std::tuple<size_t, size_t> calc_centroid(emp::vector<size_t> *pts, Grid<int> *gr) {
  emp::vector<size_t> xs_plain;
  emp::vector<size_t> xs_shift;
  emp::vector<size_t> ys_plain;
  emp::vector<size_t> ys_shift;

  for (size_t i = 0; i < (*pts).size(); i++) {
    size_t id = (*pts)[i];
    size_t x = gr->GetX(id);
    size_t y = gr->GetY(id);

    xs_plain.push_back(x);
    xs_shift.push_back((x+GRID_W/2) % GRID_W);
    ys_plain.push_back(y);
    ys_shift.push_back((y+GRID_H/2) % GRID_H);
  }

  size_t xc_plain = calc_1d_centroid(&xs_plain);
  // roll centroid back to original coordinate space
  size_t xc_shift = mod(calc_1d_centroid(&xs_shift) - GRID_W/2, GRID_W);
  size_t yc_plain = calc_1d_centroid(&ys_plain);
  // roll centroid back to original coordinate space
  size_t yc_shift = mod(calc_1d_centroid(&ys_shift) - GRID_H/2, GRID_H);

  bool test;
  // second use of xs_plain here on purpose
  test = (eval_1d_centroid(xc_plain, &xs_plain, GRID_W) < eval_1d_centroid(xc_shift, &xs_plain, GRID_W));
  size_t xc_actual = test ? xc_plain : xc_shift;

  // second use of ys_plain here on purpose
  test = (eval_1d_centroid(yc_plain, &ys_plain, GRID_H) < eval_1d_centroid(yc_shift, &ys_plain, GRID_H));
  size_t yc_actual = test ? yc_plain : yc_shift;

  return std::make_tuple(xc_actual, yc_actual);

}

#endif
