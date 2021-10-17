#include <omp.h>
#include <random>
#include <iostream>
#include <iomanip>

int main() {
  std::uint64_t N = 1000000000;
  int N_THREADS = 16;
  std::random_device rd; // Non-determinisic random source
  std::uint64_t counter = 0;

  omp_set_dynamic(0);
  omp_set_num_threads(N_THREADS);

  auto pre_time = omp_get_wtime();

#pragma omp parallel default(none) shared(N, rd, counter)
  {
    unsigned int seed;
#pragma omp critical
    {
      seed = rd(); // Seed in critical section
    }
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(-1, 1);
#pragma omp for reduction(+:counter)
    for (std::uint64_t i = 0; i < N; ++i) {
      auto x = dist(rng);
      auto y = dist(rng);
      if (x * x + y * y <= 1) {
        ++counter;
      }
    }
  }

  auto post_time = omp_get_wtime();
  auto pi = static_cast<double>(counter) / N * 4;

  std::cout << "Number of threads: " << omp_get_max_threads() << std::endl;
  std::cout << "Iterations: " << N << std::endl;
  std::cout << std::setprecision(20);
  std::cout << "Pi: " << pi << std::endl;
  std::cout << "Error: " << std::abs(pi - M_PI) << std::endl;
  std::cout << std::setprecision(4);
  std::cout << "Time spent: " << post_time - pre_time << " seconds" << std::endl;
}