#ifndef WINOGRAG_INTERFACE_WINOGRAG_INTERFACE_H_
#define WINOGRAG_INTERFACE_WINOGRAG_INTERFACE_H_

#include "../../facades/winograd/winograd_facade.h"
#include "../../matrix.h"
#include "../interface.h"

namespace s21 {

enum MatrixFillingTypes : int { kRandomly = 1, kManualy };

namespace winograd {

class Interface : public s21::Interface<Matrix> {
 public:
  using handler_pointer = void (Interface::*)();
  using handlers_type = std::map<s21::States, handler_pointer>;
  using solution_type = Matrix;
  using result_type = AlgorithmResult<solution_type>;
  using results_type = AlgorithmResults<result_type>;

 public:
  Interface();
  Interface(const Interface&) = delete;
  Interface(Interface&&) = delete;
  Interface& operator=(const Interface&) = delete;
  Interface& operator=(Interface&&) = delete;
  ~Interface();

 private:
  handlers_type handlers_;
  winograd::Facade* facade_;
  Matrix second_matrix_;
  int threads_quantity_;

 private:
  // Handlers
  void GetSolutionHandler();
  void GetMatrixFillingType();
  void InputFirstMatrixSizesHandler();
  void InputSecondMatrixSizesHandler();
  void InputFirstMatrixHandler();
  void InputSecondMatrixHandler();
  void InputThreadsQuantity();

  // Methods
  void PrintSolution(const result_type& result,
                     const std::string& algorithm_type);
  void ExecuteHandlerByCurrentState();
  void SetupHandlers();
};

}  // namespace winograd

}  // namespace s21

#endif  // WINOGRAG_INTERFACE_WINOGRAG_INTERFACE_H_
