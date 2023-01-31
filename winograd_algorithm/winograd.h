#ifndef SRC_WINOGRAD_ALG_WINOGRAD_H_
#define SRC_WINOGRAD_ALG_WINOGRAD_H_

#include <functional>
#include <mutex>
#include <thread>
#include <vector>

#include "../algorithm_result/algorithm_result.h"
#include "../exceptions/exceptions.h"
#include "../matrix.h"
#include "../timer/timer.h"

namespace s21 {

class Winograd {
 public:
  using solution_type = Matrix;
  using result_type = AlgorithmResult<solution_type>;

 public:
  result_type SolveSequently(const int iterations_quantity);
  result_type SolveInParallel(const int iterations_quantity);
  result_type SolveUsingConveyor(const int iterations_quantity);
  void SetupMatrixesForMultiplication(const Matrix &first_matrix,
                                      const Matrix &second_matrix);
  void SetThreadsQuantity(int number);

 private:
  Matrix matrix_A_;
  Matrix matrix_B_;
  Matrix matrix_R_;   // result matrix
  Matrix matrix_RP_;  // result matrix parallel
  Matrix matrix_RC_;  // result matrix conveyor
  std::mutex mtx;
  int threads_quantity_ = 2;

  result_type sequential_result_;
  result_type parallel_result_;
  result_type conveyor_result_;

 private:
  void MatrixCheck();
  void ResultMatrixSetup(Matrix *matrix, int rows, int cols);
  void Solve(std::function<void()> calculation_method,
             const int iterations_quantity, result_type *result);
  int GetThreadsQuantity();

  void WinogradCalculation();
  void WinogradCalculationParallel();
  void WinogradCalculationConveyor();

  void RowFactorCalculation(double *row_factor);
  void ColumnFactorCalculation(double *column_factor);
  void Calculate(const double *row_factor, const double *column_factor,
                 int n_start, int n_end, Matrix *matrix);
};

}  // namespace s21

#endif  // SRC_WINOGRAD_ALG_WINOGRAD_H_
