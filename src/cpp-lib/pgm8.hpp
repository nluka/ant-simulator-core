#ifndef PGM8_HPP
#define PGM8_HPP

#include <fstream>
#include <cinttypes>
#include "arr2d.hpp"

namespace pgm8 { // stands for `portable gray map 8-bit`

void write_ascii(
  std::ofstream *file,
  uint16_t width,
  uint16_t height,
  uint8_t maxPixelVal,
  uint8_t const *pixels // 1D array
);

void write_bin(
  std::ofstream *file,
  uint16_t width,
  uint16_t height,
  uint8_t maxPixelVal,
  uint8_t const *pixels
);

} // namespace pgm

#endif // PGM8_HPP
