#include <iostream>
#include <sstream>

#include <matrix/matrix.h>
#include <matrix/utils.h>
#include <matrix/chain_multiplication.h>


int main() {
  size_t n;
  std::cin >> n;
  std::vector<size_t> dims(n);
  for (size_t& d : dims) {
    std::cin >> d;
  }
  
  auto chain = gen_random_matrix_chain(dims);
  auto root = optimal_chain_order(chain);
  std::stringstream ss;
  root->print_tree(ss);
  std::cout << ss.str();
}

