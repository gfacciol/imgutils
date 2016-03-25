//
// Created by Nicola Pierazzo on 03/02/16.
//

#include <iostream>
#include "Image.hpp"
#include "utils.hpp"

using namespace imgutils;
using std::cerr;
using std::endl;

Image rebuildCfa(const Image& in1, const Image& in2, int phase) {
  int rb = phase / 2;
  int rr = 1 - rb;
  int cr = phase % 2;
  int cb = 1 - cr;
  Image result(in1.rows() * 2, in1.columns() * 2);
  for (int row = 0; row < in1.rows(); ++row) {
    for (int col = 0; col < in1.columns(); ++col) {
      result.val(2 * col + cb, 2 * row + rr) = in1.val(col, row, 1);                                  // Gr
      result.val(2 * col + cr, 2 * row + rr) = 0.5f * (in1.val(col, row, 0) + in2.val(col, row, 0));  // R
      result.val(2 * col + cb, 2 * row + rb) = 0.5f * (in1.val(col, row, 2) + in2.val(col, row, 2));  // B
      result.val(2 * col + cr, 2 * row + rb) = in2.val(col, row, 1);                                  // Gb
    }
  }
  return result;
}

int main(int argc, char** argv) {
  bool usage = pick_option(&argc, argv, "h", nullptr);
  int phase = atoi(pick_option(&argc, argv, "p", "2"));
  if (usage || argc < 3) {
    cerr << "usage: " << argv[0] << " [-p phase] input1 input2 [output]" << endl;
    cerr << "The input image has the 4 channels [Gr, R, B, Gb]." << endl;
    return EXIT_SUCCESS;
  }

  Image in1 = read_image(argv[1]);
  Image in2 = read_image(argv[2]);
  if (in1.channels() != 3 || in2.channels() != 3) {
    cerr << "The input images should have three channels." << endl;
    return EXIT_FAILURE;
  }
  if (in1.rows() != in2.rows() || in1.columns() !=  in2.columns()) {
    cerr << "The input images should have the same size" << endl;
    return EXIT_FAILURE;
  }

  Image out = rebuildCfa(in1, in2, phase);
  save_image(out, argc > 3 ? argv[3] : "-");

  return EXIT_SUCCESS;
}
