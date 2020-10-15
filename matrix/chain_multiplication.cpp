#include "chain_multiplication.h"

MultiplicationTreeElementPtr simple_chain_order(const std::vector<Matrix>& matrices) {
  if (matrices.empty()) {
    throw std::runtime_error("Empty matrix chain");
  }
  MultiplicationTreeElementPtr curr(new MultiplicationTreeLeaf(matrices[0]));
  for (size_t i = 1; i != matrices.size(); ++i) {
    MultiplicationTreeElementPtr left(curr.release());
    MultiplicationTreeElementPtr right(new MultiplicationTreeLeaf(matrices[i]));
    curr.reset(new MultiplicationTreeNode(std::move(left), std::move(right)));
  }
  return curr;
}


static MultiplicationTreeElementPtr construct_multiplication_tree(
    const std::vector<Matrix>& M,
    const std::vector<std::vector<size_t>>& best_split,
    size_t l, size_t r, size_t& node_id) {
  if (l == r) {
    return MultiplicationTreeElementPtr(new MultiplicationTreeLeaf(M[l], l + 1));
  }
  size_t curr_node_id = node_id;
  ++node_id;
  return MultiplicationTreeElementPtr(
      new MultiplicationTreeNode(
        construct_multiplication_tree(M, best_split, l, best_split[l][r], node_id),
        construct_multiplication_tree(M, best_split, best_split[l][r] + 1, r, node_id),
        curr_node_id
      )
  );
}

static MultiplicationTreeElementPtr construct_multiplication_tree(
    const std::vector<Matrix>& M,
    const std::vector<std::vector<size_t>>& best_split) {
  size_t node_id = M.size() + 1;
  return construct_multiplication_tree(M, best_split, 0, M.size() - 1, node_id);
}

MultiplicationTreeElementPtr optimal_chain_order(const std::vector<Matrix>& M) {
  if (M.empty()) {
    throw std::runtime_error("Empty matrix chain");
  }
  std::vector<size_t> dims;
  dims.reserve(M.size() + 1);
  dims.push_back(M[0].rows());
  for (auto&& m : M) {
    dims.push_back(m.columns());
  }

  // min_cost[l][r] stores minimum cost of multiplication
  // of chain matrices in [l, r]
  // M[l] * M[l + 1] * ... * M[r]
  std::vector<std::vector<size_t>> min_cost(M.size(),
                                            std::vector<size_t>(M.size()));
  // min_cost[i][i] always equals to 0
  for (size_t i = 0; i != min_cost.size(); ++i) {
    min_cost[i][i] = 0;
  }

  std::vector<std::vector<size_t>> best_split(M.size(),
                                              std::vector<size_t>(M.size()));

  for (size_t len = 2; len <= M.size(); ++len) {
    for (size_t l = 0; l + len <= M.size(); ++l) {
      size_t r = l + len - 1;
      // Find minimum cost between
      // (M[l]) * (M[l + 1] * ... * M[r])
      // (M[l] * M[l + 1]) * (M[l + 2] * .. M[r])
      // ...
      // (M[l] * ... * M[split]) * (M[split + 1] * ... * M[r])
      // ...
      // (M[l] * ... * M[r - 1]) * (M[r])
      size_t min = 0;
      size_t min_split = l;
      for (size_t split = l; split != r; ++split) {
        size_t cost = dims[l] * dims[split + 1] * dims[r + 1] +
                      min_cost[l][split] + min_cost[split + 1][r];
        if (split == l || cost < min) {
          min = cost;
          min_split = split;
        }
      }
      min_cost[l][r] = min;
      best_split[l][r] = min_split;
    }
  }
  auto root = construct_multiplication_tree(M, best_split);
  return root;
}

