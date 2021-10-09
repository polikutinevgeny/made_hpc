//
// Created by polikutin on 09/10/2021.
//

#include <stdexcept>
#include <numeric>
#include "matrix.hpp"

Matrix::Matrix(std::size_t height, std::size_t width) : width_(width), height_(height), data_(width * height) {}
Matrix::Matrix(std::size_t height, std::size_t width, double value)
    : width_(width), height_(height), data_(width * height, value) {}

Vector Matrix::operator*(const Vector &other) const {
  if (this->height_ != other.size_) {
    throw std::invalid_argument("Size mismatch");
  }
  Vector output(other.size_);
  for (std::size_t row_number = 0; row_number < this->height_; ++row_number) {
    output(row_number) = std::inner_product(other.data_.begin(),
                                            other.data_.end(),
                                            this->data_.begin() + row_number * this->width_,
                                            0.0);
  }
  return output;
}

Matrix Matrix::operator*(const Matrix &other) const {
  if (this->width_ != other.height_) {
    throw std::invalid_argument("Size mismatch");
  }
  Matrix result(this->height_, other.width_, 0);
  for (std::size_t k = 0; k < this->width_; ++k) {
    for (std::size_t i = 0; i < this->height_; ++i) {
      for (std::size_t j = 0; j < other.width_; ++j) {
        result.data_[i * result.width_ + j] += this->data_[i * this->width_ + k] * other.data_[k * other.width_ + j];
      }
    }
  }
  return result;
}
double &Matrix::operator()(std::size_t row, std::size_t col) {
  return this->data_[row * this->width_ + col];
}
const double &Matrix::operator()(std::size_t row, std::size_t col) const {
  return this->data_[row * this->width_ + col];
}
std::size_t Matrix::GetWidth() const {
  return width_;
}
std::size_t Matrix::GetHeight() const {
  return height_;
}
