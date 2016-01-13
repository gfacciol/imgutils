//
// Created by Nicola Pierazzo on 08/12/15.
//

#include <fftw3.h>
#include <iostream>
#include "Image.hpp"
#include "utils.hpp"

using namespace imgutils;
using std::string;
using std::cerr;
using std::cout;
using std::endl;

void dct_inplace(Image& img) {
  int n[] = {img.rows(), img.columns()};
  fftwf_r2r_kind dct2[] = {FFTW_REDFT10, FFTW_REDFT10};
  fftwf_plan plan = fftwf_plan_many_r2r(2, n, img.channels(), img.data(), NULL,
                                        img.channels(), 1, img.data(), NULL,
                                        img.channels(), 1, dct2, FFTW_ESTIMATE);
  fftwf_execute(plan);
  fftwf_destroy_plan(plan);

  // Normalization
  for (int i = 0; i < img.rows() * img.columns() * img.channels(); ++i) {
    img.val(i) /= 4 * img.rows() * img.columns();
  }
}

void idct_inplace(Image& img) {
  int n[] = {img.rows(), img.columns()};
  fftwf_r2r_kind idct2[] = {FFTW_REDFT01, FFTW_REDFT01};
  fftwf_plan plan = fftwf_plan_many_r2r(2, n, img.channels(), img.data(), NULL,
                                        img.channels(), 1, img.data(), NULL,
                                        img.channels(), 1, idct2, FFTW_ESTIMATE);
  fftwf_execute(plan);
  fftwf_destroy_plan(plan);
}

int main(int argc, char *argv[]) {
  bool usage = pick_option(&argc, argv, "h", nullptr);
  bool inverse = pick_option(&argc, argv, "i", nullptr);
  if (usage) {
    cerr << "usage: " << argv[0] << " [-i] [input [output]]" << endl;
    return EXIT_SUCCESS;
  }

  string filename = argc > 1 ? argv[1] : "-";
  Image im = read_image(filename);

  if (inverse) {
    idct_inplace(im);
  } else {
    dct_inplace(im);
  }

  filename = argc > 2 ? argv[2] : "-";
  save_image(im, filename);

  return EXIT_SUCCESS;
}
