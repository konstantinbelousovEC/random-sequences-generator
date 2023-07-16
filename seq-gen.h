#pragma once

#include <random>
#include <iostream>
#include <iterator>

#include "sfinae-defs.h"

namespace gen {

    template<typename Container>
    using value_type = typename Container::value_type;

    template<typename Container>
    using size_type = typename Container::size_type;

    template<typename AsctContainer>
    using key_type = typename AsctContainer::key_type;

    template<typename AsctContainer>
    using mapped_type = typename AsctContainer::mapped_type;

    class RandomSeqGen {
    private:
        std::random_device rd_;
        std::mt19937 gen_;

    public:
        RandomSeqGen(): rd_(), gen_(rd_()) {};

        template<typename Container, typename = std::enable_if_t<std::is_arithmetic_v<value_type<Container>>>>
        Container gen_seq_arithmetic_container(size_type<Container> size, value_type<Container> min_value, value_type<Container> max_value);

        template<typename Container>
        Container gen_seq_arithmetic_container(typename Container::size_type size);

        template<typename AsctContainer>
        AsctContainer gen_asct_arithmetic_container(size_type<AsctContainer> size,
                                                    key_type<AsctContainer> min_key, key_type<AsctContainer> max_key,
                                                    mapped_type<AsctContainer> min_value, mapped_type<AsctContainer> max_value)
        {
            AsctContainer container;
            if constexpr (has_reserve_method<AsctContainer>()) container.reserve(size);

            for (size_type<AsctContainer> i = 0; i < size; ++i) {
                auto res = container.try_emplace(gen_random_arithmetic_value(gen_, min_key, max_key), gen_random_arithmetic_value(gen_, min_value, max_value));
                while (!res.second) {
                    res = container.try_emplace(gen_random_arithmetic_value(gen_, min_key, max_key), gen_random_arithmetic_value(gen_, min_value, max_value));
                }
            }

            return container;
        }

    private:
        template <typename T>
        T gen_random_arithmetic_value(std::mt19937& gen, T min_value, T max_value);

    };

    // RandomSeqGen definitions
    // ----------------------------------------------------------------------------------------------------------------

    template<typename Container, typename>
    Container RandomSeqGen::gen_seq_arithmetic_container(size_type<Container> size, value_type<Container> min_value, value_type<Container> max_value) {
        Container container;
        if constexpr (has_reserve_method<Container>()) container.reserve(size);

        for (size_t i = 0; i < size; ++i) {
            if constexpr (has_push_back_method<Container>()) {
                container.push_back(gen_random_arithmetic_value(gen_, min_value, max_value));
            } else if constexpr (has_insert_method<Container>()) {
                container.insert(gen_random_arithmetic_value(gen_, min_value, max_value));
            } else if constexpr (has_push_front_method<Container>()) {
                container.push_front(gen_random_arithmetic_value(gen_, min_value, max_value));
            }
        }

        return container;
    }

    template <typename Container>
    Container RandomSeqGen::gen_seq_arithmetic_container(size_type<Container> size) {
        return gen_seq_arithmetic_container<Container>(size, std::numeric_limits<value_type<Container>>::min(), std::numeric_limits<value_type<Container>>::max());
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

// template <>
//    std::basic_string<char> RandomSeqGen::gen_seq_arithmetic_container(typename std::basic_string<char>::size_type size, char min_value, char max_value) {
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