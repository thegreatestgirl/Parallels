#ifndef INTERFACE_INTERFACE_H_
#define INTERFACE_INTERFACE_H_

#include <iostream>
#include <map>

#include "../matrix.h"
#include "../state_machine/state_machine.h"
#include "../algorithm_result/algorithm_result.h"

#include "../exceptions/exceptions.h"

namespace s21  {

constexpr int kDefaultIterationsQuantity = -1;
constexpr char kSequentialAlgorithmTypeName[] = "Sequential ";
constexpr char kParallelAlgorithmTypeName[] = "Parallel  ";
constexpr char kConveyorAlgorithmTypeName[] = "Conveyor  ";

enum MenuOptions: int {
   kRunAlgorithmAgain = 1,
   kQuit
};

template <class T>
class Interface {
 public:
   using handler_pointer = void (Interface::*)();
   using solution_type = T;
   using result_type = AlgorithmResult<solution_type>;
   using results_type = AlgorithmResults<result_type>; 

 public:
    void Execute();

 protected:
    Matrix input_matrix_;
    StateMachine state_machine_;
    int iterations_quantity_;
    std::map<States, handler_pointer> common_handlers_;

 protected:
    int InputIterationsQuantity();
    int InputNumber();
    bool IsDigit(const std::string str);
    void SetupCommoHandlers();
    std::pair<int, int> InputMatrixSizes();

    // Common handlers
    void InputIterationsQuantityHandler();
    void InputMatrixHandler();
    void MenuHandler();
    void ExecuteCommonHandler();

    void PrintResults(const results_type& results);
    void PrintAlgorithmResult(const result_type& results, const std::string& algorithm_name);
    void PrintTotalExecutionTime(const result_type& result, const std::string& algorithm_type);
    void PrintAverageTime(const result_type& result, const std::string& algorithm_type);

    virtual void GetSolutionHandler() = 0;
    virtual void PrintSolution(const result_type& result, const std::string& algorithm_type) = 0;
    virtual void SetupHandlers() = 0;
    virtual void ExecuteHandlerByCurrentState() = 0;
};

// Methods
template <typename T>
int Interface<T>::InputIterationsQuantity() {
    int iterations_quantity = InputNumber();
    if (iterations_quantity == 0) {
        throw s21::InvalidIterationsQuantity();
    }
    return iterations_quantity;
}

template <typename T>
void Interface<T>::Execute() {
    state_machine_.SetState(s21::States::kInputIterationQuantity);
    while (!state_machine_.IsFinished()) {
        try {
            ExecuteHandlerByCurrentState();
        } catch (const std::exception& exception) {
            std::cout << "> ERROR: " << exception.what() << std::endl;
            state_machine_.SetState(s21::States::kChoseMenuOptionHandler);
        }
    }
}

template <typename T>
int Interface<T>::InputNumber() {
    std::string input_string;
    while (true) {
        std::cin.clear();
        std::getline(std::cin, input_string);
        if (input_string.empty()) continue;
        if (IsDigit(input_string)) {
            return std::stoi(input_string);
        }
        std::cout << "> ERROR: Invalid input. You must input a number: ";
    }
}

template <typename T>
bool Interface<T>::IsDigit(const std::string str) {
    for (const char ch : str) {
        if (!std::isdigit(ch)) {
            return false;
        }
    }
    return true;
}

template <typename T>
void Interface<T>::SetupCommoHandlers() {
    common_handlers_.insert(std::make_pair(s21::States::kInputIterationQuantity, &s21::Interface<T>::InputIterationsQuantityHandler));
    common_handlers_.insert(std::make_pair(s21::States::kInputMatrix, &s21::Interface<T>::InputMatrixHandler));
    common_handlers_.insert(std::make_pair(s21::States::kChoseMenuOptionHandler, &s21::Interface<T>::MenuHandler));
}

template <typename T>
std::pair<int, int> Interface<T>::InputMatrixSizes() {
    std::string rows_str, columns_str;
    std::cin >> rows_str >> columns_str;
    if (!IsDigit(rows_str) || !IsDigit(columns_str)) {
        throw s21::MatrixInputFailure();
    }
    int rows = std::stoi(rows_str), columns = std::stoi(columns_str);
    if (rows == 0 || columns == 0) {
        throw s21::MatrixInputFailure();
    }
    return std::make_pair(rows, columns);
}

template <typename T>
void Interface<T>::InputMatrixHandler() {
    std::cout << "> Input matrix: " << std::endl;
    std::pair<int, int> matrix_sizes = InputMatrixSizes();
    input_matrix_.Resize(matrix_sizes.first, matrix_sizes.second);
    input_matrix_.InputValues();
    state_machine_.SetState(s21::States::kWaitSolution);
}

template <typename T>
void Interface<T>::MenuHandler() {
    std::cout << "> Menu:" << std::endl;
    std::cout << "> " << MenuOptions::kRunAlgorithmAgain<< ") Run algorithm again" << std::endl;
    std::cout << "> " << MenuOptions::kQuit<< ") Quit" << std::endl;
    while (true) {
        int menu_option = InputNumber();
        if (menu_option != MenuOptions::kQuit && menu_option != MenuOptions::kRunAlgorithmAgain) {
            std::cout << "> ERROR: invalid menu option. Try again" << std::endl;
        } else {
            if (menu_option == MenuOptions::kRunAlgorithmAgain) {
                state_machine_.SetState(s21::States::kInputIterationQuantity);
            } else {
                state_machine_.SetState(s21::States::kFinish);
            }
            return;
        }
    }
}

template <typename T>
void Interface<T>::ExecuteCommonHandler() {
    s21::States current_state = state_machine_.GetState();
    s21::Interface<T>::handler_pointer common_handler_ptr = common_handlers_[current_state];
    if (common_handler_ptr != nullptr) {
        (this->*common_handler_ptr)();
    }
}

template <typename T>
void Interface<T>::PrintResults(const results_type& results) {
   result_type sequential_results = results.sequential_result;
   result_type parallel_results = results.parallel_result;
   PrintAlgorithmResult(sequential_results, kSequentialAlgorithmTypeName);
   PrintAlgorithmResult(parallel_results, kParallelAlgorithmTypeName);
}

template <typename T>
void Interface<T>::PrintAlgorithmResult(const result_type& results, const std::string& algorithm_name) {
   PrintSolution(results, algorithm_name);
   PrintAverageTime(results, algorithm_name);
   PrintTotalExecutionTime(results, algorithm_name);
}

template <typename T>
void Interface<T>::PrintTotalExecutionTime(const result_type& result, const std::string& algorithm_type) {
   result.PrintTotalExecutionTime();
}

template <typename T>
void Interface<T>::PrintAverageTime(const result_type& result, const std::string& algorithm_type) {
   result.PrintAverageTime();
}

// Handlers
template <typename T>
void Interface<T>::InputIterationsQuantityHandler() {
    std::cout << "> Input iterations quantity: ";
    int iterations_quantity = InputIterationsQuantity();
    iterations_quantity_ = iterations_quantity;
    state_machine_.SetState(s21::States::kInputMatrix);
}

}  // namespace s21

#endif  // INTERFACE_INTERFACE_H_


