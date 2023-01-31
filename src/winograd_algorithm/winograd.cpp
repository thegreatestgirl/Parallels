#include "winograd.h"

using s21::Timer;
using s21::Winograd;

void Winograd::Solve(std::function<void()> calculation_method,
                     const int iterations_quantity, result_type *result) {
  Timer total_time_timer;
  Timer average_time_timer;
  total_time_timer.Begin();
  for (int i = 0; i < iterations_quantity; ++i) {
    average_time_timer.Begin();
    calculation_method();
    s21::TimeRange timestamp = average_time_timer.Timestamp();
    result->AddTimestamp(timestamp);
  }
  s21::TimeRange timestamp = total_time_timer.Timestamp();
  result->SetTotalExecutionTime(timestamp);
}

typename Winograd::result_type Winograd::SolveSequently(
    const int iterations_quantity) {
  std::function<void()> calculation_method =
      std::bind(&Winograd::WinogradCalculation, this);
  Solve(calculation_method, iterations_quantity, &sequential_result_);
  sequential_result_.SetSolution(matrix_R_);
  return sequential_result_;
}

typename Winograd::result_type Winograd::SolveInParallel(
    const int iterations_quantity) {
  std::function<void()> calculation_method =
      std::bind(&Winograd::WinogradCalculationParallel, this);
  Solve(calculation_method, iterations_quantity, &parallel_result_);
  parallel_result_.SetSolution(matrix_RP_);
  return parallel_result_;
}

typename Winograd::result_type Winograd::SolveUsingConveyor(
    const int iterations_quantity) {
  std::function<void()> calculation_method =
      std::bind(&Winograd::WinogradCalculationConveyor, this);
  Solve(calculation_method, iterations_quantity, &conveyor_result_);
  conveyor_result_.SetSolution(matrix_RC_);
  return conveyor_result_;
}

void Winograd::SetupMatrixesForMultiplication(const Matrix &first_matrix,
                                              const Matrix &second_matrix) {
  matrix_A_ = first_matrix;
  matrix_B_ = second_matrix;
}

void Winograd::WinogradCalculation() {
  MatrixCheck();
  ResultMatrixSetup(&matrix_R_, matrix_A_.GetRows(), matrix_B_.GetColumns());

  double *row_factor = new double[matrix_A_.GetRows()];
  double *column_factor = new double[matrix_B_.GetColumns()];

  RowFactorCalculation(row_factor);
  ColumnFactorCalculation(column_factor);
  Calculate(row_factor, column_factor, 0, matrix_R_.GetRows(), &matrix_R_);

  delete[] row_factor;
  delete[] column_factor;
}

void Winograd::SetThreadsQuantity(int number) { threads_quantity_ = number; }

int Winograd::GetThreadsQuantity() { return threads_quantity_; }

void Winograd::Calculate(const double *row_factor, const double *column_factor,
                         int n_start, int n_end, Matrix *matrix) {
  double result = 0;
  for (int i = n_start; i < n_end; ++i) {
    for (int j = 0; j < matrix->GetColumns(); ++j) {
      result = -row_factor[i] - column_factor[j];
      for (int k = 0; k < matrix_A_.GetColumns() / 2; ++k) {
        result += (matrix_A_(i, k << 1) + matrix_B_(k << 1 | 1, j)) *
                  (matrix_A_(i, k << 1 | 1) + matrix_B_(k << 1, j));
      }
      if (matrix_A_.GetColumns() % 2 != 0) {
        result += matrix_A_(i, matrix_A_.GetColumns() - 1) *
                  matrix_B_(matrix_A_.GetColumns() - 1, j);
      }
      mtx.lock();
      matrix->SetValue(i, j, result);
      mtx.unlock();
    }
  }
}

void Winograd::WinogradCalculationParallel() {
  MatrixCheck();
  ResultMatrixSetup(&matrix_RP_, matrix_A_.GetRows(), matrix_B_.GetColumns());

  double *row_factor = new double[matrix_A_.GetRows()];
  double *column_factor = new double[matrix_B_.GetColumns()];

  std::thread rowCounting(&Winograd::RowFactorCalculation, this, row_factor);
  std::thread columnCounting(&Winograd::ColumnFactorCalculation, this,
                             column_factor);

  rowCounting.join();
  columnCounting.join();

  std::vector<std::thread> threads;

  for (int i = 1; i <= GetThreadsQuantity(); ++i) {
    threads.push_back(std::thread(
        &Winograd::Calculate, this, row_factor, column_factor,
        (matrix_A_.GetRows() * (i - 1)) / GetThreadsQuantity(),
        (matrix_A_.GetRows() * i) / GetThreadsQuantity(), &matrix_RP_));
  }

  for (int i = 0; i < threads.size(); ++i) {
    if (threads[i].joinable()) {
      threads[i].join();
    }
  }

  delete[] row_factor;
  delete[] column_factor;
}

void Winograd::RowFactorCalculation(double *row_factor) {
  for (int i = 0; i < matrix_A_.GetRows(); ++i) {
    row_factor[i] = matrix_A_(i, 0) * matrix_A_(i, 1);
    for (int j = 1; j < matrix_A_.GetColumns() / 2; ++j) {
      row_factor[i] =
          row_factor[i] + matrix_A_(i, 2 * j) * matrix_A_(i, 2 * j + 1);
    }
  }
}

void Winograd::ColumnFactorCalculation(double *column_factor) {
  for (int i = 0; i < matrix_B_.GetColumns(); ++i) {
    column_factor[i] = matrix_B_(0, i) * matrix_B_(1, i);
    for (int j = 1; j < matrix_A_.GetColumns() / 2; ++j) {
      column_factor[i] =
          column_factor[i] + matrix_B_(2 * j, i) * matrix_B_(2 * j + 1, i);
    }
  }
}

void Winograd::MatrixCheck() {
  if (matrix_A_.GetColumns() != matrix_B_.GetRows())
    throw s21::InvalidMatrixInput();
  if (matrix_A_.GetRows() < 2 || matrix_A_.GetColumns() < 2 ||
      matrix_B_.GetRows() < 2 || matrix_B_.GetColumns() < 2)
    throw s21::InvalidMatrixInput();
}

void Winograd::ResultMatrixSetup(Matrix *matrix, int rows, int cols) {
  matrix->Resize(rows, cols);
}

void Winograd::WinogradCalculationConveyor() {
  MatrixCheck();
  ResultMatrixSetup(&matrix_RC_, matrix_A_.GetRows(), matrix_B_.GetColumns());

  double *row_factor = new double[matrix_A_.GetRows()];
  double *column_factor = new double[matrix_B_.GetColumns()];

  std::vector<std::thread> threads;

  threads.push_back(
      std::thread(&Winograd::RowFactorCalculation, this, row_factor));
  threads[0].join();
  threads.push_back(
      std::thread(&Winograd::ColumnFactorCalculation, this, column_factor));
  threads[1].join();

  for (int i = 1; i <= 2; ++i) {
    threads.push_back(std::thread(&Winograd::Calculate, this, row_factor,
                                  column_factor,
                                  (matrix_A_.GetRows() * (i - 1)) / 2,
                                  (matrix_A_.GetRows() * i) / 2, &matrix_RC_));
    threads[threads.size() - 1].join();
  }
}
