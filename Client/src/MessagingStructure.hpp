#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <string>
#include <vector>

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
};

class Dialogue {
 private:
  ChatId id_;
  std::vector<Message> Messages;
  // std::mutex to protect data ?
 public:
  Dialogue(ChatId id, std::vector<Message>& Messages): id_(id), Messages(Messages) {}
  void Post(Message m); // ?
  ChatId id();
  const std::vector<Message>& GetMessages();
};

#endif // !MESSAGE_HPP
