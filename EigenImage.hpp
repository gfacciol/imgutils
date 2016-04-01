/*
 * IioImage.h
 *
 *  Created on: 10/feb/2015
 *      Author: nicola
 */

#ifndef EIGENIMAGE_H_
#define EIGENIMAGE_H_

#include "Image.hpp"
#include <Eigen/Core>

namespace imgutils {

class EigenImage : public Image {
 public:
  EigenImage(int rows, int columns, int channels = 1);

  EigenImage(const EigenImage &) = delete;
  EigenImage &operator=(const EigenImage &) = delete;

  EigenImage(EigenImage &&) = default;
  EigenImage &operator=(EigenImage &&) = default;

  EigenImage(Image &&from) : Image(std::move(from)), vecmap_(data(), samples()) {};

  Eigen::Map<Eigen::VectorXf> asvector() const { return vecmap_; };

 private:
  Eigen::Map<Eigen::VectorXf> vecmap_;
};

inline EigenImage::EigenImage(int rows, int columns, int channels)
    : Image(rows, columns, channels), vecmap_(data(), samples()) {}

} /* namespace imgutils */

#endif /* EIGENIMAGE_H_ */
