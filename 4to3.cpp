//
// Created by Nicola Pierazzo on 12/01/16.
//

#include <iostream>
#include "Image.hpp"
#include "utils.hpp"

using namespace imgutils;
using std::string;
using std::cerr;
using std::cout;
using std::endl;

Image to3(const Image& in) {
  Image out(in.rows(), in.columns(), 3);
  for (int row = 0; row < in.rows(); ++row) {
    for (int col = 0; col < in.columns(); ++col) {
      out.val(col, row, 0) = in.val(col, row, 1);
      out.val(col, row, 1) = 0.5f * (in.val(col, row, 0) + in.val(col, row, 3));
      out.val(col, row, 2) = in.val(col, row, 2);
    }
  }
  return out;
}

int main(int argc, char** argv) {
  bool usage = pick_option(&argc, argv, "h", nullptr);
  if (usage) {
    cerr << "usage: " << argv[0] << " [input [output]]" << endl << endl;
    cerr << "The input image has the 4 channels [Gr, R, B, Gb]." << endl;
    return EXIT_SUCCESS;
  }

  Image in = read_image(argc > 1 ? argv[1] : "-");
  if (in.channels() != 4) {
    cerr << "The input image should have 4 channels." << endl;
    return EXIT_FAILURE;
  }

  Image out = to3(in);
  save_image(out, argc > 2 ? argv[2] : "-");

  return EXIT_SUCCESS;
}
