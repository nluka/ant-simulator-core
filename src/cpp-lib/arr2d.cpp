#include "arr2d.hpp"

// constexpr
size_t arr2d::get_1d_idx(
  size_t const arrWidth,
  size_t const targetCol,
  size_t const targetRow
) {
  return (targetRow * arrWidth) + targetCol;
}
