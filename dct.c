//
// Created by Nicola Pierazzo on 08/12/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
#include "iio.h"
#include "utils.h"

void dct_inplace(float *data, int w, int h, int c) {
  int n[] = {h, w};
  fftwf_r2r_kind dct2[] = {FFTW_REDFT10, FFTW_REDFT10};
  fftwf_plan plan = fftwf_plan_many_r2r(2, n, c, data, NULL, c, 1, data, NULL,
                                        c, 1, dct2, FFTW_ESTIMATE);
  fftwf_execute(plan);
  fftwf_destroy_plan(plan);

  // Normalization
  for (int i = 0; i < w * h * c; ++i) {
    data[i] /= 4 * w * h;
  }
}

void idct_inplace(float *data, int w, int h, int c) {
  int n[] = {h, w};
  fftwf_r2r_kind idct2[] = {FFTW_REDFT01, FFTW_REDFT01};
  fftwf_plan plan = fftwf_plan_many_r2r(2, n, c, data, NULL, c, 1, data, NULL,
                                        c, 1, idct2, FFTW_ESTIMATE);
  fftwf_execute(plan);
  fftwf_destroy_plan(plan);
}

int main(int argc, char **argv) {
  int usage = pick_option(&argc, argv, "h", NULL) != NULL;
  int inverse = pick_option(&argc, argv, "i", NULL) != NULL;
  if (usage) {
    fprintf(stderr, "usage: %s [-i] [input [output]]\n", argv[0]);
    return EXIT_SUCCESS;
  }

  int w, h, c;
  char *in = argc > 1 ? argv[1] : "-";
  float *img = iio_read_image_float_vec(in, &w, &h, &c);

  if (inverse) {
    idct_inplace(img, w, h, c);
  } else {
    dct_inplace(img, w, h, c);
  }

  char *out = argc > 2 ? argv[2] : "-";
  iio_save_image_float_vec(out, img, w, h, c);

  return EXIT_SUCCESS;
}
