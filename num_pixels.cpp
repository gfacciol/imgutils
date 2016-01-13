//
// Created by Nicola Pierazzo on 09/11/15.
//

#include <iostream>
#include "Image.hpp"
#include "utils.hpp"

using namespace imgutils;
using std::string;
using std::cerr;
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
  bool usage = pick_option(&argc, argv, "h", nullptr);
  if (usage) {
    cerr << "usage: " << argv[0] << " [input]" << endl;
    return EXIT_SUCCESS;
  }

  string filename = argc > 1 ? argv[1] : "-";
  Image im = read_image(filename);
  cout << im.rows() * im.columns() << endl;

  return EXIT_SUCCESS;
}
