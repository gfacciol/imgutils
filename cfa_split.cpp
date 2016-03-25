//
// Created by Nicola Pierazzo on 03/02/16.
//

#include <iostream>
#include "Image.hpp"
#include "utils.hpp"

using namespace imgutils;
using std::cerr;
using std::endl;

Image RGrB(const Image& in, int phase) {
  int rb = phase / 2;
  int rr = 1 - rb;
  int cr = phase % 2;
  int cb = 1 - cr;
  Image result(in.rows() / 2, in.columns() / 2, 3);
  for (int row = 0; row < result.rows(); ++row) {
    for (int col = 0; col < result.columns(); ++col) {
      result.val(col, row, 0) = in.val(2 * col + cr, 2 * row + rr);  // R
      result.val(col, row, 1) = in.val(2 * col + cb, 2 * row + rr);  // Gr
      result.val(col, row, 2) = in.val(2 * col + cb, 2 * row + rb);  // B
    }
  }
  return result;
}

Image RGbB(const Image& in, int phase) {
  int rb = phase / 2;
  int rr = 1 - rb;
  int cr = phase % 2;
  int cb = 1 - cr;
  Image result(in.rows() / 2, in.columns() / 2, 3);
  for (int row = 0; row < result.rows(); ++row) {
    for (int col = 0; col < result.columns(); ++col) {
      result.val(col, row, 0) = in.val(2 * col + cr, 2 * row + rr);  // R
      result.val(col, row, 1) = in.val(2 * col + cr, 2 * row + rb);  // Gb
      result.val(col, row, 2) = in.val(2 * col + cb, 2 * row + rb);  // B
    }
  }
  return result;
}

int main(int argc, char** argv) {
  bool usage = pick_option(&argc, argv, "h", nullptr);
  int phase = atoi(pick_option(&argc, argv, "p", "2"));
  if (usage || argc < 3) {
    cerr << "usage: " << argv[0] << " [-p phase] [input] output1 output2" << endl;
    cerr << "The input image has the 4 channels [Gr, R, B, Gb]." << endl;
    return EXIT_SUCCESS;
  }

  Image in = read_image(argc > 3 ? argv[1] : "-");
  if (in.channels() != 1) {
    cerr << "The input image should have one channel." << endl;
    return EXIT_FAILURE;
  }

  Image out1 = RGrB(in, phase);
  Image out2 = RGbB(in, phase);
  save_image(out1, argc > 3 ? argv[2] : argv[1]);
  save_image(out2, argc > 3 ? argv[3] : argv[2]);

  return EXIT_SUCCESS;
}
