#include "InputWindow.hpp"
#include <curses.h>

InputWindow::InputWindow(int h, int w, int starty, int startx):
  h(h), w(w), max_symbols(CountMaxSymbols(h-1, w-1)),
  MainWindow(create_newwin(h, w,  starty, startx)),
  InnerWindow(newwin(h-2, w-2, starty+1, startx+1)) {

  }


std::string InputWindow::Run() {
  std::string res = "";
  //auto it = res.begin();
  //curs_set(1);
  for ( ; ; ) {
    int a = getch();
    switch (a) {
      case '\n':
        wclear(InnerWindow);
        wrefresh(InnerWindow);
        return res;
      case KEY_F(1):
        wclear(InnerWindow);
        wrefresh(InnerWindow);
        return "";
      case KEY_BACKSPACE:
        break;
      default:
        if (res.size()<max_symbols) {
          res.push_back(a);
          wprintw(InnerWindow, "%c", (char)a);
          wrefresh(InnerWindow);
        }
    }
  }
  return res;
}

int InputWindow::CountMaxSymbols(int x, int y) {
  int res = x*y;
  auto f = [](int x) {
    int res = 1;
    while (x>0) {
      ++res;
      x/=10;
    }
    return res;
  };

  while (x*y-2*f(x)-1>res)
    --res;
  return res;
}
