#include "gauss_interface.h"

using s21::gauss::Interface;

// Constructors
Interface::Interface() {
  facade_ = new s21::gauss::Facade();
  iterations_quantity_ = kDefaultIterationsQuantity;
  SetupHandlers();
  SetupCommoHandlers();
}

Interface::~Interface() { delete facade_; }

// Handlers
void Interface::GetSolutionHandler() {
  std::cout << "Start solving, it may take some time..." << std::endl;
  facade_->SetupMatrix(input_matrix_);
  results_type results = facade_->GetSolution(iterations_quantity_);
  PrintResults(results);
  state_machine_.SetState(s21::States::kChoseMenuOptionHandler);
}

// Private methods
void Interface::PrintSolution(const result_type& result,
                              const std::string& algorithm_type) {
  solution_type solution = result.GetSolution();
  std::cout << algorithm_type << " results:   ";
  for (const double value : solution) {
    std::cout << value << " ";
  }
  std::cout << std::endl;
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
  handlers_.insert(std::make_pair(s21::States::kWaitSolution,
                                  &Interface::GetSolutionHandler));
}
