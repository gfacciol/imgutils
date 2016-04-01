//
// Created by Nicola Pierazzo on 21/10/15.
//

#ifndef IMGUTILS_UTILS_H
#define IMGUTILS_UTILS_H

#include "Image.hpp"
#include <string>

namespace imgutils {

const char *pick_option(int *c, char **v, const char *o, const char *d);
Image read_image(const std::string& filename);
void save_image(const Image& image, const std::string& filename);

Image pad_symmetric(const Image &src, int padding);
}

#endif //IMGUTILS_UTILS_H
