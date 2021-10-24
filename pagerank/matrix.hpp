//
// Created by polikutin on 09/10/2021.
//

#ifndef PAGERANK__MATRIX_HPP_
#define PAGERANK__MATRIX_HPP_

#include <vector>
#include "vector.hpp"

class Matrix {
 public:
  Matrix(std::size_t height, std::size_t width);
  Matrix(std::size_t height, std::size_t width, double value);
  Matrix operator*(const Matrix &other) const;
  [[nodiscard]] Matrix BlasMultiply(const Matrix &other) const;
  Vector operator*(const Vector &other) const;
  double &operator()(std::size_t row, std::size_t col);
  const double &operator()(std::size_t row, std::size_t col) const;
  [[nodiscard]] std::size_t GetWidth() const;
  [[nodiscard]] std::size_t GetHeight() const;
  [[nodiscard]] Matrix Pow(std::uint32_t power) const;
  [[nodiscard]] Matrix PowBlas(std::uint32_t power) const;
  [[nodiscard]] Matrix Copy() const;
  static Matrix Diagonal(std::size_t size);
  friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
  [[nodiscard]] Vector PageRank(double dampening, std::size_t iterations) const;
 private:
  Matrix(std::size_t height, std::size_t width, std::vector<double> values);
  std::size_t width_;
  std::size_t height_;
  std::vector<double> data_;
};


#endif //PAGERANK__MATRIX_HPP_
