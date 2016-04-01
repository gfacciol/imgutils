//
// Created by Nicola Pierazzo on 31/03/16.
//

#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "EigenImage.hpp"
#include "utils.hpp"

using namespace imgutils;
using std::cerr;
using std::endl;
using std::numeric_limits;
using std::min;
using std::max;
using Eigen::MatrixXf;
using Eigen::VectorXf;
using Eigen::LDLT;

enum class Interpolation { constant, linear, bilinear };

MatrixXf ComputeRegMatrix(Interpolation level, int r) {
  MatrixXf reg_matrix;
  switch (level) {
    case Interpolation::constant:
      reg_matrix = MatrixXf::Zero(r * r, 1);
      for (int row = 0; row < r; ++row) {
        for (int col = 0; col < r; ++col) {
          reg_matrix(row * r + col, 0) = 1;
        }
      }
      break;
    case Interpolation::linear:
      reg_matrix = MatrixXf::Zero(r * r, 3);
      for (int row = 0; row < r; ++row) {
        for (int col = 0; col < r; ++col) {
          reg_matrix(row * r + col, 0) = 1;
          reg_matrix(row * r + col, 1) = row;
          reg_matrix(row * r + col, 2) = col;
        }
      }
      break;
    case Interpolation::bilinear:
      reg_matrix = MatrixXf::Zero(r * r, 4);
      for (int row = 0; row < r; ++row) {
        for (int col = 0; col < r; ++col) {
          reg_matrix(row * r + col, 0) = 1;
          reg_matrix(row * r + col, 1) = row;
          reg_matrix(row * r + col, 2) = col;
          reg_matrix(row * r + col, 3) = row * col;
        }
      }
      break;
  }
  return reg_matrix;
}

Image ExtractPatch(const Image& source, int r, int pr, int pc, int ch) {
  Image output(r, r);
  for (int row = 0; row < r; ++row) {
    for (int col = 0; col < r; ++col) {
        output.val(col, row) = source.val(pc + col, pr + row, ch);
    }
  }
  return output;
}

Image smooth_detector(const Image& source, Interpolation level, int r) {
  Image output(source.rows(), source.columns(), 1, numeric_limits<float>::max());
  MatrixXf reg_matrix = ComputeRegMatrix(level, r);
  LDLT<MatrixXf> solver = (reg_matrix.transpose() * reg_matrix).ldlt();
  for (int pr = 0; pr <= source.rows() - r; ++pr) {
    for (int pc = 0; pc <= source.columns() - r; ++pc) {
      VectorXf dist = VectorXf::Zero(r * r);
      for (int ch = 0; ch < source.channels(); ++ch) {
        EigenImage y = ExtractPatch(source, r, pr, pc, ch);
        VectorXf reg_surf = solver.solve(reg_matrix.transpose() * y.asvector());
        dist += (reg_matrix * reg_surf - y.asvector()).cwiseAbs2();
      }
      dist = dist.cwiseSqrt();
      for (int row = pr; row < min(output.rows(), pr + r); ++row) {
        for (int col = pc; col < min(output.columns(), pc + r); ++col) {
          output.val(col, row) = min(output.val(col, row), dist((row - pr) * r + col - pc));
        }
      }
    }
  }
  return output;
}

int main(int argc, char *argv[]) {
  bool usage = pick_option(&argc, argv, "h", nullptr);
  int level = atoi(pick_option(&argc, argv, "l", "1"));
  int r = atoi(pick_option(&argc, argv, "r", "15"));
  if (usage) {
    cerr << "usage: " << argv[0] << " [input [output]] [-l level] [-r r]" << endl;
    return EXIT_SUCCESS;
  }

  Interpolation interp;
  switch (level) {
    case 0:
      interp = Interpolation::constant;
      break;
    case 1:
      interp = Interpolation::linear;
      break;
    case 2:
      interp = Interpolation::bilinear;
      break;
    default:
      cerr << "Wrong interpolation level." << endl;
      return EXIT_FAILURE;
  }

  Image input = read_image(argc > 1 ? argv[1] : "-");
  Image output = smooth_detector(input, interp, r);

  save_image(output, argc > 2 ? argv[2] : "-");
}
