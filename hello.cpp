#include <ncframe.h>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    return ncframe().start<ncf_ctl<ncf_win<std::string>>>("hello", []() {
            std::vector<std::string> vals;
            char row[255] = {0};
            for (int i = 0; i < 100; i ++) {
                sprintf(row, "%d\tHello, World!\n", i);
                vals.push_back(row);
            }
            return vals;
        }());
}
