#pragma once

#include <random>
#include <functional>
#include <type_traits>

#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

namespace gen {

    namespace sfinae {

        template <typename Container>
        constexpr auto test_push_back(typename Container::value_type) -> decltype(std::declval<Container>().push_back(std::declval<typename Container::value_type>()), std::true_type{});

        template <typename Container>
        constexpr auto test_push_back(...) -> std::false_type;

        template <typename Container>
        constexpr bool has_push_back_method() {
            return decltype(test_push_back<Container>(typename Container::value_type{}))::value;
        }

    // -----------------------------------------------------------------------------------------------------------------

        template <typename Container>
        constexpr auto test_push_front(typename Container::value_type) -> decltype(std::declval<Container>().push_front(std::declval<typename Container::value_type>()), std::true_type{});

        template <typename Container>
        constexpr auto test_push_front(...) -> std::false_type;

        template <typename Container>
        constexpr bool has_push_front_method() {
            return decltype(test_push_front<Container>(typename Container::value_type{}))::value;
        }

    // -----------------------------------------------------------------------------------------------------------------

        template <typename Container>
        constexpr auto test_insert(typename Container::value_type) -> decltype(std::declval<Container>().insert(std::declval<typename Container::value_type>()), std::true_type{});

        template <typename Container>
        constexpr auto test_insert(...) -> std::false_type;

        template <typename Container>
        constexpr bool has_insert_method() {
            return decltype(test_insert<Container>(typename Container::value_type{}))::value;
        }

    // -----------------------------------------------------------------------------------------------------------------

        template <typename Container>
        constexpr auto test_reserve(typename Container::size_type) -> decltype(std::declval<Container>().reserve(std::declval<typename Container::size_type>()), std::true_type{});

        template <typename Container>
        constexpr auto test_reserve(...) -> std::false_type;

        template <typename Container>
        constexpr bool has_reserve_method() {
            return decltype(test_reserve<Container>(typename Container::size_type{}))::value;
        }

    // -----------------------------------------------------------------------------------------------------------------

        template <typename T>
        struct is_std_seq_container : std::false_type {};

        template <typename T>
        struct is_std_seq_container<std::vector<T>> : std::true_type {};

        template <typename T>
        struct is_std_seq_container<std::deque<T>> : std::true_type {};

        template <typename T>
        struct is_std_seq_container<std::list<T>> : std::true_type {};

        template <typename T>
        struct is_std_seq_container<std::forward_list<T>> : std::true_type {};

        template <typename T>
        constexpr bool is_std_seq_container_v = is_std_seq_container<T>::value;

    // -----------------------------------------------------------------------------------------------------------------

        template <typename T>
        struct is_std_associative_val_only_container : std::false_type {};

        template <typename T>
        struct is_std_associative_val_only_container<std::set<T>> : std::true_type {};

        template <typename T>
        struct is_std_associative_val_only_container<std::unordered_set<T>> : std::true_type {};

        template <typename T>
        constexpr bool is_std_associative_val_only_container_v = is_std_associative_val_only_container<T>::value;

    // -----------------------------------------------------------------------------------------------------------------

        template <typename T>
        struct is_std_associative_key_val_container : std::false_type {};

        template <typename T, typename U>
        struct is_std_associative_key_val_container<std::map<T, U>> : std::true_type {};

        template <typename T, typename U>
        struct is_std_associative_key_val_container<std::unordered_map<T, U>> : std::true_type {};

        template <typename T>
        constexpr bool is_std_associative_key_val_container_v = is_std_associative_key_val_container<T>::value;

    } // namespace sfinae

    template<typename T> using enable_if_std_seq_container = std::enable_if_t<sfinae::is_std_seq_container_v<T>, T>;
    template<typename T> using enable_if_std_actv_val_container = std::enable_if_t<sfinae::is_std_associative_val_only_container_v<T>, T>;
    template<typename T> using enable_if_std_actv_key_val_container = std::enable_if_t<sfinae::is_std_associative_key_val_container_v<T>, T>;

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

