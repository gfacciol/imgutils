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
using std::distance;
using std::lower_bound;
using std::vector;
using std::sort;

void apply_histogram(const Image& sample, Image& im) {
  for (int c = 0; c < im.channels(); ++c) {
    vector<float> oldvalues, newvalues;
    for (int row = 0; row < im.rows(); ++row) {
      for (int col = 0; col < im.columns(); ++col) {
        oldvalues.push_back(im.val(col, row, c));
        newvalues.push_back(sample.val(col, row, c));
      }
    }
    sort(oldvalues.begin(), oldvalues.end());
    sort(newvalues.begin(), newvalues.end());
    for (int row = 0; row < im.rows(); ++row) {
      for (int col = 0; col < im.columns(); ++col) {
        int rank = distance(oldvalues.begin(), lower_bound(oldvalues.begin(), oldvalues.end(), im.val(col, row, c)));
        rank = rank * (newvalues.size() - 1) / (oldvalues.size() - 1);
        im.val(col, row, c) = newvalues[rank];
      }
    }
  }
}

int main(int argc, char *argv[]) {
  bool usage = pick_option(&argc, argv, "h", nullptr);
  if (usage || argc < 2) {
    cerr << "usage: " << argv[0] << " sample [input [output]]" << endl;
    cerr << "Applies the histogram of sample in input (sample and input must have the same size)" << endl;
    return EXIT_SUCCESS;
  }

  Image sample = read_image(argv[1]);
  Image im = read_image(argc > 2 ? argv[2] : "-");

  if (sample.channels() != im.channels()) {
    cerr << "ERROR: sample and input must have the same number of channels" << endl;
    return EXIT_FAILURE;
  }

  apply_histogram(sample, im);
  save_image(im, argc > 3 ? argv[3] : "-");

  return EXIT_SUCCESS;
}
