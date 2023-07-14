#include <iostream>
#include <vector>
#include <forward_list>
#include <list>
#include <set>
#include <deque>
#include <unordered_set>

#include "seq-gen.h"

int main() {

    gen::RandomSeqGen generator;

    auto random_vector = generator.gen_arithmetic_container<std::vector, int>(10, 5, 9);
    print(random_vector.begin(), random_vector.end(), std::cout);

    auto random_vector_2 = generator.gen_arithmetic_container<std::vector, uint8_t>(10, 100, 120);
    print(random_vector_2.begin(), random_vector_2.end(), std::cout);

    auto random_vector_3 = generator.gen_arithmetic_container<std::vector, uint16_t>(10, 100, 120);
    print(random_vector_3.begin(), random_vector_3.end(), std::cout);

    auto random_vector_4 = generator.gen_arithmetic_container<std::vector, long long int>(10, -1000, 1000);
    print(random_vector_4.begin(), random_vector_4.end(), std::cout);

    auto random_vector_5 = generator.gen_arithmetic_container<std::vector, int>(20);
    print(random_vector_5.begin(), random_vector_5.end(), std::cout);

    // ----------------------------------------------------------------------------------------------------------------

    auto random_str = generator.gen_arithmetic_container<std::basic_string, char>(50, 65, 90);
    print(random_str.begin(), random_str.end(), std::cout);

    // ----------------------------------------------------------------------------------------------------------------

    auto random_list = generator.gen_arithmetic_container<std::list, float>(5, 3.5f, 4.7f);
    print(random_list.begin(), random_list.end(), std::cout);

    auto random_forward_list = generator.gen_arithmetic_container<std::forward_list, int>(15, -100, -50);
    print(random_forward_list.begin(), random_forward_list.end(), std::cout);

    auto random_deque = generator.gen_arithmetic_container<std::deque, char>(7, 'A', 'Z');
    print(random_deque.begin(), random_deque.end(), std::cout);

    auto random_set = generator.gen_arithmetic_container<std::set, float>(10, 0.5f, 10.7f);
    print(random_set.begin(), random_set.end(), std::cout);

    auto random_unordered_set = generator.gen_arithmetic_container<std::unordered_set, float>(3, 0.5f, 10.7f);
    print(random_unordered_set.begin(), random_unordered_set.end(), std::cout);

//    auto array = generator.gen_arithmetic_container<std::array<float, 3>, float>(3, 0.5f, 10.7f);
//    print(random_unordered_set.begin(), random_unordered_set.end(), std::cout);

    // ----------------------------------------------------------------------------------------------------------------

//    auto random_list_2 = generator.gen_arithmetic_container<std::list, bool>(5, 3.5f, 4.7f);
//    print(random_list.begin(), random_list.end(), std::cout);

    return 0;
}

