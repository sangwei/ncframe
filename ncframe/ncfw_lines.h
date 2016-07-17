#ifndef _NCFW_LINES_H_
#define _NCFW_LINES_H_

#include "ncf_win.h"
#include <functional>
#include <vector>

namespace ncf {

// line formatter template
template <typename line_t = std::string> struct ncfw_line_fmt {};

// default line formatter for string line
template <> struct ncfw_line_fmt<std::string> {
    const char *operator()(std::string &line)
    {
        return line.c_str();
    }
};

// Multi line window, like readonly vi window.
// Default line data is string, but also can be other struct,
// as long as format method being provided
template <typename line_t = std::string, typename fmt_t = ncfw_line_fmt<line_t>>
class ncfw_lines_base : public ncf_win {
public:
    using iterator=typename std::vector<line_t>::iterator;
    using const_iterator=const typename std::vector<line_t>::iterator;

    ncfw_lines_base(const ncfwi &wi)
        : ncf_win(wi), sel_underline_(false), pos_(0), sel_(0), notify_(nullptr) {}

    // notify callback type
    enum notify_t { hit_row };
    using notify_fn_t = std::function<void(ncfw_lines_base<line_t, fmt_t> *,
                                           notify_t type, line_t &param)>;
    // append one line to the bottom
    void append(const line_t &line)
    {
        lines_.push_back(line);
    }
    // append lines to the bottom
    void append(const std::vector<line_t> &lines)
    {
        lines_.insert(lines_.end(), lines.begin(), lines.end());
    }
    // get line item by position
    const line_t& at(size_t pos)
    {
        return lines_[pos];
    }
    // calculate item's row
    size_t calc_height(const char* input, size_t width, size_t pos = 0)
    {
        wmove(win_, 0, 0);
        waddstr(win_, input);
        return getcury(win_) + 1;
    }
    // draw window
    virtual void draw()
    {
        // get max row and col number
        int maxy, maxx;
        getmaxyx(win_, maxy, maxx);
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
                if ( i != e - 1) {  // no next line at the end of lines
                    waddstr(win_, "\n");
                    // no need to add a '\n' at the
                    // end of line_t
                }
            } else {
                break;
            }
        }
        // clear last line
        wmove(win_, maxy - 1, 0);
        wclrtoeol(win_);
        // move cursor to selected pos
        wmove(win_, y_of_lines_[sel_ - pos_], 0);
        wrefresh(win_);
    }
    // draw selection item
    virtual void draw_sel(int pre, int cur)
    {
        if (lines_.size() == 0 || !sel_underline_) {
            return;
        }
        // get max row and col number
        int maxy, maxx;
        getmaxyx(win_, maxy, maxx);
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
        // move cursor to selected pos
        wmove(win_, y_of_lines_[sel_ - pos_], 0);
        wrefresh(win_);
    }
    // erase line item by position
    void erase(size_t pos)
    {
        lines_.erase(lines_.begin() + pos);
        if (pos_ >= lines_.size()) {
            pos_ = lines_.size() - 1;
        }
        if (sel_ >= lines_.size()) {
            sel_ = lines_.size() - 1;
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
    // move to bottom one row
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
        return 0;
    };
    // move to the bottom row
    void row_to_bottom()
    {
        int maxy, maxx;
        getmaxyx(win_, maxy, maxx);
        // calculate height for every line from the bottom
        int total_height = 0;
        int pos = lines_.size();
        while (--pos >= 0) {
            total_height += calc_height(fmt_(lines_[pos]), maxx);
            if (total_height > maxy - 1) {
                ++pos;
                break;
            } else if (total_height == maxy - 1) {
                break;
            }
        }
        pos_ = pos;
        sel_ = lines_.size() - 1;
        draw();
    }
    // move to top one row
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
        return 0;
    };
    // process key event
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
        case 'G':
            row_to_bottom();
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
    // replace current lines
    void set_lines(std::vector<line_t> &&lines)
    {
        lines_ = std::move(lines);
        pos_ = sel_ = 0;    // moving to the top line
    }
    // set notify callback function 
    void set_notify(notify_fn_t &&fn)
    {
        notify_ = std::move(fn);
    }
    // is the selected line underlined
    void set_sel_underline(bool val)
    {
        sel_underline_ = val;
    }
    // get current line's size
    size_t size()
    {
        return lines_.size();
    }

private:
    // is show underline when select
    bool sel_underline_;
    // index of the first line shown on window
    int pos_;
    // index of the selected line
    int sel_;
    // line item formatter
    fmt_t fmt_;
    // notify callback function
    notify_fn_t notify_;
    // the starting line of line items that are shown on current screen
    std::vector<int> y_of_lines_;
    // buffer for all lines
    std::vector<line_t> lines_;
};

using ncfw_lines = ncfw_lines_base<>;

}

#endif
