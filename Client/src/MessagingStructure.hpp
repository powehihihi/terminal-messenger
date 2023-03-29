#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <string>
#include <vector>
#include <stdexcept>

struct UserId {
  int id;
  UserId(): id(0) {}
  explicit UserId(int id): id(id) {}
  explicit operator int() const {
    return id;
  }
};
struct ChatId {
  int id;
  ChatId(): id(0) {}
  explicit ChatId(int id): id(id) {}
  explicit operator int() const {
    return id;
  }
};

struct Message {
  const UserId from;
  const ChatId to;
  const std::string text;
  Message() = default;
  Message(UserId from, ChatId to, std::string&& text): from(from), to(to), text(std::move(text)) {}
  
  static std::string MessageToString(const Message& msg) {
    return "f:" + std::to_string(static_cast<int>(msg.from)) +
           "t:" + std::to_string(static_cast<int>(msg.to)) +
           "m:" + msg.text;
  }

  static Message StringToMessage(const std::string& str) {
    int from, to;
    char buf[1234];
    if (sscanf(str.c_str(), "f:%dt:%dm:%s", &from, &to, buf)!=3)
      throw std::runtime_error("Wrong Message");
    return Message(UserId(from), ChatId(to), std::string(buf));
  }
};

class Dialogue {
 private:
  ChatId id_;
  std::vector<Message> Messages;
  // std::mutex to protect data ?
 public:
  Dialogue(ChatId id, std::vector<Message>& Messages): id_(id), Messages(Messages) {}
  void Post(Message m) {
    Messages.push_back(m);
  }
  ChatId id() {
    return id_;
  }
  const std::vector<Message>& GetMessages() {
    return Messages;
  }
};

#endif // !MESSAGE_HPP
