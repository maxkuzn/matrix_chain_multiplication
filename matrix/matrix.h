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

  Matrix(const std::vector<std::vector<double>>& data) {
    if (data.empty() || data[0].empty()) {
      throw std::runtime_error("Cannot construct empty matrix");
    }
    size_t rows = data.size();
    size_t columns = data[0].size();
    columns_ = columns;
    data_.reserve(rows * columns);
    for (size_t i = 0; i != rows; ++i) {
      if (data[i].size() != columns) {
        throw std::runtime_error("Different row size");
      }
      for (size_t j = 0; j != columns; ++j) {
        data_.push_back(data[i][j]);
      }
    }
  }

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


std::ostream& operator<<(std::ostream& os, const Matrix::Shape& s) {
  return os << "(" << s.rows << ", " << s.columns << ")";
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
  for (size_t i = 0; i != m.rows(); ++i) {
    if (i == 0) {
      os << "(";
    } else {
      os << " ";
    }
    for (size_t j = 0; j != m.columns(); ++j) {
      os << m(i, j);
      if (j + 1 != m.columns()) {
        os << '\t';
      }
    }
    if (i + 1 == m.rows()) {
      os << ')';
    } else {
      os << '\n';
    }
  }
  return os;
}


bool operator==(const Matrix& lhs, const Matrix& rhs) {
  static const double kMaxDifferenceForEquality = 1e-7;
  if (lhs.shape() != rhs.shape()) {
    return false;
  }
  double diff = 0;
  for (size_t i = 0; i != lhs.rows(); ++i) {
    for (size_t j = 0; j != lhs.columns(); ++j) {
      diff += (lhs(i, j) - rhs(i, j)) * (lhs(i, j) - rhs(i, j));
    }
  }
  return diff < kMaxDifferenceForEquality;
}

bool operator!=(const Matrix& lhs, const Matrix& rhs) {
  return !(lhs == rhs);
}


Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
  if (lhs.columns() != rhs.rows()) {
    std::stringstream ss;
    ss << "Cannot multiply matrix with shape "
       << lhs.shape()
       << " to matrix with shape "
       << rhs.shape();
    throw std::runtime_error(ss.str());
  }
  Matrix res(lhs.rows(), rhs.columns());
  for (size_t i = 0; i != res.rows(); ++i) {
    for (size_t j = 0; j != res.columns(); ++j) {
      res(i, j) = 0;
      for (size_t k = 0; k != lhs.columns(); ++k) {
        res(i, j) += lhs(i, k) * rhs(k, j);
      }
    }
  }
  return res;
}

