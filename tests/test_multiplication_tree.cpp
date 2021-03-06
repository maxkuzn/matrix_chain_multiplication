#include "gtest/gtest.h"

#include <matrix/multiplication_tree.h>

TEST(multiplication_tree, simple) {
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

  {
   // Let's define next tree
    //     ABC
    //    /   \
    //   AB   C
    //  /  \
    // A   B
    MultiplicationTreeElementPtr A_node(new MultiplicationTreeLeaf(A));
    MultiplicationTreeElementPtr B_node(new MultiplicationTreeLeaf(B));
    MultiplicationTreeElementPtr C_node(new MultiplicationTreeLeaf(C));

    MultiplicationTreeElementPtr AB_node(
      new MultiplicationTreeNode(std::move(A_node), std::move(B_node)));

    ASSERT_EQ(AB_node->rows(), A.rows());
    ASSERT_EQ(AB_node->columns(), B.columns());
    ASSERT_EQ(AB_node->number_of_float_multiplications(),
              A.rows() * B.rows() * B.columns());

    MultiplicationTreeElementPtr ABC_node(
      new MultiplicationTreeNode(std::move(AB_node), std::move(C_node)));
    
    ASSERT_EQ(ABC_node->rows(), Res.rows());
    ASSERT_EQ(ABC_node->columns(), Res.columns());
    ASSERT_EQ(ABC_node->shape(), Res.shape());

    ASSERT_EQ(ABC_node->number_of_float_multiplications(),
              A.rows() * B.rows() * B.columns() +
              A.rows() * C.rows() * C.columns());
    ASSERT_EQ(ABC_node->get(), Res);
  }
  {
    // Let's define next tree
    //     ABC
    //    /   \
    //   A    BC
    //       /  \
    //      B   C
    MultiplicationTreeElementPtr A_node(new MultiplicationTreeLeaf(A));
    MultiplicationTreeElementPtr B_node(new MultiplicationTreeLeaf(B));
    MultiplicationTreeElementPtr C_node(new MultiplicationTreeLeaf(C));

    MultiplicationTreeElementPtr BC_node(
      new MultiplicationTreeNode(std::move(B_node), std::move(C_node)));

    ASSERT_EQ(BC_node->rows(), B.rows());
    ASSERT_EQ(BC_node->columns(), C.columns());
    ASSERT_EQ(BC_node->number_of_float_multiplications(),
              B.rows() * C.rows() * C.columns());

    MultiplicationTreeElementPtr ABC_node(
      new MultiplicationTreeNode(std::move(A_node), std::move(BC_node)));
    
    ASSERT_EQ(ABC_node->rows(), Res.rows());
    ASSERT_EQ(ABC_node->columns(), Res.columns());
    ASSERT_EQ(ABC_node->shape(), Res.shape());

    ASSERT_EQ(ABC_node->number_of_float_multiplications(),
              B.rows() * C.rows() * C.columns() +
              A.rows() * B.rows() * C.columns());
    ASSERT_EQ(ABC_node->get(), Res);
  }
};

