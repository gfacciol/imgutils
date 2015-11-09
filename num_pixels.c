//
// Created by Nicola Pierazzo on 09/11/15.
//

#include <stdio.h>
#include <stdlib.h>
#include "iio.h"
#include "utils.h"

int main(int argc, char **argv) {
  int usage = pick_option(&argc, argv, "h", NULL) != NULL;
  if (usage) {
    fprintf(stderr, "usage: %s [input]\n", argv[0]);
    return EXIT_SUCCESS;
  }

  int w, h, c;
  char *in = argc > 1 ? argv[1] : "-";
  free(iio_read_image_float_vec(in, &w, &h, &c));

  printf("%d\n", w * h);

  return EXIT_SUCCESS;
}