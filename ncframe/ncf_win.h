#ifndef _NCF_WIN_H_
#define _NCF_WIN_H_

#include <ncurses.h>

class ncf_win {
public:
    ncf_win() {};
    ncf_win(const ncf_win& rhs);
    ncf_win(const ncf_win&& rhs);
    ncf_win& operator=(const ncf_win& rhs);
    ncf_win& operator=(const ncf_win&& rhs);
    virtual ~ncf_win() {};
 
protected:
    WINDOW* win_;
};

#endif
