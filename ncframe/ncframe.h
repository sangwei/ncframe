#ifndef _NCFRAME_H_
#define _NCFRAME_H_

#include "ncf_win.h"
#include "ncf_ctl.h"

class ncframe {
public:
    ncframe() {
        // init ncurses
        init_nc();
    };
    virtual ~ncframe() {
        // end ncurses
        end_nc();
    };
    int start(ncf_ctl* pctl) {
        // controller manager
        ncf_ctl_mgr cmgr;
        // init controller
        pctl->init();
        // set current controller
        cmgr.set_current(pctl);
        while (int c = getch()) {
            if (c == 'q') {
                break;
            }
            switch (c) {
            case 'k':
            case KEY_UP:
                cmgr.current()->key_up();
                break;
            case 'j':
            case KEY_DOWN:
                cmgr.current()->key_down();
                break;
            }
        }

        return 0;
    };

protected:
    virtual int init_nc() {
        // start curses mode
        initscr();
        cbreak();
        // no keyboard echo
        noecho();
        // get keyboard input
        keypad(stdscr, TRUE);
        refresh();
        return 0;
    };
    virtual int end_nc() {
        endwin();
        return 0;
    };
};

#endif
