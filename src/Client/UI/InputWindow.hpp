#ifndef INPUT_WINDOW_HPP
#define INPUT_WINDOW_HPP
#include <ncurses.h>
#include <string>
#include "utils.hpp"

class InputWindow {
 private:
  const int h, w, max_symbols;
  WINDOW * MainWindow;
  WINDOW * InnerWindow;
  int symbols;
 public:
  InputWindow(int h, int w, int starty, int startx);
  std::string Run();
 private:
  static int CountMaxSymbols(int x, int y);
};

#endif // !INPUT_WINDOW_HPP
