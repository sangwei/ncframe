#include <ncf_win.h>
#include <ncf_ctl.h>
#include <ncframe.h>
#include <string>
#include <vector>
#include <tuple>
#include <iostream>

template<int Index, class Search, class First, class... Types>
struct get_internal
{
	typedef typename get_internal<Index + 1, Search, Types...>::type type;
    static constexpr int index = Index;
};

template<int Index, class Search, class... Types>
struct get_internal<Index, Search, Search, Types...>
{
	typedef get_internal type;
	static constexpr int index = Index;
};

template<class T, class... Types>
T get(std::tuple<Types...> tuple)
{
	return std::get<get_internal<0,T,Types...>::type::index>(tuple);
}

template <typename ncfw_t>
class a_ctl : public ncf_ctl<ncfw_t> {
public:
    a_ctl(ncfw_t&& win, std::tuple<std::string, int>&& data) :
        ncf_ctl<ncfw_t>(std::move(win)) {
        test_ = std::get<0>(data);
        ans_ = std::get<1>(data);
    };

    virtual int init() {
        ncf_ctl<ncfw_t>::win_.buf.push_back(test_);

        if (ans_ == 42) {
            ncf_ctl<ncfw_t>::win_.buf.push_back("final ans\n");
        }
        return ncf_ctl<ncfw_t>::init();
    };
private:
    std::string test_;
    int ans_;
};

template <template <typename, typename> class win_t,
    typename buf_t, typename fmt_t>
void test_func() {
    win_t<buf_t, fmt_t> w;
};

template <typename...Ts>
class fram {
public:
    std::tuple<Ts...> objs;
    fram(Ts...ts) {
        objs = std::make_tuple(ts...);
    };
    template <typename T>
    T getv() {
        return get<T>(objs);
    };
};

int main(int argc, char** argv) {
    auto fr = new fram<std::string, int>("t1", 42);
    std::cout << get<std::string>(fr->objs) << std::endl;
    std::cout << fr->getv<int>() << std::endl;
    return 0;
    auto tpl = std::make_tuple(std::string("t1"), 3);
    //auto v1 = std::get<std::string>(tpl);

    return ncframe().start<a_ctl<ncf_win<std::string>>>("hello",
        std::make_tuple("test?", 42),
        []() {
            std::vector<std::string> vals;
            char row[255] = {0};
            for (int i = 0; i < 10; i ++) {
                sprintf(row, "%d\tHello, World!\n", i);
                vals.push_back(row);
            }
            return vals;
        }());
    // return ncframe().start<ncf_ctl<ncf_win<std::string>>>("hello",
    //     "test",
    //     []() {
    //         std::vector<std::string> vals;
    //         char row[255] = {0};
    //         for (int i = 0; i < 100; i ++) {
    //             sprintf(row, "%d\tHello, World!\n", i);
    //             vals.push_back(row);
    //         }
    //         return vals;
    //     }());
}
