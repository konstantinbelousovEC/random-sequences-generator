#include <iostream>

#include "generator.h"

template<typename Iterator>
void print(Iterator start, Iterator finish, std::ostream& out) {
    for (auto curr_iter = start; curr_iter != finish; ++curr_iter) {
        out << *curr_iter << " ";
    }
    out << std::endl;
}

template<typename Iterator>
void print_pair(Iterator start, Iterator finish, std::ostream& out) {
    for (auto curr_iter = start; curr_iter != finish; ++curr_iter) {
        out << curr_iter->first << " : " << curr_iter->second << " | ";
    }
    out << std::endl;
}

class X {
private:
    int x_{};
    int x_2_{};
    double y_{};
    std::string str_;

public:
    void print() {
        std::cout << "|" << x_ << " : " << x_2_ << " : " << y_ << " : " << str_ << "|" << " ";
    }

    X() = default;
    X(int x, double y, std::string str) : x_(x), x_2_(x_ * 2), y_(y), str_(std::move(str)) {}
};

namespace gen {

    struct XValueGenerator {
    public:
        int min_x_;
        int max_x_;
        double min_y_;
        double max_y_;
        std::string::size_type min_sz_str_;
        std::string::size_type max_sz_str_;
        std::string ch_col_;

        template<typename BitGen>
        X operator() (BitGen& gen) {
            gen::ArithmeticValueGenerator x_gen{min_x_, max_x_};
            gen::ArithmeticValueGenerator y_gen{min_y_, max_y_};
            gen::StringValueGenerator str_gen{min_sz_str_, max_sz_str_, ch_col_};

            return X(x_gen(gen), y_gen(gen), str_gen(gen));
        }

    };

}

int main() {

    gen::Generator generator;

    // -----------------------------------------------------------------------------------------------------------------

    auto rand_v_1 = generator.generate<std::vector<int>>(10, {10, 24});
    print(rand_v_1.begin(), rand_v_1.end(), std::cout);

    auto rand_d_1 = generator.generate<std::deque<int>>(10, {25, 39});
    print(rand_d_1.begin(), rand_d_1.end(), std::cout);

    auto rand_l_1 = generator.generate<std::list<int>>(10, {40, 54});
    print(rand_l_1.begin(), rand_l_1.end(), std::cout);

    auto rand_fl_1 = generator.generate<std::forward_list<int>>(10, {55, 69});
    print(rand_fl_1.begin(), rand_fl_1.end(), std::cout);

    // -----------------------------------------------------------------------------------------------------------------

    auto rand_s_1 = generator.generate<std::set<int>>(10, {0, 9});
    print(rand_s_1.begin(), rand_s_1.end(), std::cout);

    auto rand_us_1 = generator.generate<std::unordered_set<int>>(10, {10, 19});
    print(rand_us_1.begin(), rand_us_1.end(), std::cout);

    // -----------------------------------------------------------------------------------------------------------------

    auto rand_m_1 = generator.generate<std::map<int, double>>(5, {0, 9}, {0.0, 1.0});
    print_pair(rand_m_1.begin(), rand_m_1.end(), std::cout);

    auto rand_um_1 = generator.generate<std::unordered_map<int, double>>(5, {10, 30}, {10.0, 11.0});
    print_pair(rand_um_1.begin(), rand_um_1.end(), std::cout);

    // -----------------------------------------------------------------------------------------------------------------

    gen::StringValueGenerator str_gen_1{4, 5, "abc123"};
    auto rand_v_2 = generator.generate<std::vector<std::string>>(10, str_gen_1);
    print(rand_v_2.begin(), rand_v_2.end(), std::cout);

    gen::StringValueGenerator str_gen_2{2, 2, "ab"};
    auto rand_s_2 = generator.generate<std::set<std::string>>(4, str_gen_2);
    print(rand_s_2.begin(), rand_s_2.end(), std::cout);

    gen::StringValueGenerator str_gen_3{3, 3, "klx"};
    gen::StringValueGenerator str_gen_4{5, 5, "aAbBcCdDeE12345"};
    auto rand_m_2 = generator.generate<std::map<std::string, std::string>>(5, str_gen_3, str_gen_4);
    print_pair(rand_m_2.begin(), rand_m_2.end(), std::cout);

    // -----------------------------------------------------------------------------------------------------------------

    gen::XValueGenerator x_gen{10, 15, 3.0, 5.0, 3, 3, "a1b2c3"};
    auto rand_v_3 = generator.generate<std::vector<X>>(5, x_gen);
    for (auto&& elem : rand_v_3) {
        elem.print();
    }
    std::cout << std::endl;

    // -----------------------------------------------------------------------------------------------------------------

    return 0;
}

