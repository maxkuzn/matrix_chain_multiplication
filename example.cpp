#include "matrix.h"

#include <iostream>

int main() {
  Matrix a(2, 2);
  std::cout << a << '\n';
  Matrix b(2, 2, 42);
  std::cout << b << '\n';
}

