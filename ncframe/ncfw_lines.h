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
    ncfw_lines(const ncfwi& wi) : 
        ncf_win(wi), notify_(nullptr), pos_(-1), sel_(-1), sel_underline_(false) {}

    // notify callback type
    enum notify_t {
        hit_row
    };
    using notify_fn_t=std::function<void(ncfw_lines<line_t, fmt_t>*, 
        notify_t type, line_t& param)>;
    void set_notify(notify_fn_t&& fn) {
        notify_ = std::move(fn);
    }

    void set_sel_underline(bool v) {
        sel_underline_ = v;
    }

    void set_lines(std::vector<line_t>&& lines) {
        lines_ = std::move(lines);
        if (lines_.size() > 0) {
            pos_ = sel_ = 0;
        }
    }
    void append(const line_t& line) {
        lines_.push_back(line);    
        if (lines_.size() == 1) {
            pos_ = sel_ = 0;
        }
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
        // clear y record
        y_of_lines_.clear();
        for (int i = pos_, e = lines_.size(); i < e; i ++) {
            if (getcury(win_) < maxy - 1) {
                // starting row of current line
                y_of_lines_.push_back(getcury(win_));
                if (sel_ == i && sel_underline_) {
                    wattron(win_, A_UNDERLINE);
                    waddstr(win_, fmt_(lines_[i]));
                    wattroff(win_, A_UNDERLINE);
                } else {
                    waddstr(win_, fmt_(lines_[i]));
                }
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
            if (notify_ != nullptr && sel_ < lines_.size()) {
                notify_(this, notify_t::hit_row, lines_[sel_]);
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
        if (sel_ > pos_) {
            sel_ --;
        } else {
            roll(-1);
            sel_ = std::max(0, sel_ - 1);
        }
        draw();
        wmove(win_, y_of_lines_[sel_ - pos_], curx);
        return 0;
    };
    virtual int row_down() {
        int maxy, maxx;
        getmaxyx(win_, maxy, maxx);
        int cury = getcury(win_);
        int curx = getcurx(win_);

        if (sel_ >= 0 && sel_ + 1 < lines_.size() && 
            sel_ >= pos_ && (sel_ + 1 - pos_) < y_of_lines_.size() &&
            y_of_lines_[sel_ + 1 - pos_] < maxy - 1) {
            sel_ ++;    
        } else {
            roll(1);
            sel_ = std::min(sel_ + 1, (int)lines_.size() - 1);
        }
        wmove(win_, y_of_lines_[sel_ - pos_], curx);
        draw();
        return 0;
    };
private:
    std::vector<int> y_of_lines_;
    std::vector<line_t> lines_;
    // index of the first line shown on window
    int pos_;
    // index of the selected line
    int sel_;
    // is show underline when select
    bool sel_underline_;
    fmt_t fmt_;
    notify_fn_t notify_;
};

#endif
