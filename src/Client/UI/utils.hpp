#ifndef UTILS_HPP
#define UTILS_HPP
#include <ncurses.h>


WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
#endif
