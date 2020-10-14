#pragma once

#include "matrix.h"

Matrix gen_random_matrix(size_t rows, size_t columns);

std::vector<Matrix> gen_random_matrix_chain(const std::vector<size_t>& dims);

