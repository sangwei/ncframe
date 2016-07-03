#ifndef _NCF_CTL_H_
#define _NCF_CTL_H_

#include "ncf_win.h"
#include <utility>
#include <map>
#include <string>
#include <ncurses.h>
#include <iostream>

class ncf_ctl;

class ncf_ctl_mgr : public std::map<std::string, ncf_ctl*> {
public:
    ncf_ctl_mgr() : cur_ctl_(nullptr) {};
    ~ncf_ctl_mgr() {};
    void set_current(ncf_ctl* ctl) {
        cur_ctl_ = ctl;
    };
    ncf_ctl* current() {
        return cur_ctl_;
    };
private:
    ncf_ctl* cur_ctl_;
};

class ncf_ctl {
public:
    ncf_ctl() : pwin_(nullptr) {};
    ncf_ctl(ncf_win<>* pwin) : pwin_(pwin) {};
    virtual ~ncf_ctl() {};
    virtual int init() {
        if (pwin_ != nullptr) {
            pwin_->init();
            pwin_->draw();
        }
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
    int key_up() {
        WINDOW* win = pwin_->get_win();
        int maxy, maxx;
        getmaxyx(win, maxy, maxx);
        int cury = getcury(win);
        int curx = getcurx(win);
        if (cury > 0) {
            wmove(win, cury - 1, curx);
            wrefresh(win);
        } else {
            pwin_->roll(-1);
            pwin_->draw();
        }
        return 0;
    };
    int key_down() {
        WINDOW* win = pwin_->get_win();
        int maxy, maxx;
        getmaxyx(win, maxy, maxx);
        int cury = getcury(win);
        int curx = getcurx(win);
        if (cury + 2 < maxy) {
            wmove(win, cury + 1, curx);
            wrefresh(win);
        } else {
            pwin_->roll(1);
            pwin_->draw();
        }
        return 0;
    };

protected:
    ncf_win<>* pwin_;
};

#endif
