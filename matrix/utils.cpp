#include "utils.h"

#include <random>

Matrix gen_random_matrix(size_t rows, size_t columns) {
  static std::mt19937 gen(42);
  std::uniform_real_distribution<double> dist(-1.0, 1.0);
  Matrix m(rows, columns);
  for (size_t i = 0; i != rows; ++i) {
    for (size_t j = 0; j != columns; ++j) {
      m(i, j) = dist(gen);
    }
  }
  return m;
}

std::vector<Matrix> gen_random_matrix_chain(const std::vector<size_t>& dims) {
  if (dims.size() < 2) {
    throw std::runtime_error("At least two dimentions should be provided");
  }
  std::vector<Matrix> ms;
  ms.reserve(dims.size() - 1);
  for (size_t i = 1; i != dims.size(); ++i) {
    ms.emplace_back(gen_random_matrix(dims[i - 1], dims[i]));
  }
  return ms;
}

