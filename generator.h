// @copyright Copyright (c) 2023. Created by Konstantin Belousov.
// All rights reserved.

#pragma once

#include <random>
#include <stdexcept>
#include <algorithm>

namespace gen {

    // -----------------------------------------------------------------------------------------------------------------
    // concepts for Generator's generate method overloading

    // sequenced containers: vector, deque, list, forward_list
    template <typename Container>
    concept SequentialContainer = requires(Container cont) {
        typename Container::value_type;
        requires !requires {
            typename Container::key_type;
        };
    };

    // static array
    template <typename Container>
    concept StaticArray = requires(Container cont) {
        requires !requires {
            typename Container::allocator_type;
        };
    };

    // associative containers with key-value pairs: map, unordered_map
    template <typename Container>
    concept MapContainer = requires(Container cont) {
        typename Container::key_type;
        typename Container::mapped_type;
        cont.insert({typename Container::key_type{}, typename Container::mapped_type{}});
    };

    // key-only associative containers: set, unordered_set
    template <typename Container>
    concept SetContainer = requires(Container cont) {
        typename Container::key_type;
        requires !requires {
            typename Container::mapped_type;
        };
        cont.insert(typename Container::key_type{});
    };

    // -----------------------------------------------------------------------------------------------------------------

    namespace val_generators {

        template <typename T>
        struct DistributionType;

        template <typename T>
        requires std::is_integral_v<T>
        struct DistributionType<T> {
            using type = std::uniform_int_distribution<T>;
        };

        template <typename T>
        requires std::is_floating_point_v<T>
        struct DistributionType<T> {
            using type = std::uniform_real_distribution<T>;
        };

        template<typename T>
        using Distribution_t = typename DistributionType<T>::type;

        template<typename T> requires std::is_arithmetic_v<T>
        struct ArithmeticValueGenerator final {
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

            [[nodiscard]] size_t get_value_range() const noexcept {
                return std::abs(max_ - min_);
            }

            template<typename BitGen>
            T operator()(BitGen& gen) {
                return distribution_(gen);
            }

        };  // struct ArithmeticValueGenerator

    }  // namespace val_generators

    template<typename RandDevice = std::random_device, typename Engine = std::mt19937>
    class Generator final {
     private:
        RandDevice rd_;
        Engine gen_;

     public:
        Generator() : rd_(), gen_(rd_()) {}

        template<SequentialContainer Container,
                 typename size_type = typename Container::size_type,
                 typename ValueGenerator = val_generators::ArithmeticValueGenerator<typename Container::value_type>>
        Container generate(size_type size, ValueGenerator val_gen)
        {
            Container container(size);
            std::generate(container.begin(), container.end(), [&]() { return val_gen(gen_); });
            return container;
        }

        template<StaticArray Container,
                 typename ValueGenerator = val_generators::ArithmeticValueGenerator<typename Container::value_type>>
        Container generate(ValueGenerator val_gen)
        {
            Container container;
            std::generate(container.begin(), container.end(), [&]() { return val_gen(gen_); });
            return container;
        }


        template<SetContainer Container,
                 typename size_type = typename Container::size_type,
                 typename ValueGenerator = val_generators::ArithmeticValueGenerator<typename Container::value_type>>
        Container generate(size_type size, ValueGenerator key_gen)
        {
            if constexpr (!std::is_floating_point_v<typename Container::key_type>) {
                if (static_cast<size_type>(key_gen.get_value_range()) < size - 1)
                    throw std::range_error("Range of random values less than size of unique key's container");
            }

            Container container;

            for (size_type i = 0; i < size; ++i) {
                auto insert_info = container.insert(key_gen(gen_));
                while (!insert_info.second) {
                    insert_info = container.insert(key_gen(gen_));
                }
            }

            return container;
        }

        template<MapContainer Container,
                 typename size_type = typename Container::size_type,
                 typename KeyGenerator = val_generators::ArithmeticValueGenerator<typename Container::key_type>,
                 typename ValueGenerator = val_generators::ArithmeticValueGenerator<typename Container::mapped_type>>
        Container generate(size_type size, KeyGenerator key_gen, ValueGenerator val_gen)
        {
            if constexpr (!std::is_floating_point_v<typename Container::key_type>) {
                if (static_cast<size_type>(key_gen.get_value_range()) < size - 1)
                    throw std::range_error("Range of random values less than size of unique key's container");
            }

            Container container;

            for (size_type i = 0; i < size; ++i) {
                auto insert_info = container.insert({key_gen(gen_), val_gen(gen_)});
                while (!insert_info.second) {
                    insert_info = container.insert({key_gen(gen_), val_gen(gen_)});
                }
            }

            return container;
        }
    };  // class Generator

}  // namespace gen
