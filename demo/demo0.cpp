#include "ncframe.h"

using namespace ncf;

int main()
{
    ncf_app app;
    auto ctl = app.create_ctl<ncf_ctl>("ctl");
    ncfwi wi;
    auto win = ctl->create_win<ncfw_lines>("ncfw_lines", wi);
    app.run(ctl);
}
