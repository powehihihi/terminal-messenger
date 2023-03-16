#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <string>
#include <vector>

struct UserId {
  int id;
  explicit UserId(int id): id(id) {}
  explicit operator int() const {
    return id;
  }
};
struct ChatId {
  int id;
  explicit ChatId(int id): id(id) {}
  explicit operator int() const {
    return id;
  }
};

struct Message {
  const UserId user;
  const std::string text;
  Message(UserId user, std::string&& text): user(user), text(text) {}
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
