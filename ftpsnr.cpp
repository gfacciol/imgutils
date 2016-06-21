//
// Created by Nicola Pierazzo on 01/04/16.
//

#include <iostream>
#include <cmath>
#include <utility>
#include <tuple>
#include "Image.hpp"
#include "utils.hpp"

using namespace imgutils;
using std::cerr;
using std::endl;
using std::cout;
using std::tuple;
using std::pair;
using std::tie;
using std::max;
using std::min;

// flat patches count "weight", while other patches count 1
tuple<double, double, double> compute_ftrmse(const Image& mask,
                                             const Image& in1,
                                             const Image& in2,
                                             float t1, float t2) {
  pair <double, double> flat = {0., 0.}, texture = {0., 0.};
  double mse = 0.;
  for (int row = 0; row < in1.rows(); ++row) {
    for (int col = 0; col < in1.columns(); ++col) {
      for (int chan = 0; chan < in1.channels(); ++chan) {
        float v1 = min(max(in1.val(col, row, chan), 0.f), 255.f);
        float v2 = min(max(in2.val(col, row, chan), 0.f), 255.f);
        double d = (v1 - v2) * (v1 - v2);
        float m = mask.val(col, row);
        double f, t;
        if (m <= t1) {
          f = 1.;
          t = 0.;
        } else if (m < t2) {
          f = (t2 - m) / (t2 - t1);
          t = (m - t1) / (t2 - t1);
        } else {
          f = 0.;
          t = 1.;
        }
        flat.first += d * f;
        flat.second += f;
        texture.first += d * t;
        texture.second += t;
        mse += d;
      }
    }
  }
  return {sqrt(flat.first / flat.second),
          sqrt(texture.first / texture.second),
          sqrt(mse / in1.samples())};
}

int main(int argc, char** argv) {
  bool usage = pick_option(&argc, argv, "h", nullptr);
  float t1 = atof(pick_option(&argc, argv, "t1", "1"));
  float t2 = atof(pick_option(&argc, argv, "t2", "2"));
  if (usage || argc < 3) {
    cerr << "usage: " << argv[0] << " mask image1 [image2] [-t1 threshold] " <<
        "[-t2 threshold]" << endl;
    cerr << "       when the mask is < t1 (" << t1 << ") the zone is flat" << endl;
    cerr << "       when the mask is > t2 (" << t2 << ") the zone is textured" << endl;
    return EXIT_SUCCESS;
  }

  Image mask = read_image(argv[1]);
  Image in1 = read_image(argv[2]);
  Image in2 = read_image(argc > 3 ? argv[3] : "-");

  if (mask.channels() != 1) {
    cerr << "The input images should have one channel." << endl;
    return EXIT_FAILURE;
  }

  if (t1 > t2) {
    cerr << "t1 should be smaller than t2." << endl;
    return EXIT_FAILURE;
  }

  if (in1.rows() != in2.rows() ||
      in1.columns() != in2.columns() ||
      in1.channels() != in2.channels()) {
    cerr << "The two images should have the same dimension." << endl;
    return EXIT_FAILURE;
  }

  double frmse, trmse, rmse;
  tie(frmse, trmse, rmse) = compute_ftrmse(mask, in1, in2, t1, t2);
  double fpsnr = -20. * log10(frmse / 255.);
  double tpsnr = -20. * log10(trmse / 255.);
  double psnr = -20. * log10(rmse / 255.);

  cout << "fPSNR: " << fpsnr << endl;
  cout << "tPSNR: " << tpsnr << endl;
  cout << "PSNR: " << psnr << endl;

  return EXIT_SUCCESS;
}
