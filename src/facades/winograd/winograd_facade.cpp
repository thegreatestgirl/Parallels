#include "winograd_facade.h"

using s21::winograd::Facade;

Facade::Facade() { winograd_ = new Winograd(); }

Facade::~Facade() { delete winograd_; }

s21::AlgorithmResults<typename Facade::result_type> Facade::GetSolution(
    const Matrix& lhs, const Matrix& rhs, const int iterations_quantity,
    const int threads_quantity) {
  winograd_->SetupMatrixesForMultiplication(lhs, rhs);
  winograd_->SetThreadsQuantity(threads_quantity);

  AlgorithmResults<result_type> results;

  result_type sequential_result =
      winograd_->SolveSequently(iterations_quantity);
  result_type parallel_result = winograd_->SolveInParallel(iterations_quantity);
  result_type conveyor_result =
      winograd_->SolveUsingConveyor(iterations_quantity);
  results.sequential_result = sequential_result;
  results.parallel_result = parallel_result;
  results.conveyor_results = conveyor_result;
  return results;
}
