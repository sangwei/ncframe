#include <ncframe.h>

int main(int argc, char** argv) {
    ncf_win<std::string> win;
    win.push_back("Hello, World!\n");

    char row[255] = {0};
    for (int i = 0; i < 100; i ++) {
        sprintf(row, "%d\tHello, World!\n", i);
        win.push_back(row);
    }

    ncf_ctl ctl(&win);
    ncframe().start(&ctl);
    return 0;
}
