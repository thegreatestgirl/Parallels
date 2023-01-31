#ifndef STATE_MACHINE_STATE_MACHINE_H_
#define STATE_MACHINE_STATE_MACHINE_H_

#include <iostream>

namespace s21 {

enum class States : int {
  kStart = 0,
  kInputIterationQuantity,
  kInputMatrix,
  kGetMatrixFillingType,
  kInputFirstMatrix,
  kInputSecondMatrix,
  kInputFirstMatrixSizes,
  kInputSecondMatrixSizes,
  kChoseMenuOptionHandler,
  kInputThreadQuantity,
  kWaitSolution,
  kFinish,
};

class StateMachine {
 public:
  using reference = StateMachine&;
  using const_reference = const StateMachine&;

 public:
  // Constructors
  StateMachine();
  StateMachine(const States& state);
  StateMachine(const_reference other);
  StateMachine(StateMachine&& other);
  reference operator=(const_reference other);
  reference operator=(StateMachine&& other);

  // Functions
  void ToStart();
  void ToFinish();
  bool IsFinished() const;

  // Setters
  void SetState(const States& state);

  // Getters
  States GetState() const;

 private:
  States state_;
};

}  // namespace s21

#endif  // STATE_MACHINE_STATE_MACHINE_H_
