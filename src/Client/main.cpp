#include <cstddef>
#include <curses.h>
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>
#include "UI/ChatWindow.hpp"
#include "UI/DialoguesWindow.hpp"
#include "MessagingStructure.hpp"
#include "UI/InputWindow.hpp"

int main() {
  const int numOfChats = 12;
  std::vector<ChatId> AllChats;
  std::vector<std::vector<Message>> Dialogues;
  for (size_t i=0; i<numOfChats; ++i) {
    AllChats.emplace_back(i);
    std::vector<Message> tmpv {Message(UserId(i), "Hello! I'm user "+std::to_string(i)),
                               Message(UserId(i), "Nice to meet you.")};
    Dialogues.push_back(std::move(tmpv));
  }
  std::vector<std::shared_ptr<Dialogue>> Database (numOfChats, nullptr);
  for (size_t i=0; i<numOfChats; ++i)
    Database[i] = std::make_shared<Dialogue>(AllChats[i], Dialogues[i]);
  std::shared_ptr<Dialogue> dptr = Database[0];
  
	initscr();			/* Start curses mode 		*/
  noecho();
	cbreak();			/* Line buffering disabled, Pass on evertything to me 		*/
  curs_set(2);
	keypad(stdscr, TRUE);

	mvprintw(LINES-1, 1, "Press F1 to leave current mode");
  refresh();

  DialoguesWindow dw (AllChats);
  ChatWindow cw(dptr, LINES-7, (2*COLS)/3-1, COLS/3+1);
  InputWindow iw (5, (2*COLS)/3-1, LINES-7, COLS/3+1);
  for ( ; ; ) {
    int ch;
    for ( ; ; ) {
      ch = getch();
      switch (ch) {
        case KEY_DOWN:
          dw.Down();
          break;
        case KEY_UP:
          dw.Up();
          break;
        case '\n':
          auto id = dw.Enter();
          dptr = Database[id.id];
          cw.ChangeChat(dptr);
          break;
      }
      if (ch=='\n' || ch==KEY_F(1))
        break;
    }
    for ( ;ch!=KEY_F(1) ; ) {
      ch = getch();
      switch (ch) {
        case KEY_DOWN:
          cw.ScrollDown();
          break;
        case KEY_UP:
          cw.ScrollUp();
          break;
        case ' ':
          cw.ScrollToEnd();
          break;
        case '\n':
          auto str = iw.Run();
          while (!str.empty()) {
            dptr->Post(Message(UserId(-1), std::move(str)));
            cw.NewMessage();
            str = iw.Run();
          }
          break;
      }
    }
    if (ch==KEY_F(1))
      break;
  }
  endwin();
  return 0;
}
