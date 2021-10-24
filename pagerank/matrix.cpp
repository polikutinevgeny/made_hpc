//
// Created by polikutin on 09/10/2021.
//

#include <stdexcept>
#include <numeric>
#include <utility>
#include <ostream>
#include "matrix.hpp"
#include <cblas.h>
#include <random>
#include <iostream>

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
#pragma omp parallel default(none) shared(other, data_, result)
  {
#pragma omp for schedule(static) nowait
    for (std::size_t i = 0; i < this->height_; ++i) {
      const auto kRRow = i * result.width_;
      for (std::size_t k = 0; k < this->width_; ++k) {
        auto const kAVal = data_[i * this->width_ + k];
        const auto kBRow = k * other.width_;
        for (std::size_t j = 0; j < other.width_; ++j) {
          result.data_[kRRow + j] += kAVal * other.data_[kBRow + j];
        }
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
Matrix Matrix::Pow(uint32_t power) const {
  if (this->width_ != this->height_) {
    throw std::runtime_error("Invalid dimensions");
  }
  Matrix result = Matrix::Diagonal(this->width_);
  Matrix base = this->Copy();
  while (true) {
    if (power % 2 == 1) {
      result = result * base;
    }
    power = power / 2;
    if (power > 0) {
      base = base * base;
    } else {
      break;
    }
  }
  return result;
}
Matrix Matrix::Diagonal(std::size_t size) {
  Matrix result(size, size, 0);
  for (int i = 0; i < size; ++i) {
    result(i, i) = 1;
  }
  return result;
}
Matrix Matrix::Copy() const {
  return {this->height_, this->width_, this->data_};
}
Matrix::Matrix(std::size_t height, std::size_t width, std::vector<double> values)
    : width_(width), height_(height), data_(std::move(values)) {}
std::ostream &operator<<(std::ostream &os, const Matrix &m) {
  for (int i = 0; i < m.height_; ++i) {
    for (int j = 0; j < m.width_; ++j) {
      os << m(i, j) << " ";
    }
    os << std::endl;
  }
  return os;
}
Matrix Matrix::BlasMultiply(const Matrix &other) const {
  if (this->width_ != other.height_) {
    throw std::invalid_argument("Size mismatch");
  }
  Matrix result(this->height_, other.width_, 0);
  cblas_dgemm(CblasRowMajor,
              CblasNoTrans,
              CblasNoTrans,
              this->height_,
              other.width_,
              this->width_,
              1.0,
              this->data_.data(),
              this->height_,
              other.data_.data(),
              other.height_,
              0.0,
              result.data_.data(),
              result.height_);
  return result;
}
Matrix Matrix::PowBlas(std::uint32_t power) const {
  if (this->width_ != this->height_) {
    throw std::runtime_error("Invalid dimensions");
  }
  Matrix result = Matrix::Diagonal(this->width_);
  Matrix base = this->Copy();
  while (true) {
    if (power % 2 == 1) {
      result = result.BlasMultiply(base);
    }
    power = power / 2;
    if (power > 0) {
      base = base.BlasMultiply(base);
    } else {
      break;
    }
  }
  return result;
}
Vector Matrix::PageRank(double dampening, std::size_t iterations) const {
  if (this->width_ != this->height_) {
    throw std::runtime_error("Invalid dimensions");
  }

  Vector initial_guess(this->width_);

  for (int i = 0; i < initial_guess.size_; ++i) {
    initial_guess(i) = 1.0 / initial_guess.size_;
  }

  Matrix op = this->Copy();
  std::vector<double> column_sums(op.width_, 0);
  for (int i = 0; i < op.height_; ++i) {
    for (int j = 0; j < op.width_; ++j) {
      column_sums[j] += op(i, j);
    }
  }
  for (int i = 0; i < op.height_; ++i) {
    for (int j = 0; j < op.width_; ++j) {
      op(i, j) /= column_sums[j];
    }
  }
  for (int i = 0; i < op.height_; ++i) {
    for (int j = 0; j < op.width_; ++j) {
      op(i, j) = op(i, j) * dampening + (1 - dampening) / op.width_;
    }
  }
  auto result_op = op.PowBlas(iterations);

  return result_op * initial_guess;
}
