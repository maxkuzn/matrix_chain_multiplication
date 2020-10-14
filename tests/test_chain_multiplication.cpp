#include "gtest/gtest.h"

#include <matrix/matrix.h>
#include <matrix/chain_multiplication.h>

TEST(chain_multiplication, simple) {
  Matrix A({
    {1, 2},
    {3, 4},
    {5, 6},
  });

  Matrix B({
    {7, 8, 9},
    {10, 11, 12},
  });

  Matrix C({
    {13, 14},
    {15, 16},
    {17, 18},
  });

  Matrix Res({
    {1362, 1452},
    {3088, 3292},
    {4814, 5132},
  });

  std::vector<Matrix> chain{A, B, C};

  auto ABC_node = simple_chain_order(chain);
  ASSERT_EQ(ABC_node->rows(), Res.rows());
  ASSERT_EQ(ABC_node->columns(), Res.columns());
  ASSERT_EQ(ABC_node->number_of_float_multiplications(),
            A.rows() * B.rows() * B.columns() +
            A.rows() * C.rows() * C.columns());
  ASSERT_EQ(ABC_node->get(), Res);
};

TEST(chain_multiplication, optimal) {
  Matrix A({
    {1, 2},
    {3, 4},
    {5, 6},
  });

  Matrix B({
    {7, 8, 9},
    {10, 11, 12},
  });

  Matrix C({
    {13, 14},
    {15, 16},
    {17, 18},
  });

  Matrix Res({
    {1362, 1452},
    {3088, 3292},
    {4814, 5132},
  });

  std::vector<Matrix> chain{A, B, C};

  auto ABC_node = optimal_chain_order(chain);
  ASSERT_EQ(ABC_node->rows(), Res.rows());
  ASSERT_EQ(ABC_node->columns(), Res.columns());
  ASSERT_EQ(ABC_node->number_of_float_multiplications(),
            B.rows() * C.rows() * C.columns() +
            A.rows() * B.rows() * C.columns());
  ASSERT_EQ(ABC_node->get(), Res);
};

