#include "ant_facade.h"

using s21::ant::Facade;

// Constructors
Facade::Facade() { ant_algorithm_ = new AntAlgorithm(); }

Facade::~Facade() { delete ant_algorithm_; }

// Methods
s21::AlgorithmResults<typename Facade::result_type> Facade::GetSolution(
    const s21::Matrix& matrix, const int iterations_quantity) {
  AlgorithmResults<result_type> results;
  result_type sequential_result =
      ant_algorithm_->SolveTravelingSalesmanProblemSequently(
          matrix, iterations_quantity);
  result_type parallel_result =
      ant_algorithm_->SolveTravelingSalesmanProblemInParallel(
          matrix, iterations_quantity);
  results.sequential_result = sequential_result;
  results.parallel_result = parallel_result;
  return results;
}
