#ifndef FACADES_GAUSS_GAUSS_FACADE_H_
#define FACADES_GAUSS_GAUSS_FACADE_H_

#include "../../gauss_algorithm/gauss.h"

namespace s21 {

namespace gauss {

class Facade {
 public:
  using result_type = Matrix::result_type;

 public:
  // Constructors
  Facade();
  Facade(const Facade&) = delete;
  Facade(Facade&&) = delete;
  Facade& operator=(const Facade&) = delete;
  Facade& operator=(Facade&&) = delete;
  ~Facade();

  // Methods
  void SetupMatrix(const s21::Matrix& matrix);
  AlgorithmResults<result_type> GetSolution(const int iterations_quantity);

 private:
  Matrix* matrix_;

 private:
  void CreateObject(const s21::Matrix& matrix);
  void RemoveObject();
};

}  // namespace gauss

}  // namespace s21

#endif  // FACADES_GAUSS_GAUSS_FACADE_H_
