//
// Created by Nicola Pierazzo on 04/02/16.
//

#include <iostream>
#include "Image.hpp"
#include "utils.hpp"

using namespace imgutils;
using std::cerr;
using std::endl;

Image colorizeCfa(const Image& in, int phase) {
  int rr = 1 - (phase / 2);
  int cr = phase % 2;
  Image result(in.rows(), in.columns(), 3);
  for (int row = 0; row < in.rows(); ++row) {
    for (int col = 0; col < in.columns(); ++col) {
      int channel = (row + rr) % 2 + (col + cr) % 2;
      result.val(col, row, channel) = in.val(col, row);
    }
  }
  return result;
}

int main(int argc, char** argv) {
  bool usage = pick_option(&argc, argv, "h", nullptr);
  int phase = atoi(pick_option(&argc, argv, "p", "2"));
  if (usage || argc < 3) {
    cerr << "usage: " << argv[0] << " [-p phase] [input [output]]" << endl;
    return EXIT_SUCCESS;
  }

  Image in = read_image(argc > 1 ? argv[1] : "-");
  if (in.channels() != 1) {
    cerr << "The input images should have one channel." << endl;
    return EXIT_FAILURE;
  }

  Image out = colorizeCfa(in, phase);
  save_image(out, argc > 2 ? argv[2] : "-");

  return EXIT_SUCCESS;
}
