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




int main(int argc, char** argv) {
  bool usage = static_cast<bool>(pick_option(&argc, argv, "h", nullptr));
  if (usage || argc < 2) {
    cerr << "usage: " << argv[0] << " image1 [[image2] diff]" << endl;
    return EXIT_SUCCESS;
  }

  Image im1 = read_image(argv[1]);
  Image im2 = read_image(argc > 2 ? argv[2] : "-");
  char* oimg= argc > 3 ? argv[3] : (char*) "-";

  if (im1.shape() != im2.shape() || im1.channels() != im2.channels()) {
    cerr << "The two images should have the same dimension." << endl;
    return EXIT_FAILURE;
  }

  Image dif = im1.copy();

  for (auto it1 = im1.begin(), it2 = im2.begin(), itd = dif.begin(); it1 != im1.end(); ++it1, ++it2, ++itd) {
    *itd = 2 * (*it1 - *it2) + 128;
  }

  save_image(dif, oimg);

  return EXIT_SUCCESS;
}
