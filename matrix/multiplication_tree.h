#include <memory>

#include "matrix.h"

class MultiplicationTreeElement {
 public:
  virtual ~MultiplicationTreeElement() = default;

  virtual Matrix get() const = 0;

  virtual size_t number_of_float_multiplications() const = 0;

  virtual size_t rows() const = 0;
  virtual size_t columns() const = 0;
  virtual Matrix::Shape shape() const = 0;
};

using MultiplicationTreeElementPtr =  std::unique_ptr<MultiplicationTreeElement>;


class MultiplicationTreeLeaf : public MultiplicationTreeElement {
 public:
  MultiplicationTreeLeaf(const Matrix& matrix) : matrix_(matrix) {
  }


  Matrix get() const override {
    return matrix_;
  }

  size_t number_of_float_multiplications() const override {
    return 0;
  }

  size_t rows() const override {
    return matrix_.rows();
  }

  size_t columns() const override {
    return matrix_.columns();
  }

  Matrix::Shape shape() const override {
    return matrix_.shape();
  }

 private:
  const Matrix& matrix_;
};


class MultiplicationTreeNode : public MultiplicationTreeElement {
 public:
  MultiplicationTreeNode(MultiplicationTreeElementPtr&& left,
                         MultiplicationTreeElementPtr&& right)
    : left_(std::move(left))
    , right_(std::move(right))
    , rows_(left_ ? left_->rows() : 0)
    , columns_(right_ ? right_->columns() : 0)
  {
    if (!left_ || !right_) {
      throw std::runtime_error("At least one element wasn't provided");
    }
    if (left_->columns() != right_->rows()) {
      throw std::runtime_error("Number of columns for the left matrix doesn't equal to number of rows for the right matrix");
    }
  }


  Matrix get() const override {
    return left_->get() * right_->get();
  }

  size_t number_of_float_multiplications() const override {
    return rows() * columns() * left_->columns() +
           left_->number_of_float_multiplications() +
           right_->number_of_float_multiplications();
  }

  size_t rows() const override {
    return rows_;
  }

  size_t columns() const override {
    return columns_;
  }

  Matrix::Shape shape() const override {
    return Matrix::Shape{.rows = rows(), .columns = columns()};
  }

 private:
   MultiplicationTreeElementPtr left_;
   MultiplicationTreeElementPtr right_;
   const size_t rows_;
   const size_t columns_;
};

