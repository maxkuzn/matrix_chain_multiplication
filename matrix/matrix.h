#pragma once

#include <vector>
#include <iostream>
#include <sstream>


class Matrix {
 public:
  // Constructors and operators=
  Matrix(size_t rows, size_t columns) : data_(rows * columns), columns_(columns) {
    if (data_.empty()) {
      throw std::runtime_error("Cannot construct empty matrix");
    }
  }

  Matrix(size_t rows, size_t columns, double value)
    : data_(rows * columns, value), columns_(columns) {
    if (data_.empty()) {
      throw std::runtime_error("Cannot construct empty matrix");
    }
  }

  Matrix(const std::vector<std::vector<double>>& data);
  
  Matrix(const Matrix& other) {
    *this = other;
  }

  Matrix(Matrix&& other) {
    *this = std::move(other);
  }

  Matrix& operator=(const Matrix& other) {
    data_ = other.data_;
    columns_ = other.columns_;
    return *this;
  }

  Matrix& operator=(Matrix&& other) {
    data_ = std::move(other.data_);
    columns_ = other.columns_;
    return *this;
  }


  // Shape methods
  size_t columns() const {
    return columns_;
  }

  size_t rows() const {
    return size() / columns_;
  }

  size_t size() const {
    return data_.size();
  }

  struct Shape {
    size_t rows;
    size_t columns;

    bool operator==(const Shape& other) const {
      return rows == other.rows && columns == other.columns;
    }

    bool operator!=(const Shape& other) const {
      return !(*this == other);
    }
  };

  Shape shape() const {
    return Shape{rows(), columns()};
  }


  // operators[]
 private:
  class RowViewer {
   public:
    double& operator[](size_t column_idx) {
      return data_[row_idx_ * columns_ + column_idx];
    }

   private:
    friend class Matrix;

    RowViewer(std::vector<double>& data, size_t columns, size_t row_idx)
      : data_(data), columns_(columns), row_idx_(row_idx) {
    }

    std::vector<double>& data_;
    const size_t columns_;
    const size_t row_idx_;
  };

  class ConstRowViewer {
   public:
    double operator[](size_t column_idx) const {
      return data_[row_idx_ * columns_ + column_idx];
    }

   private:
    friend class Matrix;

    ConstRowViewer(const std::vector<double>& data, size_t columns, size_t row_idx)
      : data_(data), columns_(columns), row_idx_(row_idx) {
    }

    const std::vector<double>& data_;
    const size_t columns_;
    const size_t row_idx_;
  };

 public:
  RowViewer operator[](size_t row_idx) {
    return RowViewer(data_, columns_, row_idx);
  }

  ConstRowViewer operator[](size_t row_idx) const {
    return ConstRowViewer(data_, columns_, row_idx);
  }

  // Faster version of indexing
  double operator()(size_t row_idx, size_t column_idx) const {
    return data_[row_idx * columns_ + column_idx];
  }

  double& operator()(size_t row_idx, size_t column_idx) {
    return data_[row_idx * columns_ + column_idx];
  }

 private:
  std::vector<double> data_;
  size_t columns_;
};


std::ostream& operator<<(std::ostream& os, const Matrix::Shape& s);
std::ostream& operator<<(std::ostream& os, const Matrix& m);

bool operator==(const Matrix& lhs, const Matrix& rhs);
bool operator!=(const Matrix& lhs, const Matrix& rhs);

Matrix operator*(const Matrix& lhs, const Matrix& rhs);

