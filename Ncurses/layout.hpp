#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <ncurses.h>

void draw_borders(WINDOW *screen) {
  int x, y, i;

  getmaxyx(screen, y, x);

  // sides
  for (i = 1; i < (y - 1); i++) {
    mvwprintw(screen, i, 0, "|");
    mvwprintw(screen, i, x - 1, "|");
  }

  for (i = 1; i < (x-1); i++)
  {
      mvwprintw(screen, y-1, i, "-");
  }

}

#endif