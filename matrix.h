#pragma once

#include <vector>
#include <iostream>
#include <sstream>

class Matrix {
 public:
  // Constructors and operators=
  Matrix(size_t height, size_t width) : data_(height * width), width_(width) {
  }

  Matrix(size_t height, size_t width, double value)
    : data_(height * width, value), width_(width) {
  }

  Matrix(const Matrix& other) {
    *this = other;
  }

  Matrix(Matrix&& other) {
    *this = std::move(other);
  }

  Matrix& operator=(const Matrix& other) {
    data_ = other.data_;
    width_ = other.width_;
    return *this;
  }

  Matrix& operator=(Matrix&& other) {
    data_ = std::move(other.data_);
    width_ = other.width_;
    return *this;
  }


  // Shape methods
  size_t width() const {
    return width_;
  }

  size_t height() const {
    return size() / width_;
  }

  size_t size() const {
    return data_.size();
  }

  struct Shape {
    size_t height;
    size_t width;

    bool operator==(const Shape& other) const {
      return height == other.height && width == other.width;
    }

    bool operator!=(const Shape& other) const {
      return !(*this == other);
    }
  };

  Shape shape() const {
    return Shape{height(), width()};
  }


  // operators[]
 private:
  class RowViewer {
   public:
    double& operator[](size_t column_idx) {
      return data_[row_idx_ * width_ + column_idx];
    }

   private:
    friend class Matrix;

    RowViewer(std::vector<double>& data, size_t width, size_t row_idx)
      : data_(data), width_(width), row_idx_(row_idx) {
    }

    std::vector<double>& data_;
    const size_t width_;
    const size_t row_idx_;
  };

  class ConstRowViewer {
   public:
    double operator[](size_t column_idx) const {
      return data_[row_idx_ * width_ + column_idx];
    }

   private:
    friend class Matrix;

    ConstRowViewer(const std::vector<double>& data, size_t width, size_t row_idx)
      : data_(data), width_(width), row_idx_(row_idx) {
    }

    const std::vector<double>& data_;
    const size_t width_;
    const size_t row_idx_;
  };

 public:
  RowViewer operator[](size_t row_idx) {
    return RowViewer(data_, width_, row_idx);
  }

  ConstRowViewer operator[](size_t row_idx) const {
    return ConstRowViewer(data_, width_, row_idx);
  }

 private:
  std::vector<double> data_;
  size_t width_;
};


std::ostream& operator<<(std::ostream& os, const Matrix::Shape& s) {
  return os << "(" << s.height << ", " << s.width << ")";
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
  for (size_t i = 0; i != m.height(); ++i) {
    if (i == 0) {
      os << "(";
    } else {
      os << " ";
    }
    for (size_t j = 0; j != m.width(); ++j) {
      os << m[i][j];
      if (j + 1 != m.width()) {
        os << '\t';
      }
    }
    if (i + 1 == m.height()) {
      os << ')';
    } else {
      os << '\n';
    }
  }
  return os;
}


bool operator==(const Matrix& lhs, const Matrix& rhs) {
  if (lhs.height() != rhs.height() || lhs.width() != rhs.width()) {
    return false;
  }
  for (size_t i = 0; i != lhs.height(); ++i) {
    for (size_t j = 0; j != lhs.width(); ++j) {
      if (lhs[i][j] != rhs[i][j]) {
        return false;
      }
    }
  }
  return true;
}

bool operator!=(const Matrix& lhs, const Matrix& rhs) {
  return !(lhs == rhs);
}


Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
  if (lhs.width() != rhs.height()) {
    std::stringstream ss;
    ss << "Cannot multiply matrix with shape "
       << lhs.shape()
       << " to matrix with shape "
       << rhs.shape();
    throw std::runtime_error(ss.str());
  }
  Matrix res(lhs.height(), rhs.width());
  for (size_t i = 0; i != res.height(); ++i) {
    for (size_t j = 0; j != res.width(); ++j) {
      res[i][j] = 0;
      for (size_t k = 0; k != lhs.width(); ++k) {
        res[i][j] += lhs[i][k] * rhs[k][j];
      }
    }
  }
  return res;
}

