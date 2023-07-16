#include <iostream>
#include <vector>
#include <forward_list>
#include <list>
#include <set>
#include <deque>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "seq-gen.h"

template<typename Iterator>
void print(Iterator start, Iterator finish, std::ostream& out) {
    for (auto curr_iter = start; curr_iter != finish; ++curr_iter) {
        out << *curr_iter << " ";
    }
    out << std::endl;
}

template<typename T, typename U>
void print(T size, U min, U max) {
    std::cout << size << " elems from " << min << " to " << max << " | ";
}

template<typename T>
void print(T size) {
    std::cout << size << " elems from min to max | ";
}

int main() {

    gen::RandomSeqGen generator;

    auto random_vector = generator.gen_seq_arithmetic_container<std::vector<int>>(5, -5, 5);
    print(5, -5, 5); std::cout << "std::vector<int>: ";
    print(random_vector.begin(), random_vector.end(), std::cout);

    auto random_vector_2 = generator.gen_seq_arithmetic_container<std::vector<long long int>>(3);
    print(3); std::cout << "std::vector<long long int>: ";
    print(random_vector_2.begin(), random_vector_2.end(), std::cout);

    auto random_vector_3 = generator.gen_seq_arithmetic_container<std::vector<short>>(7, 0, 1);
    print(7, 0, 1); std::cout << "std::vector<short>: ";
    print(random_vector_3.begin(), random_vector_3.end(), std::cout);

    auto random_vector_6 = generator.gen_seq_arithmetic_container<std::vector<double>>(10, -1.0, 1.0);
    print(10, -1.0, 1.0); std::cout << "std::vector<double>: ";
    print(random_vector_6.begin(), random_vector_6.end(), std::cout);

    // ----------------------------------------------------------------------------------------------------------------

    auto random_basic_string = generator.gen_seq_arithmetic_container<std::basic_string<char>>(10, 65, 90);
    print(10, 65, 90); std::cout << "std::basic_string<char>: ";
    print(random_basic_string.begin(), random_basic_string.end(), std::cout);

    // ----------------------------------------------------------------------------------------------------------------

    auto random_list = generator.gen_seq_arithmetic_container<std::list<int>>(10, 0, 100);
    print(10, 0, 100); std::cout << "std::list<int>: ";
    print(random_list.begin(), random_list.end(), std::cout);

    auto random_forward_list = generator.gen_seq_arithmetic_container<std::forward_list<int>>(10, 0, 100);
    print(10, 0, 100); std::cout << "std::forward_list<int>: ";
    print(random_forward_list.begin(), random_forward_list.end(), std::cout);

    auto random_deque = generator.gen_seq_arithmetic_container<std::deque<int>>(10, 0, 100);
    print(10, 0, 100); std::cout << "std::deque<int>: ";
    print(random_deque.begin(), random_deque.end(), std::cout);

    auto random_set = generator.gen_seq_arithmetic_container<std::set<int>>(10, 0, 100);
    print(10, 0, 100); std::cout << "std::set<int>: ";
    print(random_set.begin(), random_set.end(), std::cout);

    auto random_unordered_set = generator.gen_seq_arithmetic_container<std::unordered_set<int>>(10, 0, 100);
    print(10, 0, 100); std::cout << "std::unordered_set<int>: ";
    print(random_unordered_set.begin(), random_unordered_set.end(), std::cout);

    // ----------------------------------------------------------------------------------------------------------------

    auto random_map = generator.gen_asct_arithmetic_container<std::map<int, double>>(10, 0, 9, 100.0, 300.0);
    for (auto [key, value] : random_map) {
        std::cout << key << ":" << value << std::endl;
    }

    auto random_unordered_map = generator.gen_asct_arithmetic_container<std::unordered_map<double, char>>(10, 0.0, 10.0, 97, 122);
    for (auto [key, value] : random_unordered_map) {
        std::cout << key << ":" << value << std::endl;
    }

    // ----------------------------------------------------------------------------------------------------------------

    auto random_str = generator.gen_random_string(6);
    std::cout << random_str << std::endl;

    // ----------------------------------------------------------------------------------------------------------------

    return 0;
}

