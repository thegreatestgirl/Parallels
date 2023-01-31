#include "matrix.h"

using s21::Matrix;

Matrix::Matrix()
    : rows_(kDefaultRows),
      columns_(kDefaultColumns),
      values_(nullptr),
      cached_values_(nullptr) {}

Matrix::Matrix(size_type rows, size_type columns)
    : rows_(rows), columns_(columns) {
  AllocateValues();
}

Matrix::Matrix(size_type rows) : rows_(rows), columns_(rows) {
  AllocateValues();
}

Matrix::Matrix(size_type rows, size_type columns, value_type default_value)
    : rows_(rows), columns_(rows) {
  AllocateValues();
  for (size_type i = 0; i < rows_; ++i) {
    for (size_type j = 0; j < columns_; ++j) {
      values_[i][j] = default_value;
    }
  }
}

Matrix::Matrix(const_reference other) { *this = other; }

Matrix::Matrix(rvalue other) { *this = std::move(other); }

Matrix::~Matrix() { DeleteValues(); }

typename Matrix::reference Matrix::operator=(const_reference other) {
  if (this != &other) {
    Copy(other);
  }
  return *this;
}

typename Matrix::reference Matrix::operator=(rvalue other) {
  if (this != &other) {
    rows_ = other.rows_;
    columns_ = other.columns_;
    values_ = other.values_;
    cached_values_ = other.cached_values_;
    other.rows_ = kDefaultRows;
    other.columns_ = kDefaultColumns;
    other.values_ = nullptr;
    other.cached_values_ = nullptr;
  }
  return *this;
}

void Matrix::AllocateValues() {
  if (rows_ == 0 || columns_ == 0) {
    throw InvalidMatrixSizes();
  }
  values_ = new value_type*[rows_];
  cached_values_ = new value_type*[rows_];
  for (size_type i = 0; i < rows_; i++) {
    values_[i] = new value_type[columns_]{};
    cached_values_[i] = new value_type[columns_]{};
  }
}

void Matrix::DeleteValues() {
  if (values_) {
    for (size_type i = 0; i < rows_; ++i) {
      delete[] values_[i];
    }
    delete[] values_;
    values_ = nullptr;
  }
}

void Matrix::Copy(const_reference other) {
  rows_ = other.rows_;
  columns_ = other.columns_;
  AllocateValues();
  CopyMatrixValues(other);
}

void Matrix::CopyMatrixValues(const_reference src) {
  for (size_type n = 0; n < rows_ * columns_; n++) {
    size_type i = n / columns_, j = n % columns_;
    values_[i][j] = src.values_[i][j];
    cached_values_[i][j] = src.cached_values_[i][j];
  }
}

typename Matrix::value_type& Matrix::operator()(const size_type i,
                                                const size_type j) {
  return values_[i][j];
}

typename Matrix::value_type& Matrix::operator()(const size_type i,
                                                const size_type j) const {
  return values_[i][j];
}

typename Matrix::size_type Matrix::GetRows() const { return rows_; }

typename Matrix::size_type Matrix::GetColumns() const { return columns_; }

void Matrix::Resize(const size_type rows, const size_type columns) {
  DeleteValues();
  rows_ = rows;
  columns_ = columns;
  AllocateValues();
}

void Matrix::Clear() {
  DeleteValues();
  rows_ = kDefaultRows;
  columns_ = kDefaultColumns;
}

void Matrix::InputValues() {
  for (size_type i = 0; i < rows_; ++i) {
    for (size_type j = 0; j < columns_; ++j) {
      std::string input_string;
      std::cin >> input_string;
      try {
        double value = std::stod(input_string);
        values_[i][j] = value;
      } catch (...) {
        std::cout << "Invalid matrix value" << std::endl;
        --j;
      }
    }
  }
  CacheValues();
}

void Matrix::SetValue(const size_type i, const size_type j,
                      const value_type value) {
  values_[i][j] = value;
}

void Matrix::Randomize() {
  for (size_type i = 0; i < rows_; ++i) {
    for (size_type j = 0; j < columns_; ++j) {
      values_[i][j] = GenerateRandomNumber();
    }
  }
}

double Matrix::GenerateRandomNumber() {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 100);
  return static_cast<double>(dist6(rng));
}

Matrix Matrix::Multiply(const Matrix& other) {
  Matrix res(rows_, other.columns_);

  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < other.columns_; ++j) {
      for (int k = 0; k < columns_; ++k) {
        res.values_[i][j] += values_[i][k] * other.values_[k][j];
      }
    }
  }
  return res;
}

bool Matrix::isEqual(const Matrix& other) {
  bool is_equal = true;
  if (rows_ == other.rows_ && columns_ == other.columns_) {
    for (int n = 0; is_equal && n < rows_ * columns_; n++) {
      int i = n / columns_, j = n % columns_;
      if (fabs(values_[i][j] - other.values_[i][j]) > kPrecision) {
        is_equal = false;
      }
    }
  } else {
    is_equal = false;
  }
  return is_equal;
}

void Matrix::Print() {
  for (size_type i = 0; i < rows_; ++i) {
    for (size_type j = 0; j < columns_; ++j) {
      std::printf("%8.2lf ", values_[i][j]);
    }
    std::cout << std::endl;
  }
}

void Matrix::CacheValues() {
  for (size_type i = 0; i < rows_; ++i) {
    for (size_type j = 0; j < columns_; ++j) {
      cached_values_[i][j] = values_[i][j];
    }
  }
}

void Matrix::GetValuesFromCache() {
  for (size_type i = 0; i < rows_; ++i) {
    for (size_type j = 0; j < columns_; ++j) {
      values_[i][j] = cached_values_[i][j];
    }
  }
}
