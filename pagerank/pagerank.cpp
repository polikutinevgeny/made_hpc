#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include <omp.h>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include "matrix.hpp"
#include "vector.hpp"

static const int kTopStations = 10;
int main() {
  std::ifstream stations("stations_data.tsv");
  std::string line;
  std::unordered_map<std::size_t, std::string> station_names;
  while (std::getline(stations, line)) {
    auto split_pos = line.find('\t');
    auto id = std::stoi(line.substr(0, split_pos));
    auto name = line.substr(split_pos + 1, line.size() - split_pos - 1);
    station_names[id] = name;
  }

  std::ifstream transfers("routes_data.tsv");
  Matrix adjacency_matrix(station_names.size(), station_names.size());
  while (std::getline(transfers, line)) {
    auto split_pos = line.find('\t');
    auto from = std::stoi(line.substr(0, split_pos));
    auto to = std::stoi(line.substr(split_pos + 1, line.size() - split_pos - 1));
    adjacency_matrix(from, to) = 1;
  }

  // Naive
  std::unordered_map<std::size_t, std::size_t> incoming_connections;
  for (std::size_t i = 0; i < adjacency_matrix.GetHeight(); ++i) {
    for (std::size_t j = 0; j < adjacency_matrix.GetWidth(); ++j) {
      if (adjacency_matrix(i, j) > 0) {
        incoming_connections[j]++;
      }
    }
  }
  std::vector<std::pair<std::size_t, std::size_t>> incoming_connections_pairs;
  incoming_connections_pairs.reserve(incoming_connections.size());
  for (const auto kIt: incoming_connections) {
    incoming_connections_pairs.emplace_back(kIt.second, kIt.first);
  }
  std::sort(incoming_connections_pairs.begin(),
            incoming_connections_pairs.end(),
            [](auto left, auto right) { return left > right; });
  std::cout << "Naive" << std::endl;
  for (int i = 0; i < kTopStations; ++i) {
    std::cout << station_names[incoming_connections_pairs[i].second] << " " << incoming_connections_pairs[i].first
              << std::endl;
  }
  std::cout << std::endl;

  // Pagerank
  auto scores = adjacency_matrix.PageRank(0.85, 10000);
  std::cout << "Pagerank" << std::endl;
  std::cout << std::setprecision(8);
  std::vector<std::pair<double, std::size_t>> pagerank_pairs;
  pagerank_pairs.reserve(scores.GetSize());
  for (int i = 0; i < scores.GetSize(); ++i) {
    pagerank_pairs.emplace_back(scores(i), i);
  }
  std::sort(pagerank_pairs.begin(),
            pagerank_pairs.end(),
            [](auto left, auto right) { return left > right; });
  for (int i = 0; i < kTopStations; ++i) {
    std::cout << station_names[pagerank_pairs[i].second] << " " << pagerank_pairs[i].first
              << std::endl;
  }
  std::cout << std::endl;
  return 0;
}