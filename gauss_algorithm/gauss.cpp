#include "gauss.h"

using s21::gauss::Matrix;

// Constructors
Matrix::Matrix() {
  rows_ = 0;
  columns_ = 0;
  values_ = nullptr;
}

Matrix::Matrix(const size_type rows, const size_type columns) {
  rows_ = rows;
  columns_ = columns;
  AllocateValues();
}

Matrix::Matrix(const s21::Matrix& matrix) { Copy(matrix); }

// Functions
typename Matrix::result_type Matrix::SolveSequentially(
    const int iterations_quantity) {
  std::function<void()> forward_method = std::bind(&Matrix::Forward, this);
  std::function<void()> backward_method = std::bind(&Matrix::Backward, this);
  return Solve(forward_method, backward_method, iterations_quantity);
}

typename Matrix::result_type Matrix::SolveInParallel(
    const int iterations_quantity) {
  std::function<void()> forward_method =
      std::bind(&Matrix::ParallelForward, this);
  std::function<void()> backward_method =
      std::bind(&Matrix::ParallelBackward, this);
  return Solve(forward_method, backward_method, iterations_quantity);
}

typename Matrix::result_type Matrix::Solve(
    std::function<void()> forward_method, std::function<void()> backward_method,
    const int iterations_quantity) {
  if (!IsCorrect()) {
    throw s21::InvalidGaussMatrix();
  }
  result_.Reset();
  GetValuesFromCache();
  Timer total_time_timer;
  Timer average_time_timer;
  total_time_timer.Begin();
  for (int i = 0; i < iterations_quantity; ++i) {
    average_time_timer.Begin();
    forward_method();
    backward_method();
    s21::TimeRange timestamp = average_time_timer.Timestamp();
    result_.AddTimestamp(timestamp);
  }
  s21::TimeRange timestamp = total_time_timer.Timestamp();
  result_.SetTotalExecutionTime(timestamp);
  solution_type solution = GetSolution();
  result_.SetSolution(solution);
  return result_;
}

typename Matrix::solution_type Matrix::GetSolution() {
  solution_type solution;
  size_type j = columns_ - 1;
  for (size_type i = 0; i < rows_; ++i) {
    solution.push_back(values_[i][j]);
  }
  return solution;
}

// Private functions
void Matrix::Forward() {
  for (size_type current_row = 0; current_row < rows_ - 1; ++current_row) {
    DivideRow(current_row);
    for (size_type next_row = current_row + 1; next_row < rows_; ++next_row) {
      ProcessRows(current_row, next_row);
    }
  }
  double divisor = values_[rows_ - 1][columns_ - 2];
  DivideEachElementOfRow(rows_ - 1, divisor);
}

void Matrix::Backward() {
  for (size_type current_row = rows_ - 1; current_row > 0; --current_row) {
    DivideRow(current_row);
    for (size_type prev_row = current_row - 1; prev_row >= 0; --prev_row) {
      ProcessRows(current_row, prev_row);
    }
  }
  double divisor = values_[0][0];
  DivideEachElementOfRow(0, divisor);
}

void Matrix::ParallelForward() {
  for (size_type current_row = 0; current_row < rows_; ++current_row) {
    DivideRow(current_row);
    mutex_.unlock();
    size_type middle_row = (rows_ - current_row) / 2;
    std::thread t1([this, current_row, middle_row]() {
      for (size_type next_row = current_row + 1;
           next_row < current_row + middle_row; ++next_row) {
        ProcessRows(current_row, next_row);
      }
    });
    std::thread t2([this, current_row, middle_row]() {
      for (size_type next_row = current_row + middle_row; next_row < rows_;
           ++next_row) {
        ProcessRows(current_row, next_row);
      }
    });
    t1.join();
    t2.join();
    mutex_.lock();
  }
}

void Matrix::ParallelBackward() {
  for (size_type current_row = rows_ - 1; current_row >= 0; --current_row) {
    DivideRow(current_row);
    mutex_.unlock();
    size_type middle_row = rows_ - (rows_ / 2) - ((rows_ - current_row) / 2);
    std::thread t1([this, current_row, middle_row]() {
      for (size_type prev_row = current_row - 1;
           prev_row > current_row - middle_row; --prev_row) {
        ProcessRows(current_row, prev_row);
      }
    });
    std::thread t2([this, current_row, middle_row]() {
      for (size_type prev_row = current_row - middle_row; prev_row > -1;
           --prev_row) {
        ProcessRows(current_row, prev_row);
      }
    });
    t1.join();
    t2.join();
    mutex_.lock();
  }
  double divisor = values_[0][0];
  DivideEachElementOfRow(0, divisor);
}

void Matrix::ProcessRows(const size_type first_row_index,
                         const size_type second_row_index) {
  if (first_row_index == second_row_index) return;
  double multiplier = values_[second_row_index][first_row_index] * -1;
  double* subrow = GetSubrow(first_row_index, multiplier);
  SumRow(second_row_index, subrow);
  delete subrow;
}

void Matrix::DivideRow(const size_type row_index) {
  double divisor = values_[row_index][row_index];
  if (divisor != 0) {
    DivideEachElementOfRow(row_index, divisor);
  }
}

void Matrix::DivideEachElementOfRow(const size_type row_index,
                                    const double divisor) {
  if (divisor == 0 || divisor == -0) return;
  for (size_type i = 0; i < columns_; ++i) {
    values_[row_index][i] = values_[row_index][i] / divisor;
  }
}

double* Matrix::GetSubrow(const size_type base_row, const double multiplier) {
  double* subrow = new double[columns_];
  for (size_type i = 0; i < columns_; ++i) {
    subrow[i] = values_[base_row][i] * multiplier;
  }
  return subrow;
}

void Matrix::SumRow(const int row_number, const double* subrow) {
  for (size_type i = 0; i < columns_; ++i) {
    values_[row_number][i] = values_[row_number][i] + subrow[i];
  }
}

void Matrix::SwapRows(const size_type first_row_index,
                      const size_type second_row_index) {
  double* tmp = new double[columns_];
  for (size_type i = 0; i < columns_; ++i) {
    tmp[i] = values_[first_row_index][i];
  }
  for (size_type i = 0; i < columns_; ++i) {
    values_[first_row_index][i] = values_[second_row_index][i];
  }
  for (size_type i = 0; i < columns_; ++i) {
    values_[second_row_index][i] = tmp[i];
  }
  delete tmp;
}

int Matrix::FindRowToSwap(const value_type start_row) {
  for (size_type i = start_row; i < rows_; ++i) {
    if (values_[i][0] != 0) {
      return i;
    }
  }
  return -1;
}

bool Matrix::IsCorrectFirstRow() { return values_[0][0] != 0; }

void Matrix::ProcessFirstRow() {
  if (!IsCorrectFirstRow()) {
    int new_row = FindRowToSwap(1);
    if (new_row == -1) {
      throw s21::InvalidGaussMatrix();
    }
    SwapRows(0, new_row);
  }
}

bool Matrix::IsCorrect() { return columns_ - rows_ == 1; }
