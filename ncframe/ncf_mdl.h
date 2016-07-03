#ifndef _NCF_MDL_H_
#define _NCF_MDL_H_

#include <string>

template <typename base_t=std::string>
class ncf_mdl : public base_t {
public:
    ncf_mdl() {};
    virtual ~ncf_mdl() {};
    virtual const std::string& to_str() {
        return c_str();
    };

    base_t data;
};

#endif
