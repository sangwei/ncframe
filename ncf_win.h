#ifndef _NCF_WIN_H_
#define _NCF_WIN_H_

#include <ncurses.h>

class ncf_win {
public:
    ncwin() {};
    ncwin(const ncwin& rhs);
    ncwin(const ncwin&& rhs);
    ncwin& operator=(const ncwin& rhs);
    ncwin& operator=(const ncwin&& rhs);
    virtual ~ncwin() {};
 
protected:
    WINDOW* win_;
};

#endif
