#include "gtest/gtest.h"

#include <matrix/matrix.h>

TEST(matrix, simple) {
  size_t rows = 2;
  size_t columns = 3;
  Matrix a(rows, columns);
  ASSERT_EQ(a.rows(), rows);
  ASSERT_EQ(a.columns(), columns);
  ASSERT_EQ(a.size(), rows * columns);

  double value = 4.5;
  a[0][1] = value;
  ASSERT_EQ(a[0][1], value);
};

TEST(matrix, constructor) {
  Matrix a({
    {1, 2},
    {3, 4},
  });

  ASSERT_EQ(a.rows(), 2);
  ASSERT_EQ(a.columns(), 2);
  ASSERT_EQ(a.size(), 4);

  ASSERT_EQ(a[0][0], 1);
  ASSERT_EQ(a[0][1], 2);
  ASSERT_EQ(a[1][0], 3);
  ASSERT_EQ(a[1][1], 4);
  ASSERT_EQ(a(0, 0), 1);
  ASSERT_EQ(a(0, 1), 2);
  ASSERT_EQ(a(1, 0), 3);
  ASSERT_EQ(a(1, 1), 4);
};

TEST(matrix, comparison) {
  Matrix a(2, 2, 1);
  Matrix b(2, 2, 1);
  Matrix c({
    {1, 2},
    {3, 4}
  });
  Matrix d({
    {1, 2},
    {3, 4}
  });
  Matrix e(3, 1);

  ASSERT_EQ(a[0][0], 1);
  ASSERT_EQ(a[0][1], 1);
  ASSERT_EQ(a[1][0], 1);
  ASSERT_EQ(a[1][1], 1);

  ASSERT_EQ(a, b);
  ASSERT_EQ(c, d);
  ASSERT_NE(a, c);
  ASSERT_NE(a, e);
  ASSERT_NE(c, e);

  ASSERT_EQ(a.shape(), b.shape());
  ASSERT_EQ(a.shape(), c.shape());
  ASSERT_NE(a.shape(), e.shape());
  ASSERT_EQ(c.shape(), c.shape());
};
