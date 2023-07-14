#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <deque>
#include <unordered_set>

#include "seq-gen.h"

int main() {

    gen::RandomSeqGen generator;

    auto random_vector = generator.generate_container<std::vector, int>(10, 5, 9);
    print(random_vector.begin(), random_vector.end(), std::cout);

    auto random_vector_2 = generator.generate_container<std::vector, uint8_t>(10, 100, 120);
    print(random_vector_2.begin(), random_vector_2.end(), std::cout);

    auto random_vector_3 = generator.generate_container<std::vector, uint16_t>(10, 100, 120);
    print(random_vector_3.begin(), random_vector_3.end(), std::cout);

    auto random_vector_4 = generator.generate_container<std::vector, long long int>(10, -1000, 1000);
    print(random_vector_4.begin(), random_vector_4.end(), std::cout);

    auto random_vector_5 = generator.generate_container<std::vector, int>(20);
    print(random_vector_5.begin(), random_vector_5.end(), std::cout);

    // ----------------------------------------------------------------------------------------------------------------

    auto random_str = generator.generate_container<std::basic_string, char>(50, 65, 90);
    print(random_str.begin(), random_str.end(), std::cout);

    // ----------------------------------------------------------------------------------------------------------------

    auto random_list = generator.generate_container<std::list, float>(5, 3.5f, 4.7f);
    print(random_list.begin(), random_list.end(), std::cout);

    auto random_deque = generator.generate_container<std::deque, char>(7, 'A', 'Z');
    print(random_deque.begin(), random_deque.end(), std::cout);

    auto random_set = generator.generate_container<std::set, float>(10, 0.5f, 10.7f);
    print(random_set.begin(), random_set.end(), std::cout);

    auto random_unordered_set = generator.generate_container<std::unordered_set, float>(3, 0.5f, 10.7f);
    print(random_unordered_set.begin(), random_unordered_set.end(), std::cout);

    return 0;
}

