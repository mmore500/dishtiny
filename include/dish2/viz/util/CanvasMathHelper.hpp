#pragma once
#ifndef DISH2_VIZ_UTIL_CANVASMATHHELPER_HPP_INCLUDE
#define DISH2_VIZ_UTIL_CANVASMATHHELPER_HPP_INCLUDE

#include <cmath>

#include "../../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/Canvas.hpp"

#include "../../config/cfg.hpp"
#include "../../config/num_cells_local.hpp"

namespace dish2 {

class CanvasMathHelper {

  const size_t canvas_height;
  const size_t canvas_width;

public:

  CanvasMathHelper(const emp::web::Canvas& canvas)
  : canvas_height( canvas.GetHeight() )
  , canvas_width( canvas.GetWidth() )
  { emp_assert( canvas_height > 0 ); emp_assert( canvas_width > 0 );}

  size_t GetGridWidth() const { return std::sqrt( dish2::num_cells_local() ); }

  size_t GetGridHeight() const { return std::sqrt( dish2::num_cells_local() ); }

  size_t GetCellWidthPx() const {
    emp_assert( GetGridWidth() >= 0 );
    return canvas_width / GetGridWidth();
  }

  size_t GetCellHeightPx() const {
    emp_assert( GetGridHeight() >= 0 );
    return canvas_height / GetGridHeight();
  }

  size_t GetCanvasX(const double grid_x) const {

    emp_assert( GetGridWidth() <= canvas_width );
    const size_t offset_x = (
      canvas_width - GetCellWidthPx() * GetGridWidth()
    ) / 2;

    return grid_x * GetCellWidthPx() + offset_x;

  }

  size_t GetCanvasY(const double grid_y) const {

    emp_assert( GetGridHeight() <= canvas_height );
    const size_t offset_y = (
      canvas_height - GetCellHeightPx() * GetGridHeight()
    ) / 2;

    return grid_y * GetCellHeightPx() + offset_y;

  }

  size_t GetGridX(const size_t pos) const { return pos % GetGridWidth(); }

  size_t GetGridY(const size_t pos) const { return pos / GetGridHeight(); }

  size_t GetPopulationPos(const int x, const int y) const {

    const int h = uitsl::audit_cast<int>( GetGridWidth() );
    const int w = uitsl::audit_cast<int>( GetGridHeight() );

    return emp::Mod(y, h) * w + emp::Mod(x, w);

  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_UTIL_CANVASMATHHELPER_HPP_INCLUDE
