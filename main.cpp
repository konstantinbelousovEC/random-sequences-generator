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


    return 0;
}

