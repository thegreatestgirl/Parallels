#include "ant_interface.h"

using s21::ant::Interface;

// Constructors
Interface::Interface() {
  facade_ = new ant::Facade();
  iterations_quantity_ = kDefaultIterationsQuantity;
  SetupHandlers();
  SetupCommoHandlers();
}

Interface::~Interface() { delete facade_; }

// Handlers
void Interface::GetSolutionHandler() {
  std::cout << "Start solving, it may take some time..." << std::endl;
  results_type results =
      facade_->GetSolution(input_matrix_, iterations_quantity_);
  PrintResults(results);
  state_machine_.SetState(s21::States::kChoseMenuOptionHandler);
}

void Interface::PrintSolution(const result_type& result,
                              const std::string& algorithm_type) {
  solution_type solution = result.GetSolution();
  std::cout << algorithm_type << " results:   ";
  std::cout << "Distance: " << solution.distance << std::endl;
  std::cout << "Path: ";
  for (const int value : solution.vertices) {
    std::cout << value << " ";
  }
  std::cout << std::endl;
}

// Methods
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
