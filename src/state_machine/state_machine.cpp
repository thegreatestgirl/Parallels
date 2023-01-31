#include "state_machine.h"

using s21::StateMachine;
using s21::States;

StateMachine::StateMachine() { state_ = States::kStart; }

StateMachine::StateMachine(const States& state) { state_ = state; }

StateMachine::StateMachine(const_reference other) { *this = other; }

StateMachine::StateMachine(StateMachine&& other) { *this = std::move(other); }

typename StateMachine::reference StateMachine::operator=(
    const_reference other) {
  state_ = other.state_;
  return *this;
}

typename StateMachine::reference StateMachine::operator=(StateMachine&& other) {
  if (this != &other) {
    state_ = other.state_;
    other.state_ = States();
  }
  return *this;
}

void StateMachine::ToStart() { state_ = States::kStart; }

void StateMachine::SetState(const States& state) { state_ = state; }

States StateMachine::GetState() const { return state_; }

void StateMachine::ToFinish() { state_ = States::kFinish; }

bool StateMachine::IsFinished() const { return state_ == States::kFinish; }
