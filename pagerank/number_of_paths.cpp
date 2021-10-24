#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include <omp.h>
#include "matrix.hpp"
#include "vector.hpp"

void RandomGraph(Matrix &matrix) {
  std::random_device rd;
  std::default_random_engine engine(rd());
  std::uniform_int_distribution<int> dist(0, 1);
  for (std::size_t i = 0; i < matrix.GetHeight(); ++i) {
    for (std::size_t j = 0; j < matrix.GetWidth(); ++j) {
      if (i != j) {
        matrix(i, j) = dist(engine);
      } else {
        matrix(i, j) = 0;
      }
    }
  }
}

long MeasureRuntimeOnce(std::size_t size) {
  Matrix a(size, size);
  RandomGraph(a);
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  auto result = a.Pow(size);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
}

auto MeasureRuntime(std::size_t size, std::size_t iterations) {
  std::vector<long> times(iterations);
  for (auto &it: times) {
    it = MeasureRuntimeOnce(size);
  }
  return std::accumulate(times.begin(), times.end(), 0L) / iterations;
}

long MeasureRuntimeOnceBlas(std::size_t size) {
  Matrix a(size, size);
  RandomGraph(a);
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  auto result = a.PowBlas(size);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
}

auto MeasureRuntimeBlas(std::size_t size, std::size_t iterations) {
  std::vector<long> times(iterations);
  for (auto &it: times) {
    it = MeasureRuntimeOnceBlas(size);
  }
  return std::accumulate(times.begin(), times.end(), 0L) / iterations;
}

int main() {
  const auto kMatrixSize = 10;
  omp_set_num_threads(16);
  Matrix a(kMatrixSize, kMatrixSize);
  RandomGraph(a);
  Matrix b = a.Pow(kMatrixSize);
  Matrix c = a.PowBlas(kMatrixSize);
  std::cout << "Testing\n";
  std::cout << std::fixed << std::setprecision(0) << a << std::endl << "OpenMP, self-made\n" << b << std::endl << "BLAS\n" << c << std::endl;
  std::cout << std::setprecision(5);

  std::cout << "OpenMP, self-made\n";
  std::vector<std::size_t> sizes = {10, 16, 20, 32, 40, 64, 80, 128, 160, 256, 320, 512, 640, 1024};
  for (const auto &it: sizes) {
    std::cout << "Measuring size " << it << ": " << static_cast<double>(MeasureRuntime(it, 10)) / 1e+6
              << " seconds" << std::endl;
  }

  std::cout << "BLAS\n";
  for (const auto &it: sizes) {
    std::cout << "BLAS. Measuring size " << it << ": " << static_cast<double>(MeasureRuntimeBlas(it, 10)) / 1e+6
              << " seconds" << std::endl;
  }
  return 0;
}
