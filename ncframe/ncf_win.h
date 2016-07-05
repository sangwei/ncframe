#ifndef _NCF_WIN_H_
#define _NCF_WIN_H_

#include <ncurses.h>
#include <vector>

template <typename buf_t=std::string>
struct default_fmt {
    const char* operator()(buf_t buf) {
        return buf.c_str();
    };
};

template <typename buf_t=std::string, typename fmt_t=default_fmt<buf_t>>
class ncf_win {
public:
    std::vector<buf_t> buf;

    ncf_win() : pos_(0), win_(nullptr) {};
    ncf_win(std::vector<buf_t>&& data) : ncf_win() {
        buf = std::move(data);
    };
    ncf_win(ncf_win&& rhs) {
        buf = std::move(rhs.buf);
        pos_ = rhs.pos_;
        std::swap(win_, rhs.win_);
    };
    ncf_win& operator=(ncf_win&& rhs) {
        buf = std::move(rhs.buf);
        pos_ = rhs.pos_;
        std::swap(win_, rhs.win_);
        return (*this);
    };
    virtual ~ncf_win() {
        if (win_ != nullptr) {
            delwin(win_);
        }
    };
    WINDOW* get_win() {
        return win_;
    };
    virtual int init() {
        win_ = newwin(0, 0, 0, 0);
        return 0;
    };
    virtual int roll(int num) {
        pos_ = std::max(pos_ + num, 0);
        int size = buf.size() - 1;
        pos_ = std::min(pos_, size);
        return 0;
    };
    virtual int draw() {
        // get max row and col number
        int maxy, maxx;
        getmaxyx(win_, maxy, maxx);
        // backup cursor postion
        int cury = getcury(win_);
        int curx = getcurx(win_);
        // clear window
        wclear(win_);
        for (auto it = buf.begin() + pos_, e = buf.end(); it < e; it ++) {
            if (getcury(win_) <= maxy - 1) {
                waddstr(win_, fmt(*it));
            } else {
                break;
            }
        }
        // clear last line
        wmove(win_, maxy - 1, 0);
        wclrtoeol(win_);
        // move cursor back
        wmove(win_, cury, curx);
        wrefresh(win_);
        return 0;
    };

protected:
    WINDOW* win_;
    fmt_t fmt;
    int pos_;
};

#endif
