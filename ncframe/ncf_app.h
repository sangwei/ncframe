#ifndef _NCF_APP_H_
#define _NCF_APP_H_

#include "ncf_ctl.h"
#include <ncurses.h>
#include <map>
#include <string>

namespace ncf {

class ncf_app {
public:
    ncf_app() { 
        initscr(); 
        noecho();
        // get keyboard input
        keypad(stdscr, TRUE);
        clear();
        refresh();
    }
    virtual ~ncf_app() { endwin(); }

    template <typename ctl_t, typename...Ts>
    ctl_t* create_ctl(const char* name, Ts...ts) {
        if (mctl_.find(name) != mctl_.end()) {
            return nullptr; // name conflict 
        }
        ctl_t* ctl = new ctl_t(ts...);
        mctl_[name] = ctl;
        return ctl;
    }

    virtual void run(ncf_ctl* ctl) {
        ctl->show();
        int ch;
        while (true) {
            ch = getch();
            if (ch == 'q') {
                break;
            }
            ctl->on_key(ch);
        }
    }

private:
    std::map<std::string, ncf_ctl*> mctl_;
};

}

#endif
