#ifndef DIALOGUES_WINDOW_H
#define DIALOGUES_WINDOW_H

#include <algorithm>
#include <curses.h>
#include <ncurses.h>
#include <list>
#include <string>
#include <vector>
#include "../MessagingStructure.hpp"
#include "utils.hpp"


class DialoguePreview {
  ChatId id;
  const int h, w;
  bool isTop, isBot, chosen;
  WINDOW * win;
 public:
  DialoguePreview(ChatId id, const int h, const int w, int x, bool isTop=false, bool isBot=false);
  // DialoguePreview& operator=(const DialoguePreview&) = delete;
  void Unchoose();
  void Choose();
  void SetId(ChatId id);
  ChatId GetId();
  void UpdateText();
 private:
  void WriteText(std::string&& s);
};

class DialoguesWindow {
 private:
  std::vector<ChatId> Chats;
  std::vector<DialoguePreview> Previews;
  size_t topChatId=0, chosenPreview=0;
  // Window parameters
  int height, width;
  WINDOW * MainWindow;
  const int previews_height = 4;
 public:
  DialoguesWindow(const std::vector<ChatId>& Chats);
  void Up();
  void Down();
  ChatId Enter();
  void NewChat(ChatId new_id);
};

// old
/*
class DialoguesWindow {
 private:
  const int dialogue_height = 4;
  std::vector<ChatId> Chats;
  int height, width;
  WINDOW * MainWindow;
  std::vector<WINDOW *> Windows;
  int chosen;
 public:
  DialoguesWindow(std::vector<ChatId> Chats);

  void Choose(int i);
  void Up();
  void Down();
  void NewChat(ChatId id);
  ChatId Enter();
};
*/




#endif // !DIALOGUES_WINDOW_H
