#ifndef EXCEPTIONS_EXCEPTIONS_H_
#define EXCEPTIONS_EXCEPTIONS_H_

#include <exception>

namespace s21 {

class InvalidGaussMatrix : public std::exception {
 public:
  const char *what() const throw() {
    return "> Invalid Gauss matrix. The number of columns must be greater than "
           "the number of rows by 1";
  }
};

class InvalidMatrixInput : public std::exception {
 public:
  const char *what() const throw() {
    return "> Algorithm does not support this size of matrix";
  }
};

class InvalidIterationsQuantity : public std::exception {
 public:
  const char *what() const throw() { return "> Invalid iterations quantity"; }
};

class MatrixInputFailure : public std::exception {
 public:
  const char *what() const throw() { return "> Matrix input failure"; }
};

class InvalidMatrixSizes : public std::exception {
 public:
  const char *what() const throw() { return "> Invalid matrix sizes"; }
};

class DisconnectedGraph : public std::exception {
 public:
  const char *what() const throw() { return "> Disconnected graph"; }
};

}  // namespace s21

#endif  // EXCEPTIONS_EXCEPTIONS_H_
