#include <stdio.h> // only for "fprintf"
#include <stdlib.h> // only for "free"
#include <stdint.h>
extern "C" {
#include "iio.h"
}

// read an image in any format from STDIN and write a ppm to STDOUT
int main(int c, char *v[])
{
	if (c != 3) {
		fprintf(stderr, "usage:\n\t%s infile outfile\n", *v);
		return 1;
	}
	int w, h, pd;
	//uint16_t *x = iio_read_image_uint16_vec(v[1], &w, &h, &pixeldim);
	float *x = iio_read_image_float_vec(v[1], &w, &h, &pd);
	if (!x) {
		fprintf(stderr, "failed to read an image from file "
				"\"%s\"\n", v[1]);
		return 1;
	}
	//fprintf(stderr, "got a %dx%d image with %d channels\n", w, h, pd);
	//for (int i = 0; i < pd; i++)
	//	fprintf(stderr, "p0: %g\n", (float)x[i]);
	//iio_save_image_uint16_vec(v[2], x, w, h, pd);
	iio_save_image_float_vec(v[2], x, w, h, pd);
	free(x);
	return 0;
}
