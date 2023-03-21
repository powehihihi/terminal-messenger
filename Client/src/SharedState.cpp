#include "SharedState.hpp"
#include "MessagingStructure.hpp"

void SharedState::PushIncome(const Message& msg) {
  income.push(msg);
}
void SharedState::PushOutcome(const Message& msg) {
  outcome.push(msg);
}
bool SharedState::PopIncome(Message& msg) {
  return income.pop(msg);
}
bool SharedState::PopOutcome(Message& msg) {
  return outcome.pop(msg);
}
void SharedState::PushError(boost::beast::error_code ec, const char * what) {
  errors.push({ec, what});
}
