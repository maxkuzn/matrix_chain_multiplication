#include "gtest/gtest.h"

#include <matrix/matrix.h>

TEST(matrix, simple) {
  size_t rows = 2;
  size_t columns = 3;
  Matrix a(rows, columns);
  ASSERT_EQ(a.height(), rows);
  ASSERT_EQ(a.width(), columns);
  ASSERT_EQ(a.size(), rows * columns);

  double value = 4.5;
  a[0][1] = value;
  ASSERT_EQ(a[0][1], value);
};

