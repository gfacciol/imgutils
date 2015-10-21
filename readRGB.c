#include <stdio.h>
#include <stdlib.h>
#include "iio.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s input [output]\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  int w, h, c;
  FILE *input = fopen(argv[1], "r");
  fread(&w, sizeof(int), 1, input);
  fread(&h, sizeof(int), 1, input);
  fread(&c, sizeof(int), 1, input);

  float *img = malloc(w * h * c * sizeof(float));
  fread(img, sizeof(float), w * h * c, input);
  fclose(input);

  char *output = argc > 2 ? argv[2] : "-";
  iio_save_image_float_split(output, img, w, h, c);
  free(img);
}
