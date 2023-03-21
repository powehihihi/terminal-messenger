#ifndef SHARED_STATE_HPP
#define SHARED_STATE_HPP

#include "ThreadSafeQueue.hpp"
#include "MessagingStructure.hpp"
#include <boost/beast/core/error.hpp>
#include <utility>

class SharedState {
 private:
  ThreadSafe::Queue<Message> income, outcome;
  ThreadSafe::Queue<std::pair<boost::beast::error_code, const char *>> errors;
 public:
  SharedState() = default;
  
  void PushIncome(const Message&);
  void PushOutcome(const Message&);
  bool PopIncome(Message& msg);
  bool PopOutcome(Message& msg);
  void PushError(boost::beast::error_code, const char * what);
};
#endif
