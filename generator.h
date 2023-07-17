#pragma once

#include <random>
#include <functional>

#include "sfinae-defs.h"

namespace gen {

    template<typename T> using enable_if_arithmetic = std::enable_if_t<std::is_arithmetic_v<T>, T>;
    template<typename T> using enable_if_std_string = std::enable_if_t<is_std_string_v<T>, T>;
    template<typename T> using enable_if_std_seq_container = std::enable_if_t<is_std_seq_container_v<T>, T>;

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    struct ArithmeticValueGenerator {
    private:
        T min_ = std::numeric_limits<T>::min();
        T max_ = std::numeric_limits<T>::max();

    public:
        ArithmeticValueGenerator() = default;
        ArithmeticValueGenerator(T min, T max) : min_(min), max_(max) {}

        template<typename BitGen>
        T operator() (BitGen& gen) {
            if constexpr (std::is_integral<T>::value) {
                return std::uniform_int_distribution<T> {min_, max_}(gen);
            } else {
                return std::uniform_real_distribution<T> {min_, max_}(gen);
            }
        }
    }; // ArithmeticValueGenerator

    struct StringValueGenerator {
    private:
        using str_sz = std::string::size_type;
    private:
        str_sz min_sz_;
        str_sz max_sz_;
        std::string char_coll_;

    public:
        StringValueGenerator(str_sz min_size, str_sz max_size, std::string char_coll)
                : min_sz_(min_size), max_sz_(max_size), char_coll_(std::move(char_coll )) {}

        template<typename BitGen>
        std::string operator() (BitGen& gen) {
            std::uniform_int_distribution<str_sz> str_size_dist(min_sz_, max_sz_);
            str_sz str_size = str_size_dist(gen);

            std::string rand_str;
            rand_str.reserve(str_size);

            std::uniform_int_distribution<str_sz> distribution(0, char_coll_.size() - 1);

            for (int i = 0; i < str_size; ++i) {
                rand_str += char_coll_[distribution(gen)];
            }

            return rand_str;
        }

    }; // StringValueGenerator

    class Generator {
    public:
        template<typename Container> using size_type = typename Container::size_type;
        template<typename Container> using value_type = typename Container::value_type;
        template<typename AssociativeContainer> using key_type = typename AssociativeContainer::key_type;
        template<typename AssociativeContainer> using mapped_type = typename AssociativeContainer::mapped_type;

    private:
        std::random_device rd_;
        std::mt19937 gen_;

    public:
        Generator();

        //  = std::function<value_type<SequenceContainer>(NumGenerator, value_type<SequenceContainer>, value_type<SequenceContainer>)>
        template<typename SequenceContainer, typename ValueGenerator = ArithmeticValueGenerator<value_type<SequenceContainer>>>
        enable_if_std_seq_container<SequenceContainer> generate(size_type<SequenceContainer> size,
                                                                ValueGenerator val_gen = ArithmeticValueGenerator<value_type<SequenceContainer>>{});

    }; // Generator

    Generator::Generator() : rd_(), gen_(rd_()) {}

    template<typename SequenceContainer, typename ValueGenerator>
    enable_if_std_seq_container<SequenceContainer> Generator::generate(size_type<SequenceContainer> size,
                                                                       ValueGenerator val_gen)
    {
        SequenceContainer container;
        if constexpr (has_reserve_method<SequenceContainer>()) container.reserve(size);

        for (size_type<SequenceContainer> i = 0; i < size; ++i) {
            if constexpr (has_push_back_method<SequenceContainer>()) {
                container.push_back(val_gen(gen_));
            } else if constexpr (has_push_front_method<SequenceContainer>()) {
                container.push_front(val_gen(gen_));
            }
        }

        return container;
    }

} // namespace

/*

 sequenced containers:
    - vector
    - deque
    - list
    - forward_list
 associative containers:
    key-value
        - map
        - unordered_map
    value
        - set
        - unordered_set

*/
