#ifndef _NCF_CTL_H_
#define _NCF_CTL_H_

#include "ncf_win.h"

class ncf_ctl {
public:
    ncf_ctl() {};
    virtual ~ncf_ctl() {};

    virtual int show();

protected:
    ncf_win* win_;
};

#endif
