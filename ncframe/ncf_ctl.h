#ifndef _NCF_CTL_H_
#define _NCF_CTL_H_

#include "ncf_win.h"
#include <map>
#include <string>

class ncf_ctl_base {
public:
    ncf_ctl_base() {};
    virtual ~ncf_ctl_base() {};
    virtual int init() = 0;
    virtual int on_key(int key) = 0;
    virtual int key_up() = 0;
    virtual int key_down() = 0;
};

template <typename ncfw_t>
class ncf_ctl : public ncf_ctl_base {
public:
    typedef ncfw_t win_type;

    ncf_ctl() {};
    ncf_ctl(ncfw_t&& win) : win_(std::move(win)) {};
    virtual ~ncf_ctl() {};
    virtual int init() {
        win_.init();
        win_.draw();
        return 0;
    };
    virtual int on_key(int key) {
        switch (key) {
        case 'k':
        case KEY_UP:
            key_up();
            break;
        case 'j':
        case KEY_DOWN:
            key_down();
            break;
        }
        return 0;
    };
    virtual int key_up() {
        WINDOW* win = win_.get_win();
        int maxy, maxx;
        getmaxyx(win, maxy, maxx);
        int cury = getcury(win);
        int curx = getcurx(win);
        if (cury > 0) {
            wmove(win, cury - 1, curx);
            wrefresh(win);
        } else {
            win_.roll(-1);
            win_.draw();
        }
        return 0;
    };
    virtual int key_down() {
        WINDOW* win = win_.get_win();
        int maxy, maxx;
        getmaxyx(win, maxy, maxx);
        int cury = getcury(win);
        int curx = getcurx(win);
        if (cury + 2 < maxy) {
            wmove(win, cury + 1, curx);
            wrefresh(win);
        } else {
            win_.roll(1);
            win_.draw();
        }
        return 0;
    };

protected:
    ncfw_t win_;
};

#endif
