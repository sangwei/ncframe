#ifndef _NCF_CTL_H_
#define _NCF_CTL_H_

#include "ncf_win.h"
#include <map>
#include <string>

namespace ncf {

class ncf_ctl {
public:
    template <typename win_t>
    win_t* create_win(const char* name, const char* title,
        int h, int w, int y, int x) {
        return create_win<win_t>(name, ncfwi(title, h, w, y, x));
    }
    template <typename win_t>
    win_t* create_win(const char* name, const ncfwi& wi) {
        if (mwin_.find(name) != mwin_.end()) {
            return nullptr; // name conflict
        }
        win_t* win = new win_t(wi);
        mwin_[name] = win;
        return win;
    }
    void show() {
        for (auto it = mwin_.begin(); it != mwin_.end(); it++) {
            it->second->draw();
        }
    }
    virtual void on_key(int key) {
        auto it = mwin_.begin();
        it->second->on_key(key);
    }
private:
    std::map<std::string, ncf_win*> mwin_;
};

}

#endif
