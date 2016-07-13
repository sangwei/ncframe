#include "ncframe.h"
#include <fstream>
#include <streambuf>

using namespace ncf;

int main()
{
    ncf_app app;
    auto ctl = app.create_ctl<ncf_ctl>("ctl");
    /*
    ncfwi wi;
    auto win = ctl->create_win<ncfw_text>("text", wi);
    std::ifstream ifs("CMakeCache.txt");
    std::string str((std::istreambuf_iterator<char>(ifs)),
                             std::istreambuf_iterator<char>());
    win->set_text(str.c_str());
    */
    ncfwi wi;
    auto win = ctl->create_win<ncfw_lines>("text", wi);

    std::ifstream ifs("CMakeCache.txt");
    std::string str((std::istreambuf_iterator<char>(ifs)),
                             std::istreambuf_iterator<char>());
    std::vector<std::string> lines;
    int it = 0;
    int lb = 0;
    while (it != str.size()) {
        it ++;
        if (str[it - 1] == '\n') {
            lines.push_back(str.substr(lb, it - lb));
            lb = it;
        }
    }
    if (lb != str.size()) {
        lines.push_back(str.substr(lb));
    }
    win->set_lines(std::move(lines));
    win->set_sel_underline(true);
    app.run(ctl);
}
