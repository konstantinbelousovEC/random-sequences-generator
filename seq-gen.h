#pragma once

#include <random>
#include <iostream>
#include <iterator>

#include "sfinae-defs.h"

namespace gen {

    template <typename T>
    T gen_random_value(const std::mt19937& gen, T min_value, T max_value) = delete;

    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    T gen_random_value(std::mt19937& gen, T min_value, T max_value) {
        if constexpr (std::is_integral<T>::value) {
            std::uniform_int_distribution<T> dis(min_value, max_value);
            return dis(gen);
        } else {
            std::uniform_real_distribution<T> dis(min_value, max_value);
            return dis(gen);
        }
    }

    class RandomSeqGen {
    private:
        std::random_device rd_;
        std::mt19937 gen_;

    public:
        RandomSeqGen(): rd_(), gen_(rd_()) {};

        template <template<typename...> class Container, typename T>
        Container<T> generate_container(typename Container<T>::size_type size, T min_value, T max_value) {
            Container<T> container;

            if constexpr (has_reserve_method<Container<T>>()) container.reserve(size);

            for (size_t i = 0; i < size; ++i) {
                if constexpr (HasPushBackMethod<Container<T>>()) {
                    container.push_back(gen_random_value<T>(gen_, min_value, max_value));
                } else {
                    container.insert(gen_random_value<T>(gen_, min_value, max_value));
                }
            }
            return container;

        }

        template <template<typename...> class Container, typename T>
        Container<T> generate_container(typename Container<T>::size_type size) {
            return generate_container<Container, T>(size, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        }
    };

    template <>
    std::basic_string<char> RandomSeqGen::generate_container(typename std::basic_string<char>::size_type size, char min_value, char max_value) {
        std::basic_string<char> container;
        container.reserve(size);

        for (size_t i = 0; i < size; ++i) {
            int rand = gen_random_value<int>(gen_, 0, 51);
            if (rand < 26) {
                container.push_back(gen_random_value<char>(gen_, 65, 90));
            } else {
                container.push_back(gen_random_value<char>(gen_, 97, 122));
            }
        }

        return container;
    }

} // namespace gen

template<typename Iterator>
void print(Iterator start, Iterator finish, std::ostream& out) {
    for (auto curr_iter = start; curr_iter != finish; ++curr_iter) {
        out << *curr_iter << " ";
    }
    out << std::endl;
}