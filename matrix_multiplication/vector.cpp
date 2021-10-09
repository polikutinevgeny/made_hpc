//
// Created by polikutin on 09/10/2021.
//

#include "vector.hpp"
Vector::Vector(std::size_t size) : size_(size), data_(size) {}

double &Vector::operator()(std::size_t index) {
  return this->data_[index];
}

const double &Vector::operator()(std::size_t index) const {
  return this->data_[index];
}
std::size_t Vector::GetSize() const {
  return size_;
}
