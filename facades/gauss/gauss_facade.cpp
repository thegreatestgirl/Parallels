#include "gauss_facade.h"

using s21::gauss::Facade;

Facade::Facade() { matrix_ = nullptr; }

Facade::~Facade() { RemoveObject(); }

// Functions
void Facade::SetupMatrix(const s21::Matrix& matrix) {
  RemoveObject();
  CreateObject(matrix);
}

s21::AlgorithmResults<typename Facade::result_type> Facade::GetSolution(
    const int iterations_quantity) {
  AlgorithmResults<result_type> results;
  result_type sequential_result =
      matrix_->SolveSequentially(iterations_quantity);
  result_type parallel_result = matrix_->SolveInParallel(iterations_quantity);
  results.sequential_result = sequential_result;
  results.parallel_result = parallel_result;
  return results;
}

// Private functions
void Facade::CreateObject(const s21::Matrix& matrix) {
  matrix_ = new Matrix(matrix);
}

void Facade::RemoveObject() {
  if (matrix_ != nullptr) {
    delete matrix_;
    matrix_ = nullptr;
  }
}
