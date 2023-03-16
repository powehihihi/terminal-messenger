#include "ChatWindow.hpp"
#include "../MessagingStructure.hpp"
#include "utils.hpp"
#include <curses.h>
#include <string>

ChatWindow::ChatWindow(std::shared_ptr<Dialogue> dialogue,
  int height, int width,
  int start_x): dialogue(dialogue),
                    height(height),
                    width(width),
                    start_x(start_x),
                    MainWindow(create_newwin(height, width, start_y, start_x)),
                    ChatNameWindow(newwin(name_height, width, start_y, start_x)),
                    MessagesPad(newpad(10*height, width-2)) // pad height should be dynamic...
{
  wborder(ChatNameWindow, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LTEE, ACS_RTEE);
  Refresh();
}

void ChatWindow::Refresh() {
  std::string empty (width-2, ' ');
  mvwprintw(ChatNameWindow, 1, 1, "%s", empty.c_str());
  mvwprintw(ChatNameWindow, 1, 1, "Chat ID: %s", std::to_string(static_cast<int>(dialogue->id())).c_str());
  wrefresh(ChatNameWindow);
  
  wclear(MessagesPad);
  for (auto& m: dialogue->GetMessages())
    PrintMessage(m);
  /*
    wprintw(MessagesPad, "User: %s\n%s\n", 
            std::to_string(static_cast<int>(m.user)).c_str(), 
            m.text.c_str());
  */
  int cur_pos_x, cur_pos_y;
  getyx(MessagesPad, cur_pos_y, cur_pos_x);
  pad_end = cur_pos_y + 1;
  ScrollToEnd();
}

void ChatWindow::ScrollUp() {
  if (pad_pos==0)
    return;
  if (pad_pos==1)
    --pad_pos;
  else 
    pad_pos-=2;
  ScrollPad();
}
void ChatWindow::ScrollDown() {
  if (pad_pos>=pad_end-height+name_height)
    return;
  if (pad_pos+1==pad_end)
    ++pad_pos;
  else
    pad_pos+=2;
  ScrollPad();
}
void ChatWindow::ScrollToEnd() {
  if (pad_end>height-name_height)
    pad_pos = pad_end - height + name_height+1;
  ScrollPad();
}

void ChatWindow::NewMessage() {
  PrintMessage(dialogue->GetMessages().back());
  ScrollToEnd();
}

void ChatWindow::ChangeChat(std::shared_ptr<Dialogue> ptr) {
  dialogue = ptr;
  Refresh();
}
void ChatWindow::PrintMessage(const Message& m) {
  // mvcur(int, int, int, int);
  wprintw(MessagesPad, "  User %s:\n%s\n", 
          std::to_string(static_cast<int>(m.user)).c_str(), 
          m.text.c_str());
  int t;
  getyx(MessagesPad, pad_end, t);
  ++pad_end;
}

void ChatWindow::ScrollPad() {
  prefresh(MessagesPad, pad_pos, 0, start_y+name_height, start_x+1, height-name_height, width-2);
}

/*
ChatWindow::ChatWindow(Dialogue& d,
    int height, int width,
    int start_pos,
    int name_height, 
    int input_height): dialogue(d),
                       height(height), 
                       width(width), 
                       name_height(name_height), 
                       name_width(width),
                       input_height(input_height),
                       input_width(width),
                       MainWindow(create_newwin(height, width, 0, start_pos)) 
{
  ChatNameSubWindow = newwin(name_height, name_width, 0, start_pos);
  wborder(ChatNameSubWindow, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LTEE, ACS_RTEE);
  wrefresh(ChatNameSubWindow);
  MessagesPad = newpad(10*LINES, width-2); // temporary. pad size should be dynamic.
  InputSubWindow = newwin(input_height, input_width, height-input_height, start_pos);
  wborder(InputSubWindow, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_LTEE, ACS_RTEE, ACS_LLCORNER, ACS_LRCORNER);
  wrefresh(InputSubWindow);
  ShowChat(id, Messages);
}

void ChatWindow::ShowChat(Dialogue& d) {
  dialogue = d;
  mvwprintw(ChatNameSubWindow, 1, 1, "Chat ID: %s", std::to_string(static_cast<int>(id)).c_str());
  wrefresh(ChatNameSubWindow);
}
*/
