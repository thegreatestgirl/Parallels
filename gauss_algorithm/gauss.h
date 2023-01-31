#ifndef GAUSS_GAUSS_H_
#define GAUSS_GAUSS_H_

#include <array>
#include <chrono>
#include <functional>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

#include "../algorithm_result/algorithm_result.h"
#include "../exceptions/exceptions.h"
#include "../matrix.h"
#include "../timer/timer.h"

namespace s21 {

namespace gauss {

class Matrix : public s21::Matrix {
 public:
  using solution_type = std::vector<s21::Matrix::value_type>;
  using result_type = AlgorithmResult<solution_type>;
  using size_type = int;

 public:
  // Constructors
  Matrix();
  Matrix(const size_type rows, const size_type columns);
  Matrix(const s21::Matrix& matrix);

  // Methods
  result_type SolveSequentially(const int iterations_quantity);
  result_type SolveInParallel(const int iterations_quantity);

 private:
  std::mutex mutex_;
  result_type result_;

 private:
  result_type Solve(std::function<void()> forward_method,
                    std::function<void()> backward_method,
                    const int iterations_quantity);
  void Forward();
  void Backward();
  void ParallelForward();
  void ParallelBackward();

  void ProcessRows(const size_type first_row_index,
                   const size_type second_row_index);
  solution_type GetSolution();
  void DivideRow(const size_type row_index);
  void DivideEachElementOfRow(const size_type row_index, const double divisor);
  double* GetSubrow(const size_type base_row, const double multiplier);
  void SumRow(const int row_number, const double* subrow);
  void SwapRows(const size_type first_row_index,
                const size_type second_row_index);
  int FindRowToSwap(const value_type start_row);
  bool IsCorrectFirstRow();
  void ProcessFirstRow();
  bool IsCorrect();
};

}  // namespace gauss

}  // namespace s21

#endif  // GAUSS_GAUSS_H_
