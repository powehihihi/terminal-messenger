#include "MessagingStructure.hpp"

void Dialogue::Post(Message m) {
  Messages.push_back(m);
}

ChatId Dialogue::id() {
  return id_;
}
const std::vector<Message>& Dialogue::GetMessages() {
  return Messages;
}