            std::uniform_int_distribution<str_sz> dist(0, char_coll_.size() - 1);

            for (int i = 0; i < str_size; ++i) {
                rand_str += char_coll_[dist(gen)];
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
        Generator() : rd_(), gen_(rd_()) {}

        template<typename SequenceContainer, typename ValueGenerator = ArithmeticValueGenerator<value_type<SequenceContainer>>>
        enable_if_std_seq_container<SequenceContainer> generate(size_type<SequenceContainer> size,
                                                                ValueGenerator val_gen = ArithmeticValueGenerator<value_type<SequenceContainer>>{});

        template<typename SequenceContainer, typename ValueGenerator = ArithmeticValueGenerator<value_type<SequenceContainer>>>
        enable_if_std_actv_val_container<SequenceContainer> generate(size_type<SequenceContainer> size,
                                                                     ValueGenerator val_gen = ArithmeticValueGenerator<value_type<SequenceContainer>>{});

        template<typename SequenceContainer, typename KeyGenerator = ArithmeticValueGenerator<key_type<SequenceContainer>>, typename ValueGenerator = ArithmeticValueGenerator<mapped_type<SequenceContainer>>>
        enable_if_std_actv_key_val_container<SequenceContainer> generate(size_type<SequenceContainer> size,
                                                                         KeyGenerator key_gen = ArithmeticValueGenerator<value_type<SequenceContainer>>{},
                                                                         ValueGenerator val_gen = ArithmeticValueGenerator<value_type<SequenceContainer>>{});

    }; // Generator

    template<typename SequenceContainer, typename ValueGenerator>
    enable_if_std_seq_container<SequenceContainer> Generator::generate(size_type<SequenceContainer> size,
                                                                       ValueGenerator val_gen)
    {
        SequenceContainer container;
        if constexpr (sfinae::has_reserve_method<SequenceContainer>()) container.reserve(size);

        for (size_type<SequenceContainer> i = 0; i < size; ++i) {
            if constexpr (sfinae::has_push_back_method<SequenceContainer>()) {
                container.push_back(val_gen(gen_));
            } else if constexpr (sfinae::has_push_front_method<SequenceContainer>()) {
                container.push_front(val_gen(gen_));
            }
        }

        return container;
    }

    template<typename SequenceContainer, typename ValueGenerator>
    enable_if_std_actv_val_container<SequenceContainer> Generator::generate(size_type<SequenceContainer> size,
                                                                            ValueGenerator val_gen)
    {
        // the size of container must be more or equal to all possible unique values in range [val_gen.min, val_gen.max]
        // if this condition is false - UB or infinite cycle may happen
        // todo: add check for this condition - it will throw an exception then

        SequenceContainer container;

        for (size_type<SequenceContainer> i = 0; i < size; ++i) {
            auto insert_info = container.insert(val_gen(gen_));
            while (!insert_info.second) {
                insert_info = container.insert(val_gen(gen_));
            }
        }

        return container;
    }

    template<typename SequenceContainer, typename KeyGenerator, typename ValueGenerator>
    enable_if_std_actv_key_val_container<SequenceContainer>
    Generator::generate(size_type<SequenceContainer> size, KeyGenerator key_gen, ValueGenerator val_gen)
    {
        // the size of container must be more or equal to all possible unique values in range [val_gen.min, val_gen.max]
        // if this condition is false - UB or infinite cycle may happen
        // todo: add check for this condition - it will throw an exception then

        SequenceContainer container;

        for (size_type<SequenceContainer> i = 0; i < size; ++i) {
            auto insert_info = container.insert({key_gen(gen_), val_gen(gen_)});
            while (!insert_info.second) {
                insert_info = container.insert({key_gen(gen_), val_gen(gen_)});
            }
        }

        return container;
    }

} // namespace gen
