//
// Created by Nicola Pierazzo on 21/10/15.
//

#include <cstdlib>
#include <cstring>
#include "utils.hpp"

extern "C" {
#include "iio.h"
}

using std::string;
using std::free;
using std::strcmp;

namespace imgutils {

const char *pick_option(int *c, char **v, const char *o, const char *d) {
  int id = d ? 1 : 0;
  for (int i = 0; i < *c - id; i++) {
    if (v[i][0] == '-' && 0 == strcmp(v[i] + 1, o)) {
      char *r = v[i + id] + 1 - id;
      for (int j = i; j < *c - id; j++)
        v[j] = v[j + id + 1];
      *c -= id + 1;
      return r;
    }
  }
  return d;
}

Image read_image(const string &filename) {
  int w, h, c;
  float *data = iio_read_image_float_vec(filename.c_str(), &w, &h, &c);
  Image im(data, h, w, c);
  free(data);
  return im;
}

void save_image(const Image &image, const string &filename) {
  iio_save_image_float_vec(const_cast<char *>(filename.c_str()),
                           const_cast<float *>(image.data()),
                           image.columns(),
                           image.rows(),
                           image.channels());
}

inline int SymmetricCoordinate(int pos, int size) {
  if (pos < 0) pos = -pos - 1;
  if (pos >= 2 * size) pos %= 2 * size;
  if (pos >= size) pos = 2 * size - 1 - pos;
  return pos;
}

Image pad_symmetric(const Image &src, int padding) {
  Image result(src.rows() + 2 * padding,
               src.columns() + 2 * padding,
               src.channels());
  for (int row = 0; row < result.rows(); ++row) {
    for (int col = 0; col < result.columns(); ++col) {
      for (int chan = 0; chan < result.channels(); ++chan) {
        result.val(col, row, chan) =
            src.val(SymmetricCoordinate(col - padding, src.columns()),
                    SymmetricCoordinate(row - padding, src.rows()),
                    chan);
      }
    }
  }
  return result;
}

bool isMonochrome (const Image &u) {
  for (int row = 0; row < u.rows(); ++row) {
    for (int col = 0; col < u.columns(); ++col) {
      float v = u.val(col, row, 0);
      for (int ch = 1; ch < u.channels(); ++ch) {
        if (u.val(col, row, ch) != v)  {
           return false;
        }
      }
    }
  }
  return true;
}

Image makeMonochrome (const Image &u) {
  Image result(u.rows(), u.columns());
  for (int row = 0; row < u.rows(); ++row) {
    for (int col = 0; col < u.columns(); ++col) {
      double v = 0;
      for (int ch = 0; ch < u.channels(); ++ch) {
        v += u.val(col, row, ch);
      }
      result.val(col,row) = v / u.channels();
    }
  }
  return result;
}

}
