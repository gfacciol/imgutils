//
// Created by Nicola Pierazzo on 01/04/16.
//

#include <iostream>
#include <cmath>
#include "Image.hpp"
#include "utils.hpp"

using namespace imgutils;
using std::cerr;
using std::endl;
using std::cout;

// flat patches count "weight", while other patches count 1
double compute_frmse(const Image& mask, const Image& in1, const Image& in2, float weight) {
  double ans = 0.f;
  double sum = 0.f;
  for (int row = 0; row < in1.rows(); ++row) {
    for (int col = 0; col < in1.columns(); ++col) {
      for (int chan = 0; chan < in1.channels(); ++chan) {
        double d = in1.val(col, row, chan) - in2.val(col, row, chan);
        double w = ((weight - 1.) * mask.val(col, row) + 1.);
        ans += d * d * w;
        sum += w;
      }
    }
  }
  return sqrt(ans / sum);
}

int main(int argc, char** argv) {
  bool usage = pick_option(&argc, argv, "h", nullptr);
  bool rmse = pick_option(&argc, argv, "m", nullptr);
  float weight = atof(pick_option(&argc, argv, "w", "10"));
  if (usage || argc < 3) {
    cerr << "usage: " << argv[0] << " mask image1 [image2] [-w weight] [-m]" << endl;
    return EXIT_SUCCESS;
  }

  Image mask = read_image(argv[1]);
  Image in1 = read_image(argv[2]);
  Image in2 = read_image(argc > 3 ? argv[3] : "-");

  if (mask.channels() != 1) {
    cerr << "The input images should have one channel." << endl;
    return EXIT_FAILURE;
  }

  if (in1.rows() != in2.rows() || in1.columns() != in2.columns() || in1.channels() != in2.channels()) {
    cerr << "The two images should have the same dimension." << endl;
    return EXIT_FAILURE;
  }

  double frmse = compute_frmse(mask, in1, in2, weight);
  double fpsnr = -20. * log10(frmse / 255.);

  cout << (rmse ? frmse : fpsnr) << endl;

  return EXIT_SUCCESS;
}
