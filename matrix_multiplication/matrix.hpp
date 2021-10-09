//
// Created by polikutin on 09/10/2021.
//

#ifndef MATRIX_MULTIPLICATION__MATRIX_HPP_
#define MATRIX_MULTIPLICATION__MATRIX_HPP_

#include <vector>
#include "vector.hpp"

class Matrix {
 public:
  Matrix(std::size_t height, std::size_t width);
  Matrix(std::size_t height, std::size_t width, double value);
  Matrix operator*(const Matrix& other) const;
  Vector operator*(const Vector& other) const;
  double& operator()(std::size_t row, std::size_t col);
  const double& operator()(std::size_t row, std::size_t col) const;
 private:
  std::size_t width_;
 public:
  std::size_t GetWidth() const;
  std::size_t GetHeight() const;
 private:
  std::size_t height_;
  std::vector<double> data_;
};

#endif //MATRIX_MULTIPLICATION__MATRIX_HPP_
