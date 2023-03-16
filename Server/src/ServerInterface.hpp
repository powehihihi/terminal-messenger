#ifndef SERVER_INTERFACE_HPP
#define SERVER_INTERFACE_HPP
#include "../Client/MessagingStructure.hpp"

class ServerInterface {
 private:
 public:
  std::vector<ChatId> getAllChatsOfUser(UserId);
  Dialogue getDialogue(ChatId);
  void newMessage(ChatId, Message);
};

#endif
