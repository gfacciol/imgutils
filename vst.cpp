//
// Created by Nicola Pierazzo on 03/02/16.
//

#include <iostream>
#include <cmath>
#include <algorithm>
#include "Image.hpp"
#include "utils.hpp"

using namespace imgutils;
using std::string;
using std::cerr;
using std::endl;
using std::max;

int main(int argc, char *argv[]) {
  bool usage = pick_option(&argc, argv, "h", nullptr);
  bool inverse = pick_option(&argc, argv, "i", nullptr);
  if (usage || argc < 3) {
    cerr << "usage: " << argv[0] << " a b [-i] [input [output]]" << endl;
    cerr << "Applies the (inverse) VST when the image has the noise model sqrt(ax + b)" << endl;
    return EXIT_SUCCESS;
  }

  float a = atof(argv[1]);
  float b = atof(argv[2]);

  Image im = read_image(argc > 3 ? argv[3] : "-");

  if (inverse) {
    for (float& x : im) x = (a / 4.f) * x * x - (b / a);
  } else {
    for (float& x : im) x = (2.f / a) * sqrt(max(0.f, a * x + b));
  }

  save_image(im, argc > 4 ? argv[4] : "-");

  return EXIT_SUCCESS;
}
