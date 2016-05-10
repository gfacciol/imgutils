//
// Created by Nicola Pierazzo on 27/04/16.
//

#include <utility>
#include <iostream>
#include <cmath>
#include "Image.hpp"
#include "utils.hpp"

using namespace imgutils;
using std::cerr;
using std::endl;
using std::cout;
using std::sqrt;
using std::log10;
using std::min;
using std::max;

double mse(const Image &im1, const Image &im2) {
  double ans = 0.;
  for (auto it1 = im1.begin(), it2 = im2.begin(); it1 != im1.end(); ++it1, ++it2) {
    float v1 = min(max(*it1, 0.f), 255.f);
    float v2 = min(max(*it2, 0.f), 255.f);
    ans += (v1 - v2) * (v1 - v2);
  }
  ans /= im1.samples();
  return ans;
}

double rmse(const Image &im1, const Image &im2) {
  return sqrt(mse(im1, im2));
}

double psnr(const Image &im1, const Image &im2) {
  return -20. * log10(rmse(im1, im2) / 255.);
}

int main(int argc, char** argv) {
  bool usage = static_cast<bool>(pick_option(&argc, argv, "h", nullptr));
  bool show_rmse = static_cast<bool>(pick_option(&argc, argv, "m", nullptr));
  if (usage || argc < 2) {
    cerr << "usage: " << argv[0] << " image1 [image2] [-m]" << endl;
    return EXIT_SUCCESS;
  }

  Image in1 = read_image(argv[1]);
  Image in2 = read_image(argc > 2 ? argv[2] : "-");

  if (in1.shape() != in2.shape() || in1.channels() != in2.channels()) {
    cerr << "The two images should have the same dimension." << endl;
    return EXIT_FAILURE;
  }

  cout << (show_rmse ? rmse(in1, in2) : psnr(in1, in2)) << endl;

  return EXIT_SUCCESS;
}
