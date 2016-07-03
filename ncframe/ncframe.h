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
        // init controller
        pctl->init();
        // set current controller
        ncf_ctl_mgr::inst()->set_current(pctl);
        while (int c = getch()) {
            if (c == 'q') {
                break;
            }
            switch (c) {
            case 'k':
            case KEY_UP:
                ncf_ctl_mgr::inst()->current()->key_up();
                break;
            case 'j':
            case KEY_DOWN:
                ncf_ctl_mgr::inst()->current()->key_down();
                break;
            }
        }

        return 0;
    };

protected:
    virtual int init_nc();
    virtual int end_nc();
};

#endif
