#include "ncframe.h"

int ncframe::init_nc() {
    // start curses mode
    initscr();
    cbreak();
    // no keyboard echo
    noecho();
    // get keyboard input
    keypad(stdscr, TRUE);
    refresh();
    return 0;
}

int ncframe::end_nc() {
    endwin();
    return 0;
}
