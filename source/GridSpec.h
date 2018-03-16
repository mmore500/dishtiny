#pragma once

#include <stdlib.h>
#include <tuple>

#include "base/vector.h"

#include "DishtinyConfig.h"

class GridSpec {
public:
  const size_t height;
  const size_t width;
  const size_t area;

  GridSpec(DishtinyConfig& config)
  : height(config.GRID_H())
  , width(config.GRID_W())
  , area(config.GRID_H() * config.GRID_W())
  { ; }

  GridSpec(const GridSpec &) = default;

  inline size_t GetWidth() const { return width; }
  inline size_t GetHeight() const { return height; }
  inline size_t GetArea() const { return area; }

  inline size_t GetX(size_t id) const { return id % width; }
  inline size_t GetY(size_t id) const { return id / width; }
  inline size_t GetID(int x, int y) const {
    return mod(y, height) * width + mod(x, width);
  }

  inline size_t CalcTaxiDist(size_t cell1, size_t cell2) const {
    return (
      calc_1d_tor_dist(GetX(cell1), GetX(cell2), width) +
      calc_1d_tor_dist(GetY(cell1), GetY(cell2), height)
    );
  }

  inline size_t CalcTaxiDist(size_t cell1, size_t x2, size_t y2) const {
    return (
      calc_1d_tor_dist(GetX(cell1), x2, width) +
      calc_1d_tor_dist(GetY(cell1), y2, height)
    );
  }

  inline size_t CalcTaxiDist(size_t x1, size_t y1, size_t x2, size_t y2) const {
    return (
      calc_1d_tor_dist(x1, x2, width) +
      calc_1d_tor_dist(y1, y2, height)
    );
  }

  inline std::tuple<size_t, size_t> CalcCentroid(emp::vector<size_t> &pts) const {
    emp::vector<size_t> xs_plain;
    emp::vector<size_t> xs_shift;
    emp::vector<size_t> ys_plain;
    emp::vector<size_t> ys_shift;

    for (size_t i = 0; i < pts.size(); i++) {
      size_t id = pts[i];
      size_t x = GetX(id);
      size_t y = GetY(id);

      xs_plain.push_back(x);
      xs_shift.push_back((x+(width/2)) % width);
      ys_plain.push_back(y);
      ys_shift.push_back((y+(height/2)) % height);
    }

    size_t xc_plain = calc_1d_centroid(xs_plain);
    // roll centroid back to original coordinate space
    size_t xc_shift = mod(calc_1d_centroid(xs_shift) - width/2, width);
    size_t yc_plain = calc_1d_centroid(ys_plain);
    // roll centroid back to original coordinate space
    size_t yc_shift = mod(calc_1d_centroid(ys_shift) - height/2, height);

    bool test;
    // second use of xs_plain here on purpose
    test = (
      eval_1d_centroid(xc_plain, xs_plain, width) <
      eval_1d_centroid(xc_shift, xs_plain, width)
    );
    size_t xc_actual = test ? xc_plain : xc_shift;

    // second use of ys_plain here on purpose
    test = (
      eval_1d_centroid(yc_plain, ys_plain, height) <
      eval_1d_centroid(yc_shift, ys_plain, height)
    );
    size_t yc_actual = test ? yc_plain : yc_shift;

    return std::make_tuple(xc_actual, yc_actual);

  }


private:
  inline int mod(int a, int base) const {
    return ((a % base) + base) % base;
  }

  inline size_t calc_1d_tor_dist(size_t z1, size_t z2, size_t dim) const {
    // absolute value
    size_t dz = z2 > z1 ? z2 - z1 : z1 - z2;
    if (dz > dim / 2) dz = dim - dz;
    return dz;
  }

  inline size_t calc_1d_centroid(emp::vector<size_t>& zs) const {
    size_t sum = 0;
    for (size_t i = 0; i < zs.size(); i++) {
      sum += zs[i];
    }
    return sum / zs.size();
  }

  inline size_t eval_1d_centroid(
    size_t cz,
    emp::vector<size_t>& zs,
    size_t dim) const {

    size_t sum = 0;
    for (size_t i = 0; i < zs.size(); i++) {
      sum += calc_1d_tor_dist(zs[i], cz, dim);
    }
    return sum;

  }

};
