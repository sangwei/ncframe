#ifndef _NCFRAME_H_
#define _NCFRAME_H_

#include "ncf_win.h"
#include "ncf_ctl.h"

class ncframe {
public:
    ncframe() : cur_ctl_(nullptr) {
        // init ncurses
        init_nc();
    };
    virtual ~ncframe() {
        // end ncurses
        end_nc();
        // delete all controllers
        for (auto it = ctls_.begin(), e = ctls_.end(); it != e; it ++) {
            delete it->second;
        }
        ctls_.clear();
    };
    template <typename ctl_t>
    ctl_t* create_ctl(const std::string& name,
        std::vector<typename ctl_t::win_type::buf_type>&& init_data) {
        if (ctls_.find(name) == ctls_.end()) {
            typedef typename ctl_t::win_type ncf_win_type;
            auto ctl = new ctl_t(ncf_win_type(std::move(init_data)));
            // initialize new controller
            ctl->init();
            ctls_[name] = ctl;
            return ctl;
        } else {
            return static_cast<ctl_t*>(ctls_[name]);
        }
    };
    template <typename ctl_t>
    int start(const std::string& name,
        std::vector<typename ctl_t::win_type::buf_type>&& init_data) {
        // create the first controller
        auto ctl = create_ctl<ctl_t>(name, std::move(init_data));
        // set current controller
        set_current(ctl);
        while (int c = getch()) {
            if (c == 'q') {
                break;
            }
            current()->on_key(c);
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
    void set_current(ncf_ctl_base* ctl) {
        cur_ctl_ = ctl;
    };
    ncf_ctl_base* current() {
        return cur_ctl_;
    };

private:
    std::map<std::string, ncf_ctl_base*> ctls_;
    ncf_ctl_base* cur_ctl_;
};

#endif
