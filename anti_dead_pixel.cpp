//
// Created by Nicola Pierazzo on 14/12/15.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <array>
#include "Image.hpp"
#include "utils.hpp"

using namespace imgutils;
using std::string;
using std::cerr;
using std::array;
using std::sort;

void anti_dead_pixel(Image& img, int of = 1) {
#pragma omp parallel for
  for (int row = of; row < img.rows() - of; ++row) {
    for (int col = of; col < img.columns() - of; ++col) {
      for (int ch = 0; ch < img.channels(); ++ch) {
        array<float, 8> neighbors;
        neighbors[0] = img.val(col - of, row - of, ch);
        neighbors[1] = img.val(col, row - of, ch);
        neighbors[2] = img.val(col + of, row - of, ch);
        neighbors[3] = img.val(col - of, row, ch);
        neighbors[4] = img.val(col + of, row, ch);
        neighbors[5] = img.val(col - of, row + of, ch);
        neighbors[6] = img.val(col, row + of, ch);
        neighbors[7] = img.val(col + of, row + of, ch);
        sort(neighbors.begin(), neighbors.end());
        if (img.val(col, row, ch) > neighbors[7] * 1.5f ||
            img.val(col, row, ch) * 1.5f < neighbors[0]) {
          /* in-place. Dead pixels are not contiguous, so it works */
          img.val(col, row, ch) = 0.5f * (neighbors[3] + neighbors[4]);
        }
      }
    }
  }
}

int main(int argc, char *argv[]) {
  bool usage = pick_option(&argc, argv, "h", nullptr);
  bool bayer = pick_option(&argc, argv, "b", nullptr);
  if (usage) {
    fprintf(stderr, "usage: %s [-b] [input [output]]\n", argv[0]);
    return EXIT_SUCCESS;
  }

  string filename = argc > 1 ? argv[1] : "-";
  Image img = read_image(filename);

  if (bayer) {
    anti_dead_pixel(img, 2);
  } else {
    anti_dead_pixel(img);
  }

  filename = argc > 2 ? argv[2] : "-";
  save_image(img, filename);

  return EXIT_SUCCESS;
}
