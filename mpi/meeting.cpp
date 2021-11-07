#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <random>
#include "mpi.h"

int getBufferSize(const std::vector<int> &ranks, const std::vector<std::string> &names) {
  int buffer_size = sizeof(std::size_t); // Number of processors
  buffer_size += ranks.size() * sizeof(int); // Processor ranks
  buffer_size += names.size() * sizeof(std::size_t); // Processor name lengths
  for (const auto &it: names) {
    buffer_size += it.size(); // Processor names
  }
  return buffer_size;
}

int selectTarget(int size, const std::vector<int> &visited) {
  std::set<int> visited_set(visited.begin(), visited.end());
  std::vector<int> targets;
  for (int i = 0; i < size; ++i) {
    if (visited_set.find(i) == visited_set.end()) {
      targets.push_back(i);
    }
  }
  if (targets.empty()) {
    return -1;
  }
  std::cout << "Possible targets ";
  for (const auto &it: targets) {
    std::cout << it << " ";
  }
  std::cout << std::endl;
  std::random_device rd;
  std::uniform_int_distribution<int> dist(0, targets.size() - 1);
  return targets[dist(rd)];
}

void send(MPI_Comm comm, const std::vector<int> &ranks, const std::vector<std::string> &names, int to_rank) {
  int buffer_size = getBufferSize(ranks, names);
  int buffer_position = 0;
  std::vector<char> buffer(buffer_size);
  char *buffer_ptr = buffer.data();

  auto total = ranks.size();
  MPI_Pack(&total, 1, MPI_UNSIGNED_LONG_LONG, buffer_ptr, buffer_size, &buffer_position, comm);

  for (int i = 0; i < total; ++i) {
    MPI_Pack(&ranks[i], 1, MPI_INT, buffer_ptr, buffer_size, &buffer_position, comm);
    auto name_size = names[i].size();
    MPI_Pack(&name_size, 1, MPI_UNSIGNED_LONG_LONG, buffer_ptr, buffer_size, &buffer_position, comm);
    MPI_Pack(names[i].c_str(), static_cast<int>(name_size), MPI_CHAR, buffer_ptr, buffer_size, &buffer_position, comm);
  }
  MPI_Ssend(buffer_ptr, buffer_size, MPI_PACKED, to_rank, 0, comm);
  std::cout << "Sent to rank " << to_rank << std::endl;
}

void recieve(MPI_Comm comm, std::vector<int> &ranks, std::vector<std::string> &names) {
  MPI_Status status;
  MPI_Probe(MPI_ANY_SOURCE, 0, comm, &status);
  int buffer_size;
  MPI_Get_count(&status, MPI_PACKED, &buffer_size);

  int buffer_position = 0;
  std::vector<char> buffer(buffer_size);
  char *buffer_ptr = buffer.data();

  MPI_Recv(buffer_ptr, buffer_size, MPI_PACKED, MPI_ANY_SOURCE, 0, comm, &status);

  std::size_t total;
  MPI_Unpack(buffer_ptr, buffer_size, &buffer_position, &total, 1, MPI_UNSIGNED_LONG_LONG, comm);

  for (int i = 0; i < total; ++i) {
    int rank;
    MPI_Unpack(buffer_ptr, buffer_size, &buffer_position, &rank, 1, MPI_INT, comm);
    ranks.push_back(rank);

    std::size_t name_size;
    MPI_Unpack(buffer_ptr, buffer_size, &buffer_position, &name_size, 1, MPI_UNSIGNED_LONG_LONG, comm);
    std::vector<char> name_buf(name_size);
    MPI_Unpack(buffer_ptr, buffer_size, &buffer_position, name_buf.data(), static_cast<int>(name_size), MPI_CHAR, comm);
    std::string name(name_buf.begin(), name_buf.end());
    names.push_back(name);
  }
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int prank, psize;
  MPI_Comm_rank(MPI_COMM_WORLD, &prank);
  MPI_Comm_size(MPI_COMM_WORLD, &psize);

  std::stringstream ss;
  ss << "Processor #" << prank;
  auto name = ss.str();

  std::vector<int> ranks;
  std::vector<std::string> names;

  if (prank == 0) {
    ranks.push_back(prank);
    names.push_back(name);
    int target = selectTarget(psize, ranks);
    if (target == -1) {
      MPI_Finalize();
      return 0;
    }
    std::cout << "Sending from " << prank << " to " << target << std::endl;
    send(MPI_COMM_WORLD, ranks, names, target);
  } else {
    recieve(MPI_COMM_WORLD, ranks, names);
    ranks.push_back(prank);
    names.push_back(name);
    int target = selectTarget(psize, ranks);
    if (target == -1) {
      std::cout << "Visited all processes in order:" << std::endl;
      for (const auto &name: names) {
        std::cout << name << std::endl;
      }
      MPI_Finalize();
      return 0;
    }
    std::cout << "Sending from " << prank << " to " << target << std::endl;
    send(MPI_COMM_WORLD, ranks, names, target);
  }
  MPI_Finalize();
  return 0;
}
