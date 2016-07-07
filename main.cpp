#include "ncfw_lines.h"
#include "ncf_app.h"

using namespace ncf;

int main() {
    ncf_app app;
    auto ctl = app.create_ctl<ncf_ctl>("ctl");
    auto win = ctl->create_win<ncfw_lines<>>("win", "", 0, 0, 0, 0);
    win->set_lines([](){
        std::vector<std::string> lines;
        int size = 10;
        lines.reserve(size);
        char s[100] = {0};
        for (int i = 0; i < size; i ++) {
            sprintf(s, "%d\tPress q to exit.\n", i);
            lines.push_back(s);
        }
        return lines;
    }());
    app.run(ctl);
}
