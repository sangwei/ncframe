#ifndef _NCF_CTL_H_
#define _NCF_CTL_H_

#include "ncf_win.h"
#include <map>
#include <string>

namespace ncf {

class ncf_ctl {
public:
    ncf_ctl() : cur_win_(nullptr) {};
    virtual ~ncf_ctl()
    {
        // release all sub windows
        for (auto it : mwin_) {
            delete it.second;
        }
    }
    // create sub window of this controller
    template <typename win_t>
    win_t *create_win(const char *name, const char *title, int h = 0, int w = 0, int y = 0, int x = 0)
    {
        return create_win<win_t>(name, ncfwi(title, h, w, y, x));
    }
    // create sub window of this controller
    // using a struct as init parameter
    template <typename win_t>
    win_t *create_win(const char *name, const ncfwi &wi)
    {
        if (mwin_.find(name) != mwin_.end()) {
            return nullptr; // name conflict
        }
        win_t *win = new win_t(wi);
        mwin_[name] = win;
        // last created window is current window
        cur_win_ = win;
        return win;
    }
    void show()
    {
        for (auto it = mwin_.begin(); it != mwin_.end(); it++) {
            it->second->draw();
        }
    }
    virtual void on_key(int key)
    {
        if (cur_win_ != nullptr) {
            cur_win_->on_key(key);
        }
    }

protected:
    ncf_win *cur_win_;
    std::map<std::string, ncf_win *> mwin_;
};
}

#endif
