#ifndef _NCFW_LINES_H_
#define _NCFW_LINES_H_

#include "ncf_win.h"
#include <functional>
#include <vector>

namespace ncf {

template <typename line_t = std::string> struct ncfw_line_fmt {
    const char *operator()(line_t &line);
};

template <> struct ncfw_line_fmt<std::string> {
    const char *operator()(std::string &line)
    {
        return line.c_str();
    };
};

template <typename line_t = std::string, typename fmt_t = ncfw_line_fmt<line_t>>
class ncfw_lines_base : public ncf_win {
public:
    using iterator=typename std::vector<line_t>::iterator;

    ncfw_lines_base(const ncfwi &wi)
        : ncf_win(wi), notify_(nullptr), pos_(0), sel_(0), sel_underline_(false) {}

    // notify callback type
    enum notify_t { hit_row };
    using notify_fn_t = std::function<void(ncfw_lines_base<line_t, fmt_t> *,
                                           notify_t type, line_t &param)>;
    void set_notify(notify_fn_t &&fn)
    {
        notify_ = std::move(fn);
    }

    void set_sel_underline(bool v)
    {
        sel_underline_ = v;
    }
    void set_lines(std::vector<line_t> &&lines)
    {
        lines_ = std::move(lines);
        if (lines_.size() > 0) {
            pos_ = sel_ = 0;
        }
    }
    void append(const line_t &line)
    {
        lines_.push_back(line);
        if (lines_.size() == 1) {
            pos_ = sel_ = 0;
        }
    }
    void append(const std::vector<line_t> &lines)
    {
        lines_.insert(lines_.end(), lines.begin(), lines.end());
        if (lines_.size() == 1) {
            pos_ = sel_ = 0;
        }
    }
    iterator erase(const iterator pos)
    {
        iterator it = lines_.erase(pos);
        if (it == lines_.end()) {

        }
        if (pos_ >= lines_.size()) {
            pos_ = lines_.size() - 1;
        }
        if (sel_ >= lines_.size()) {
            sel_ = lines_.size() - 1;
        }
    }
    virtual void draw_sel(int pre, int cur)
    {
        if (lines_.size() == 0 || !sel_underline_) {
            return;
        }
        // get max row and col number
        int maxy, maxx;
        getmaxyx(win_, maxy, maxx);
        // backup cursor postion
        int cury = getcury(win_);
        int curx = getcurx(win_);
        // draw previous selected position
        wmove(win_, y_of_lines_[pre - pos_], 0);
        waddstr(win_, fmt_(lines_[pre]));
        // draw current selected position
        wmove(win_, y_of_lines_[cur - pos_], 0);
        wattron(win_, A_UNDERLINE);
        waddstr(win_, fmt_(lines_[cur]));
        wattroff(win_, A_UNDERLINE);
        // clear last line
        wmove(win_, maxy - 1, 0);
        wclrtoeol(win_);
        // move cursor back
        wmove(win_, cury, curx);
        wrefresh(win_);
    }
    virtual void draw()
    {
        if (lines_.size() == 0) {
            return;
        }
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
        for (int i = pos_, e = lines_.size(); i < e; i++) {
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
    virtual void on_key(int key)
    {
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
    // return true, if it really scrolled
    virtual bool roll(int num)
    {
        int pre = pos_;
        pos_ = std::max(pos_ + num, 0);
        int size = lines_.size() - 1;
        pos_ = std::min(pos_, size);
        if (pre != pos_) {
            return true;
        }
        return false;
    }
    virtual int row_up()
    {
        if (sel_ <= 0) {
            return 0;   // no row above here
        }
        if (sel_ > pos_) {
            // only move up selection
            sel_ --;
            draw_sel(sel_ + 1, sel_);
        } else {
            // top most line is selected
            // scroll up
            sel_ --;
            pos_ --;
            draw();
        }
        wmove(win_, y_of_lines_[sel_ - pos_], 0);
        wrefresh(win_);
        return 0;
    };

    virtual int row_down()
    {
        if (sel_ >= lines_.size() - 1) {
            return 0;   // no row below current selection
                        // just do nothing
        }
        if (sel_ + 1 - pos_ < y_of_lines_.size()) {
            // next selection still on current screen
            // only selection should be drawn again
            sel_ ++;
            draw_sel(sel_ - 1, sel_);
        } else {
            // move to next screen position
            pos_ ++;
            sel_ ++;
            draw();
            // size of y_of_lines, is the total number of lines
            // shown on current window
            // so the posible biggest sel_ is:
            //     pos_ + y_of_lines_.size() - 1
            if (sel_ > pos_ + y_of_lines_.size() - 1) {
                // redraw selection
                draw_sel(sel_, pos_ + y_of_lines_.size() - 1);
                sel_ = pos_ + y_of_lines_.size() - 1;
            }
        }
        wmove(win_, y_of_lines_[sel_ - pos_], 0);
        wrefresh(win_);
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

using ncfw_lines = ncfw_lines_base<>;

}

#endif
