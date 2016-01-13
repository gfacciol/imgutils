#include <iostream>
#include <fstream>
#include "Image.hpp"
#include "utils.hpp"

using namespace imgutils;
using std::string;
using std::cerr;
using std::ifstream;

int main(int argc, char *argv[]) {
  bool usage = pick_option(&argc, argv, "h", nullptr);
  if (usage || argc < 2) {
    fprintf(stderr, "Usage: %s input [output]\n", argv[0]);
    return EXIT_SUCCESS;
  }

  ifstream input(argv[1], ifstream::binary);
  int w, h, c;
  input.read(reinterpret_cast<char*>(&w), sizeof w);
  input.read(reinterpret_cast<char*>(&h), sizeof h);
  input.read(reinterpret_cast<char*>(&c), sizeof c);

  Image im(h, w, c);
  input.read(reinterpret_cast<char*>(im.data()), w * h * c * sizeof(float));

  string filename = argc > 2 ? argv[2] : "-";
  save_image(im, filename);

  return EXIT_SUCCESS;
}
