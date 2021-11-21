#include "core.h"
#include <ncurses.h> /* for async key input + fancy terminal */
#include <stdio.h>
#include <time.h>

void init_core() {
  initscr();
  cbreak();
  nodelay(stdscr, TRUE);
  noecho();
}

void end_core() { endwin(); }

void print_time() {
  time_t now;
  struct tm *timeinfo;

  time(&now);
  timeinfo = localtime(&now);
  mvprintw(0, 0, asctime(timeinfo));
}

void print_window(const char *msg) { printw(msg); }

int get_async_key() { return getch(); }
