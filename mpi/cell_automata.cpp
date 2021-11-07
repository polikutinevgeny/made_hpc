#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include <sstream>
#include <iterator>
#include "mpi.h"

inline int ruleTest(int rule, int left, int center, int right) {
  return (rule >> (left << 2 | center << 1 | right)) & 1;
}

void step(const std::vector<int> &space, std::vector<int> &dest_space, int rule) {
  for (int i = 1; i < space.size() - 1; ++i) {
    dest_space[i] = ruleTest(rule, space[i - 1], space[i], space[i + 1]);
  }
}

void exchange(std::vector<int> &space, MPI_Comm comm) {
  int left, right;
  MPI_Cart_shift(comm, 0, 1, &left, &right);
  MPI_Sendrecv(
      &space[1],
      1, MPI_INT, left, 0, &space[space.size() - 1], 1, MPI_INT, right, 0, comm, MPI_STATUS_IGNORE);
  MPI_Sendrecv(
      &space[space.size() - 2],
      1, MPI_INT, right, 0, &space[0], 1, MPI_INT, left, 0, comm, MPI_STATUS_IGNORE);
}

void init(std::vector<int> &space) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> dist(0, 1);
  for (auto &it: space) {
    it = dist(rng);
  }
}

void output(const std::vector<int> &space, MPI_File file) {
  std::stringstream ss;
  std::copy(space.begin() + 1, space.end() - 1, std::ostream_iterator<int>(ss, ""));
  ss << std::endl;
  std::string output = ss.str();
  MPI_File_write(file, output.c_str(), output.size(), MPI_CHAR, MPI_STATUS_IGNORE);
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int SPACE_SIZE = 1024 * 128;
  int STEPS = 1024 * 512;
  bool IS_PERIODIC = true;
  int RULE = 110;
  bool PRINT_OUTPUT = false;

  int prank, psize;
  MPI_Comm_rank(MPI_COMM_WORLD, &prank);
  MPI_Comm_size(MPI_COMM_WORLD, &psize);

  MPI_Comm ring_1D;
  int ndim = 1;
  int dims, periods;
  dims = psize;
  periods = IS_PERIODIC ? 1 : 0;
  MPI_Cart_create(MPI_COMM_WORLD,
                  ndim, &dims, &periods, 0, &ring_1D);

  MPI_Comm_rank(ring_1D, &prank);
  MPI_Comm_size(ring_1D, &psize);

  int SHARD_SIZE = SPACE_SIZE / psize + (prank == psize - 1 ? SPACE_SIZE % psize : 0);

  std::vector<int> shard(SHARD_SIZE + 2);
  std::vector<int> shard2(SHARD_SIZE + 2);
  init(shard);
  exchange(shard, ring_1D);

  MPI_File fh;
  if (PRINT_OUTPUT) {
    std::stringstream filename_stream;
    filename_stream << "output_" << prank << ".txt";
    MPI_File_open(MPI_COMM_SELF, filename_stream.str().c_str(), MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
  }

  for (int i = 0; i < STEPS; ++i) {
    step(shard, shard2, RULE);
    std::swap(shard, shard2);
    exchange(shard, ring_1D);
    if (PRINT_OUTPUT) {
      output(shard, fh);
    }
  }

  if (PRINT_OUTPUT) {
    MPI_File_close(&fh);
  }

//  int rank = 0;
//  while (rank < psize) {
//    if (prank == rank) {
//      std::cout << "Array printed by rank: " << prank << " " << shard.size() << std::endl;
//      for (int i = 0; i < shard.size(); ++i) {
//        std::cout << shard[i];
//      }
//      std::cout << std::endl;
//    }
//    rank++;
//    MPI_Barrier(ring_1D);
//  }

  MPI_Finalize();
  return 0;
}
