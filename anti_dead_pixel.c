//
// Created by Nicola Pierazzo on 14/12/15.
//

#include <stdio.h>
#include <stdlib.h>
#include "iio.h"
#include "utils.h"

void insertion_sort(float *v, int n) {
  for (int i = 1; i < n; ++i) {
    int j = i;
    while ((j > 0) && v[j-1] > v[j]) {
      float tmp = v[j-1];
      v[j-1] = v[j];
      v[j] = tmp;
      --j;
    }
  }
}

void anti_dead_pixel(float *img, int w, int h, int c, int offset) {
  for (int row = offset; row < h - offset; ++row) {
    for (int col = offset; col < w - offset; ++col) {
      for (int ch = 0; ch < c; ++ch) {
        int pos = row * w * c + col * c + ch;
        float neighbors[8];
        neighbors[0] = img[pos - offset * w * c - offset * c];
        neighbors[1] = img[pos - offset * w * c];
        neighbors[2] = img[pos - offset * w * c + offset * c];
        neighbors[3] = img[pos - offset * c];
        neighbors[4] = img[pos + offset * c];
        neighbors[5] = img[pos + offset * w * c - offset * c];
        neighbors[6] = img[pos + offset * w * c];
        neighbors[7] = img[pos + offset * w * c + offset * c];
        insertion_sort(neighbors, 8);
        if (img[pos] > neighbors[7] * 1.5f || img[pos] * 1.5f < neighbors[0]) {
          /* in-place. not optimal, but dead pixels are not continuous, so it should work */
          img[pos] = 0.5f * (neighbors[3] + neighbors[4]);
        }
      }
    }
  }
}

int main(int argc, char **argv) {
  int usage = pick_option(&argc, argv, "h", NULL) != NULL;
  int bayer = pick_option(&argc, argv, "b", NULL) != NULL;
  if (usage) {
    fprintf(stderr, "usage: %s [-b] [input [output]]\n", argv[0]);
    return EXIT_SUCCESS;
  }

  int w, h, c;
  char *in = argc > 1 ? argv[1] : "-";
  float *img = iio_read_image_float_vec(in, &w, &h, &c);

  if (bayer) {
    anti_dead_pixel(img, w, h, c, 2);
  } else {
    anti_dead_pixel(img, w, h, c, 1);
  }

  char *out = argc > 2 ? argv[2] : "-";
  iio_save_image_float_vec(out, img, w, h, c);

  return EXIT_SUCCESS;
}
