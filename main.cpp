#include <iostream>
#include <utility>

#include "generator.h"

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

    template<typename T, typename U>
    struct XValueGenerator {
    private:
        gen::ArithmeticValueGenerator<T> x_gen_;
        gen::ArithmeticValueGenerator<U> y_gen_;
        gen::StringValueGenerator str_gen_;

    public:
        XValueGenerator(gen::ArithmeticValueGenerator<T> x_gen,
                        gen::ArithmeticValueGenerator<U> y_gen,
                        gen::StringValueGenerator str_gen)
        : x_gen_(x_gen), y_gen_(y_gen), str_gen_(std::move(str_gen)) {}

        template<typename BitGen>
        X operator() (BitGen& gen) {
            return X(x_gen_(gen), y_gen_(gen), str_gen_(gen));
        }

    };

}

int main() {

    gen::Generator generator;

    // -----------------------------------------------------------------------------------------------------------------

    auto rand_v_1 = generator.generate<std::vector<int>>(10, {10, 24});
    gen::print::print(rand_v_1.begin(), rand_v_1.end(), std::cout);

    auto rand_d_1 = generator.generate<std::deque<int>>(10, {25, 39});
    gen::print::print(rand_d_1.begin(), rand_d_1.end(), std::cout);

    auto rand_l_1 = generator.generate<std::list<int>>(10, {40, 54});
    gen::print::print(rand_l_1.begin(), rand_l_1.end(), std::cout);

    auto rand_fl_1 = generator.generate<std::forward_list<int>>(10, {55, 69});
    gen::print::print(rand_fl_1.begin(), rand_fl_1.end(), std::cout);

    // -----------------------------------------------------------------------------------------------------------------

    auto rand_s_1 = generator.generate<std::set<int>>(10, {0, 9});
    gen::print::print(rand_s_1.begin(), rand_s_1.end(), std::cout);

    auto rand_us_1 = generator.generate<std::unordered_set<int>>(10, {10, 19});
    gen::print::print(rand_us_1.begin(), rand_us_1.end(), std::cout);

    // -----------------------------------------------------------------------------------------------------------------

    auto rand_m_1 = generator.generate<std::map<int, double>>(5, {0, 9}, {0.0, 1.0});
    gen::print::print_pair(rand_m_1.begin(), rand_m_1.end(), std::cout);

    auto rand_um_1 = generator.generate<std::unordered_map<int, double>>(5, {10, 30}, {10.0, 11.0});
    gen::print::print_pair(rand_um_1.begin(), rand_um_1.end(), std::cout);

    // -----------------------------------------------------------------------------------------------------------------

    gen::StringValueGenerator str_gen_1{4, 5, "abc123"};
    auto rand_v_2 = generator.generate<std::vector<std::string>>(10, str_gen_1);
    gen::print::print(rand_v_2.begin(), rand_v_2.end(), std::cout);

    gen::StringValueGenerator str_gen_2{2, 2, "ab"};
    auto rand_s_2 = generator.generate<std::set<std::string>>(4, str_gen_2);
    gen::print::print(rand_s_2.begin(), rand_s_2.end(), std::cout);

    gen::StringValueGenerator str_gen_3{3, 3, "klx"};
    gen::StringValueGenerator str_gen_4{5, 5, "aAbBcCdDeE12345"};
    auto rand_m_2 = generator.generate<std::map<std::string, std::string>>(5, str_gen_3, str_gen_4);
    gen::print::print_pair(rand_m_2.begin(), rand_m_2.end(), std::cout);

    // -----------------------------------------------------------------------------------------------------------------

    gen::ArithmeticValueGenerator x_gen{10, 15};
    gen::ArithmeticValueGenerator y_gen{3.0, 5.0};
    gen::StringValueGenerator str_gen{3, 3, "a1b2c3"};
    gen::XValueGenerator xv_gen{x_gen, y_gen, str_gen};
    auto rand_v_3 = generator.generate<std::vector<X>>(5, xv_gen);
    for (auto&& elem : rand_v_3) {
        elem.print();
    }
    std::cout << std::endl;

    // -----------------------------------------------------------------------------------------------------------------

    return 0;
}

