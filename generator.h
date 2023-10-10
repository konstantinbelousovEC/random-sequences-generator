// @copyright Copyright (c) 2023. Created by Konstantin Belousov.
// All rights reserved.

#pragma once

#include <random>
#include <stdexcept>
#include <algorithm>

namespace gen {

    // -----------------------------------------------------------------------------------------------------------------
    // concepts for Generator's generate method overloading

    template <typename T>
    concept arithmetic_v = std::is_arithmetic_v<T>;

    template <typename Container>
    concept HasKeyType = requires { typename Container::key_type; };

    template <typename Container>
    concept HasValueType = requires { typename Container::value_type; };

    template <typename Container>
    concept HasMappedType = requires { typename Container::mapped_type; };

    template <typename Container>
    concept HasAllocator = requires { typename Container::allocator_type; };

    // sequenced containers: vector, deque, list, forward_list
    template <typename Container>
    concept SequentialContainer = requires {
        requires HasValueType<Container>;
        requires !HasKeyType<Container>;
    };

    // static array
    template <typename Container>
    concept StaticArray = requires {
        requires !HasAllocator<Container>;
    };

    // associative containers with key-value pairs: map, unordered_map
    template <typename Container>
    concept MapContainer = requires(Container container) {
        requires HasKeyType<Container>;
        requires HasMappedType<Container>;
        container.insert({typename Container::key_type{}, typename Container::mapped_type{}});
    };

    // key-only associative containers: set, unordered_set
    template <typename Container>
    concept SetContainer = requires(Container container) {
        requires HasKeyType<Container>;
        requires !HasMappedType<Container>;
        container.insert(typename Container::key_type{});
    };

    template <typename ValGen, typename BitGen>
    concept ValueGen = requires(ValGen gen, BitGen bit_gen) {
        typename ValGen::return_type;
        { gen.operator()(bit_gen) } -> std::same_as<typename ValGen::return_type>;
    };

    template <typename ValGen, typename BitGen>
    concept ValueGenRange = requires(ValGen gen) {
        requires ValueGen<ValGen, BitGen>;
        { gen.get_value_range() } -> std::convertible_to<size_t>;
    };

    template <typename Container>
    concept IsFloatingPointKeyType = requires {
        std::is_floating_point_v<typename Container::key_type>;
    };

    // -----------------------------------------------------------------------------------------------------------------

    namespace val_gens {

        template <typename T>
        struct DistributionType;

        template <std::integral T>
        struct DistributionType<T> {
            using type = std::uniform_int_distribution<T>;
        };

        template <std::floating_point T>
        struct DistributionType<T> {
            using type = std::uniform_real_distribution<T>;
        };

        template<typename T>
        using Distribution_t = typename DistributionType<T>::type;

        template<arithmetic_v T>
        class ArithmeticValueGenerator final {
         public:
            using return_type = T;

         private:
            T min_ = std::numeric_limits<T>::min();
            T max_ = std::numeric_limits<T>::max();
            Distribution_t<T> distribution_{min_, max_};

         public:
            ArithmeticValueGenerator() = default;

            ArithmeticValueGenerator(T min, T max)
            : min_(min < max ? min : max),
              max_(min < max ? max : min),
              distribution_(min_, max_) {}

            [[nodiscard]] size_t get_value_range() const {
                return std::abs(max_ - min_);
            }

            template<typename BitGen>
            T operator()(BitGen& gen) {
                return distribution_(gen);
            }

        };  // class ArithmeticValueGenerator

    }  // namespace val_gens

    template<typename RandDevice = std::random_device, typename Engine = std::mt19937>
    class Generator final {
     private:
        RandDevice rd_;
        Engine gen_;

     public:
        Generator() : rd_(), gen_(rd_()) {}

        template<SequentialContainer Container,
                 typename ValueGenerator = val_gens::ArithmeticValueGenerator<typename Container::value_type>>
        requires ValueGen<ValueGenerator, Engine>
        Container generate(size_t size, ValueGenerator val_gen)
        {
            Container container(size);
            std::generate(container.begin(), container.end(), [&]() { return val_gen(gen_); });
            return container;
        }

        template<StaticArray Container,
                 typename ValueGenerator = val_gens::ArithmeticValueGenerator<typename Container::value_type>>
        requires ValueGen<ValueGenerator, Engine>
        Container generate(ValueGenerator val_gen)
        {
            Container container;
            std::generate(container.begin(), container.end(), [&]() { return val_gen(gen_); });
            return container;
        }


        template<SetContainer Container,
                 typename ValueGenerator = val_gens::ArithmeticValueGenerator<typename Container::value_type>>
        requires ValueGenRange<ValueGenerator, Engine>
        Container generate(size_t size, ValueGenerator key_gen)
        {
            if constexpr (!IsFloatingPointKeyType<Container>) {
                if (static_cast<size_t>(key_gen.get_value_range()) < size - 1)
                    throw std::range_error("Range of random values less than size of unique key's container");
            }

            Container container;
            for (size_t i = 0; i < size; ++i) {
                auto insert_info = container.insert(key_gen(gen_));
                while (!insert_info.second) {
                    insert_info = container.insert(key_gen(gen_));
                }
            }

            return container;
        }

        template<MapContainer Container,
                 typename KeyGenerator = val_gens::ArithmeticValueGenerator<typename Container::key_type>,
                 typename ValueGenerator = val_gens::ArithmeticValueGenerator<typename Container::mapped_type>>
        requires ValueGenRange<KeyGenerator, Engine> && ValueGenRange<ValueGenerator, Engine>
        Container generate(size_t size, KeyGenerator key_gen, ValueGenerator val_gen)
        {
            if constexpr (!IsFloatingPointKeyType<Container>) {
                if (static_cast<size_t>(key_gen.get_value_range()) < size - 1)
                    throw std::range_error("Range of random values less than size of unique key's container");
            }

            Container container;

            for (size_t i = 0; i < size; ++i) {
                auto insert_info = container.insert({key_gen(gen_), val_gen(gen_)});
                while (!insert_info.second) {
                    insert_info = container.insert({key_gen(gen_), val_gen(gen_)});
                }
            }

            return container;
        }
    };  // class Generator

}  // namespace gen
