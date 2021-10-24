//
// Created by polikutin on 09/10/2021.
//

#ifndef PAGERANK__VECTOR_HPP_
#define PAGERANK__VECTOR_HPP_

#include <vector>

class Vector {
 public:
  Vector(std::size_t size);
  double &operator()(std::size_t index);
  const double &operator()(std::size_t index) const;
  friend std::ostream& operator<<(std::ostream& os, const Vector& m);
 private:
  std::size_t size_;
 public:
  std::size_t GetSize() const;
 private:
  std::vector<double> data_;
  friend class Matrix;
};

#endif //PAGERANK__VECTOR_HPP_
