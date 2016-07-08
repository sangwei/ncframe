#ifndef _NCFW_LINES_H_
#define _NCFW_LINES_H_

#include "ncf_win.h"
#include <vector>
#include <functional>

using namespace ncf;

template <typename line_t=std::string>
struct ncfw_line_fmt {
    const char* operator()(line_t& line);
};

template <>
struct ncfw_line_fmt<std::string> {
    const char* operator()(std::string& line) {
        return line.c_str();
    };
};

template <typename line_t=std::string, typename fmt_t=ncfw_line_fmt<line_t>>
class ncfw_lines : public ncf_win {
public:
    ncfw_lines(const ncfwi& wi) : ncf_win(wi), notify_(nullptr) {}

    // notify callback type
    enum notify_t {
        hit_row
    };
    using notify_fn_t=std::function<void(ncfw_lines<line_t, fmt_t>*, 
        notify_t type, line_t& param)>;
    void set_notify(notify_fn_t&& fn) {
        notify_ = std::move(fn);
    }

    void set_lines(std::vector<line_t>&& lines) {
        lines_ = std::move(lines);
    }
    void append(const line_t& line) {
        lines_.push_back(line);    
    }
    virtual void draw() {
        // get max row and col number
        int maxy, maxx;
        getmaxyx(win_, maxy, maxx);
        // backup cursor postion
        int cury = getcury(win_);
        int curx = getcurx(win_);
        // clear window
        wclear(win_);
        for (auto it = lines_.begin() + pos_, e = lines_.end(); it < e; it ++) {
            if (getcury(win_) <= maxy - 1) {
                waddstr(win_, fmt_(*it));
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
    }
    virtual void on_key(int key) {
        switch (key) {
        case 'k':
        case KEY_UP:
            row_up();
            break;
        case 'j':
        case KEY_DOWN:
            row_down();
            break;
        case 10:
            if (notify_ != nullptr) {
                notify_(this, notify_t::hit_row, lines_[pos_]);
            }
            break;
        default:
            break;
        }
    }
    virtual void roll(int num) {
        pos_ = std::max(pos_ + num, 0);
        int size = lines_.size() - 1;
        pos_ = std::min(pos_, size);
    }
    virtual int row_up() {
        int maxy, maxx;
        getmaxyx(win_, maxy, maxx);
        int cury = getcury(win_);
        int curx = getcurx(win_);
        if (cury > 0) {
            wmove(win_, cury - 1, curx);
            wrefresh(win_);
        } else {
            roll(-1);
            draw();
        }
        return 0;
    };
    virtual int row_down() {
        int maxy, maxx;
        getmaxyx(win_, maxy, maxx);
        int cury = getcury(win_);
        int curx = getcurx(win_);
        if (cury + 2 < maxy) {
            wmove(win_, cury + 1, curx);
            wrefresh(win_);
        } else {
            roll(1);
            draw();
        }
        return 0;
    };
private:
    std::vector<line_t> lines_;
    int pos_;
    fmt_t fmt_;
    notify_fn_t notify_;
};

#endif
