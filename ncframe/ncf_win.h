#ifndef _NCF_WIN_H_
#define _NCF_WIN_H_

#include <ncurses.h>
#include <string>

namespace ncf {

struct ncfwi {
    int h = 0;
    int w = 0;
    int y = 0;
    int x = 0;
    std::string title = "win";
    ncfwi() {}
    ncfwi(const char* title, int h, int w, int y, int x) :
        title(title), h(h), w(w), y(y), x(x) {}
};

class ncf_win {
public:
    ncf_win(const char* title, int h, int w, int y, int x) {
        title_ = title;
        win_ = newwin(h, w, y, x);
    }
    ncf_win(const ncfwi& wi) :
        ncf_win(wi.title.c_str(), wi.h, wi.w, wi.y, wi.x) {}
    ncf_win(const ncf_win& rhs) = delete;
    ncf_win(ncf_win&& rhs) = delete;
    ncf_win& operator=(const ncf_win& rhs) = delete;
    ncf_win& operator=(ncf_win&& rhs) = delete;
    WINDOW* ncwin() { return win_; }
    virtual ~ncf_win() { delwin(win_); }
    virtual void draw() { 
        wclear(win_);
        waddstr(win_, title_.c_str()); 
        wrefresh(win_);
    }
    virtual void on_key(int key) {}

protected:
    std::string title_;
    WINDOW* win_;
};

}

#endif
