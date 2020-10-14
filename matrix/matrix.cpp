#include "matrix.h"


Matrix::Matrix(const std::vector<std::vector<double>>& data) {
  if (data.empty() || data[0].empty()) {
    throw std::runtime_error("cannot construct empty matrix");
  }
  size_t rows = data.size();
  size_t columns = data[0].size();
  columns_ = columns;
  data_.reserve(rows * columns);
  for (size_t i = 0; i != rows; ++i) {
    if (data[i].size() != columns) {
      throw std::runtime_error("different row size");
    }
    for (size_t j = 0; j != columns; ++j) {
      data_.push_back(data[i][j]);
    }
  }
}


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

