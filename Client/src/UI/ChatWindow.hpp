#ifndef CHAT_WINDOW_HPP
#define CHAT_WINDOW_HPP
#include <ncurses.h>
#include <vector>
#include <string>
#include <memory>
#include "utils.hpp"
#include "../MessagingStructure.hpp"


class ChatWindow {
 private:
  std::shared_ptr<Dialogue> dialogue;
  const int height, width, start_x, start_y=0, name_height = 3;
  WINDOW * MainWindow;
  WINDOW * ChatNameWindow;
  WINDOW * MessagesPad; // shoud be it's own type
  int pad_pos=0, pad_end=0;

 public:
  ChatWindow(std::shared_ptr<Dialogue> dialogue,
    int height, int width,
    int start_x);
  void ScrollUp();
  void ScrollDown();
  void ScrollToEnd();
  void NewMessage(); // prints last message to the pad 
                     // ChatWindow should be notified after every message
  void ChangeChat(std::shared_ptr<Dialogue>); // calls refresh
 private:
  void Refresh(); // reprint everything, called by constructor and ChangeChat
  void ScrollPad(); // called by other Scrolls
  void PrintMessage(const Message& m); // prints message to pad, called by Refresh and NewMessage 
};


#endif // !CHAT_WINDOW_HPP

