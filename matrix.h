#ifndef MATRIX_H_
#define MATRIX_H_

#include <cmath>
#include <iostream>
#include <random>

#include "exceptions/exceptions.h"

namespace s21 {

constexpr unsigned int kDefaultRows = 0;
constexpr unsigned int kDefaultColumns = 0;
constexpr double kPrecision = 0.0000001;

class Matrix {
 public:
  using size_type = int;
  using value_type = double;
  using reference = Matrix&;
  using const_reference = const Matrix&;
  using rvalue = Matrix&&;
  using self_type = Matrix;

 public:
  // Constructors
  Matrix();
  Matrix(size_type rows, size_type columns);
  Matrix(size_type rows);
  Matrix(size_type rows, size_type columns, value_type default_value);
  Matrix(const_reference other);
  Matrix(rvalue other);
  ~Matrix();
  reference operator=(const_reference other);
  reference operator=(rvalue other);

  // Functions
  void Resize(const size_type rows, const size_type columns);
  void Clear();
  void Randomize();
  Matrix Multiply(const Matrix& other);
  bool isEqual(const Matrix& other);
  void Print();
  void InputValues();
  void SetValue(const size_type i, const size_type j, const value_type value);

  // Getters
  size_type GetRows() const;
  size_type GetColumns() const;

  // Overloads
  value_type& operator()(const size_type i, const size_type j);
  value_type& operator()(const size_type i, const size_type j) const;

 protected:
  size_type rows_, columns_;
  value_type** values_;
  value_type** cached_values_;

 protected:
  void AllocateValues();
  void DeleteValues();
  void Copy(const_reference other);
  void CopyMatrixValues(const_reference src);
  double GenerateRandomNumber();
  void CacheValues();
  void GetValuesFromCache();
};

}  // namespace s21

#endif  // SRC_MATRIX_H_
