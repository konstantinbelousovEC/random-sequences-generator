#pragma once

#include <random>
#include <iostream>
#include <iterator>

#include "sfinae-defs.h"

namespace gen {

    class RandomSeqGen {
    private:
        std::random_device rd_;
        std::mt19937 gen_;

    public:
        RandomSeqGen(): rd_(), gen_(rd_()) {};

        template <template<typename...> class Container, typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
        Container<T> gen_arithmetic_container(typename Container<T>::size_type size, T min_value, T max_value);

        template <template<typename...> class Container, typename T>
        Container<T> gen_arithmetic_container(typename Container<T>::size_type size);

    private:
        template <typename T>
        T gen_random_arithmetic_value(std::mt19937& gen, T min_value, T max_value);

    };

    // RandomSeqGen definitions
    // ----------------------------------------------------------------------------------------------------------------

    template <template<typename...> class Container, typename T, typename>
    Container<T> RandomSeqGen::gen_arithmetic_container(typename Container<T>::size_type size, T min_value, T max_value) {
        Container<T> container;

        if constexpr (has_reserve_method<Container<T>>()) container.reserve(size);

        for (size_t i = 0; i < size; ++i) {
            if constexpr (has_push_back_method<Container<T>>()) {
                container.push_back(gen_random_arithmetic_value<T>(gen_, min_value, max_value));
            } else if constexpr (has_insert_method<Container<T>>()) {
                container.insert(gen_random_arithmetic_value<T>(gen_, min_value, max_value));
            } else if constexpr (has_push_front_method<Container<T>>()) {
                container.push_front(gen_random_arithmetic_value<T>(gen_, min_value, max_value));
            }
        }
        return container;

    }

    template <template<typename...> class Container, typename T>
    Container<T> RandomSeqGen::gen_arithmetic_container(typename Container<T>::size_type size) {
        return gen_arithmetic_container<Container, T>(size, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    }

    template <typename T>
    T RandomSeqGen::gen_random_arithmetic_value(std::mt19937& gen, T min_value, T max_value) {
        if constexpr (std::is_integral<T>::value) {
            std::uniform_int_distribution<T> dis(min_value, max_value);
            return dis(gen);
        } else {
            std::uniform_real_distribution<T> dis(min_value, max_value);
            return dis(gen);
        }
    }

    template <>
    [[maybe_unused]] bool RandomSeqGen::gen_random_arithmetic_value(std::mt19937& gen, bool min_value, bool max_value) = delete;

}   // namespace gen --------------------------------------------------------------------------------------------------

template<typename Iterator>
void print(Iterator start, Iterator finish, std::ostream& out) {
    for (auto curr_iter = start; curr_iter != finish; ++curr_iter) {
        out << *curr_iter << " ";
    }
    out << std::endl;
}

// template <>
//    std::basic_string<char> RandomSeqGen::gen_arithmetic_container(typename std::basic_string<char>::size_type size, char min_value, char max_value) {
//        std::basic_string<char> container;
//        container.reserve(size);
//
//        for (size_t i = 0; i < size; ++i) {
//            int rand = gen_random_arithmetic_value<int>(gen_, 0, 51);
//            if (rand < 26) {
//                container.push_back(gen_random_arithmetic_value<char>(gen_, 65, 90));
//            } else {
//                container.push_back(gen_random_arithmetic_value<char>(gen_, 97, 122));
//            }
//        }
//
//        return container;
//    }