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
  if (im.channels() == 1) {
      cout << 1 << endl;
      return EXIT_SUCCESS;
  }
  for(int j=0; j<im.rows(); j++)
  for(int i=0; i<im.columns(); i++)
  for(int k=1; k<im.channels(); k++)
     if (im.val(i,j,0) != im.val(i,j,k)) {
         cout << 0 << endl;
         return EXIT_SUCCESS;
     }
  cout << 1 << endl;
  return EXIT_SUCCESS;
}
