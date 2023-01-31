#include "winograd_interface.h"

using s21::winograd::Interface;

// Constructors
Interface::Interface() {
  facade_ = new winograd::Facade();
  SetupHandlers();
  SetupCommoHandlers();
  common_handlers_.erase(States::kInputMatrix);
}

Interface::~Interface() { delete facade_; }

// Handlers
void Interface::GetSolutionHandler() {
  std::cout << "Start solving, it may take some time..." << std::endl
            << std::endl;
  s21::winograd::Interface::results_type results = facade_->GetSolution(
      input_matrix_, second_matrix_, iterations_quantity_, threads_quantity_);
  std::cout << "First matrix: " << std::endl;
  input_matrix_.Print();
  std::cout << std::endl;
  std::cout << "Second matrix: " << std::endl;
  second_matrix_.Print();
  std::cout << std::endl;
  PrintSolution(results.sequential_result, kSequentialAlgorithmTypeName);
  PrintSolution(results.parallel_result, kParallelAlgorithmTypeName);
  PrintSolution(results.conveyor_results, kConveyorAlgorithmTypeName);
  state_machine_.SetState(States::kChoseMenuOptionHandler);
}

void Interface::GetMatrixFillingType() {
  std::cout << "> Chose matrix filling type:" << std::endl;
  std::cout << "> " << MatrixFillingTypes::kRandomly << ") randomly"
            << std::endl;
  std::cout << "> " << MatrixFillingTypes::kManualy << ") manually"
            << std::endl;
  while (true) {
    int matrix_filling_type = InputNumber();
    if (matrix_filling_type == MatrixFillingTypes::kManualy) {
      state_machine_.SetState(States::kInputFirstMatrix);
      return;
    } else if (matrix_filling_type == MatrixFillingTypes::kRandomly) {
      state_machine_.SetState(States::kInputFirstMatrixSizes);
      return;
    } else {
      std::cout << "> ERROR: Invalid matrix filling type. Try again: ";
    }
  }
}

void Interface::InputFirstMatrixHandler() {
  std::cout << "> Input first matrix: " << std::endl;
  std::pair<int, int> matrix_sizes = InputMatrixSizes();
  input_matrix_.Resize(matrix_sizes.first, matrix_sizes.second);
  input_matrix_.InputValues();
  state_machine_.SetState(s21::States::kInputSecondMatrix);
}

void Interface::InputSecondMatrixHandler() {
  std::cout << "> Input second matrix: " << std::endl;
  std::pair<int, int> matrix_sizes = InputMatrixSizes();
  second_matrix_.Resize(matrix_sizes.first, matrix_sizes.second);
  second_matrix_.InputValues();
  state_machine_.SetState(s21::States::kWaitSolution);
}

void Interface::InputFirstMatrixSizesHandler() {
  std::cout << "> Input first matrix sizes: " << std::endl;
  std::pair<int, int> matrix_sizes = InputMatrixSizes();
  input_matrix_.Resize(matrix_sizes.first, matrix_sizes.second);
  input_matrix_.Randomize();
  state_machine_.SetState(States::kInputSecondMatrixSizes);
}

void Interface::InputSecondMatrixSizesHandler() {
  std::cout << "> Input second matrix sizes: " << std::endl;
  std::pair<int, int> matrix_sizes = InputMatrixSizes();
  second_matrix_.Resize(matrix_sizes.first, matrix_sizes.second);
  second_matrix_.Randomize();
  state_machine_.SetState(States::kWaitSolution);
}

// Methods
void Interface::PrintSolution(const result_type& result,
                              const std::string& algorithm_type) {
  std::cout << algorithm_type << "results:" << std::endl;
  Matrix solution = result.GetSolution();
  solution.Print();
  PrintAverageTime(result, algorithm_type);
  PrintTotalExecutionTime(result, algorithm_type);
  std::cout << std::endl;
}

void Interface::InputThreadsQuantity() {
  std::cout << "> Input threads quantity (from 1 to "
            << (4 * std::thread::hardware_concurrency()) << "): ";
  int threads_quantity = InputNumber();
  if (threads_quantity > 4 * std::thread::hardware_concurrency()) {
    std::cout << "> Unavailible ammount of threads" << std::endl;
    return;
  }
  threads_quantity_ = threads_quantity;
  state_machine_.SetState(States::kGetMatrixFillingType);
}

void Interface::ExecuteHandlerByCurrentState() {
  s21::States current_state = state_machine_.GetState();
  handler_pointer handler = handlers_[current_state];
  if (handler != nullptr) {
    (this->*handler)();
    return;
  }
  ExecuteCommonHandler();
}

void Interface::SetupHandlers() {
  handlers_.insert(
      std::make_pair(States::kInputMatrix, &Interface::InputThreadsQuantity));
  handlers_.insert(std::make_pair(States::kGetMatrixFillingType,
                                  &Interface::GetMatrixFillingType));
  handlers_.insert(std::make_pair(States::kInputFirstMatrix,
                                  &Interface::InputFirstMatrixHandler));
  handlers_.insert(std::make_pair(States::kInputSecondMatrix,
                                  &Interface::InputSecondMatrixHandler));
  handlers_.insert(std::make_pair(States::kInputFirstMatrixSizes,
                                  &Interface::InputFirstMatrixSizesHandler));
  handlers_.insert(std::make_pair(States::kInputSecondMatrixSizes,
                                  &Interface::InputSecondMatrixSizesHandler));
  handlers_.insert(
      std::make_pair(States::kWaitSolution, &Interface::GetSolutionHandler));
}
