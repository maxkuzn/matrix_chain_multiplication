#pragma once

#include "matrix.h"
#include "multiplication_tree.h"

MultiplicationTreeElementPtr simple_chain_order(const std::vector<Matrix>& matrices);

MultiplicationTreeElementPtr optimal_chain_order(const std::vector<Matrix>& matrices);
