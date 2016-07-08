#include "ncfw_lines.h"
#include "ncf_app.h"

using namespace ncf;
using demo_win=ncfw_lines<>;

class demo_ctl : public ncf_ctl {
public:
    demo_win::notify_fn_t notify_fn() {
        return [this](demo_win* win, demo_win::notify_t type, std::string& param) {
            win->append("enter ok, new line added\n"); 
            win->draw();
        };
    }
};

int main() {
    ncf_app app;
    auto ctl = app.create_ctl<demo_ctl>("ctl");
    auto win = ctl->create_win<demo_win>("win", ncfwi());
    win->set_notify(ctl->notify_fn());
    win->set_lines([](){
        std::vector<std::string> lines;
        int size = 100;
        lines.reserve(size);
        char s[255] = {0};
        for (int i = 0; i < size; i ++) {
            sprintf(s, "%d\tPress q to exit.\n", i);
//            sprintf(s, "%d\tLet's get into more details of attributes. The functions attron(), attroff(), attrset() , and their sister functions attr_get() etc.. can be used to switch attributes on/off , get attributes and produce a colorful display.\n", i);
            lines.push_back(s);
        }
        return lines;
    }());
    app.run(ctl);
}
