#include "pgm8.hpp"

static
void write_header(
  char const *const magicNum,
  std::ofstream *const file,
  uint16_t const width,
  uint16_t const height,
  uint8_t const maxPixelVal
) {
  *file
    << magicNum << '\n'
    << width << ' ' << height << '\n'
    << static_cast<int>(maxPixelVal) << '\n';
}

void pgm8::write_ascii(
  std::ofstream *const file,
  uint16_t const width,
  uint16_t const height,
  uint8_t const maxPixelVal,
  uint8_t const *const pixels // 1D array
) {
  write_header("P2", file, width, height, maxPixelVal);

  // pixels
  for (uint16_t r = 0; r < height; ++r) {
    for (uint16_t c = 0; c < width; ++c) {
      *file
        << static_cast<int>(pixels[arr2d::get_1d_idx(width, c, r)])
        << ' ';
    }
    *file << '\n';
  }
}

void pgm8::write_bin(
  std::ofstream *const file,
  uint16_t const width,
  uint16_t const height,
  uint8_t const maxPixelVal,
  uint8_t const *const pixels
) {
  write_header("P5", file, width, height, maxPixelVal);

  // pixels
  file->write(reinterpret_cast<char const *>(pixels), width * height);
}
