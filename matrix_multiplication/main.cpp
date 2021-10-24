#include <iostream>
#include <chrono>
#include <random>
#include "matrix.hpp"
#include "vector.hpp"

void random_fill(Matrix &matrix) {
  std::random_device rd;
  std::default_random_engine engine(rd());
  std::uniform_real_distribution<double> dist(-1e-3, 1e-3);
  for (std::size_t i = 0; i < matrix.GetHeight(); ++i) {
    for (std::size_t j = 0; j < matrix.GetWidth(); ++j) {
      matrix(i, j) = dist(engine);
    }
  }
}

long measure_runtime_matrix_once(std::size_t size) {
  Matrix a(size, size), b(size, size);
  random_fill(a);
  random_fill(b);
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  auto result = a * b;
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
}

auto measure_runtime_matrix(std::size_t size, std::size_t iterations) {
  std::vector<long> times(iterations);
  for (auto &it: times) {
    it = measure_runtime_matrix_once(size);
  }
  return std::accumulate(times.begin(), times.end(), 0L) / iterations;
}

int main() {

  return 0;
}
