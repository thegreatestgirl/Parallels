#ifndef INTERFACES_GAUSS_GAUSS_INTERFACE_H_
#define INTERFACES_GAUSS_GAUSS_INTERFACE_H_

#include "../../facades/gauss/gauss_facade.h"
#include "../../matrix.h"
#include "../interface.h"

namespace s21 {

namespace gauss {

class Interface : public s21::Interface<std::vector<double>> {
 public:
  using handler_pointer = void (Interface::*)();
  using handlers_type = std::map<s21::States, handler_pointer>;
  using solution_type = std::vector<double>;
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
  Facade* facade_;

 private:
  // Handlers
  void GetSolutionHandler();

  // Methods
  void PrintSolution(const result_type& result,
                     const std::string& algorithm_type);
  void ExecuteHandlerByCurrentState();
  void SetupHandlers();
};

}  // namespace gauss

}  // namespace s21

#endif  // INTERFACES_GAUSS_GAUSS_INTERFACE_H_
