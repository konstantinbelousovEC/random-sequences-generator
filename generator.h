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
    };

    // key-only associative containers: set, unordered_set
    template <typename Container>
    concept SetContainer = requires(Container cont) {
        typename Container::key_type;
        requires !requires {
            typename Container::mapped_type;
        };
    };

    // -----------------------------------------------------------------------------------------------------------------

    namespace generators {

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
            : min_(min), max_(max), distribution_(min_, max_)
            {
                if (max_ < min_) std::swap(max_, min_);
            }

            [[nodiscard]] size_t get_value_range() const noexcept {
                return static_cast<T>(std::abs(max_ - min_));
            }

            template<typename BitGen>
            T operator()(BitGen& gen) {
                return distribution_(gen);
            }

        };  // struct ArithmeticValueGenerator

        struct StringValueGenerator final {
         private:
            using str_sz = std::string::size_type;

         private:
            str_sz min_sz_;
            str_sz max_sz_;
            std::string char_collection_;
            std::uniform_int_distribution<str_sz> char_dist_;
            std::uniform_int_distribution<str_sz> str_size_dist_;

         public:
            StringValueGenerator(str_sz min_size, str_sz max_size, std::string char_collection)
                    : min_sz_(min_size),
                      max_sz_(max_size),
                      char_collection_(std::move(char_collection)),
                      char_dist_(0, char_collection_.size() - 1),
                      str_size_dist_(min_sz_, max_sz_) {}

            [[nodiscard]] size_t get_value_range() const noexcept {
                size_t collection_size = char_collection_.size();
                
                size_t amount_of_possible_strings = 0;
                size_t max_value = std::numeric_limits<size_t>::max();
                
                for (size_t i = min_sz_; i <= max_sz_; i++) {
                    if (amount_of_possible_strings > max_value / collection_size) return max_value;
                    amount_of_possible_strings += static_cast<size_t>(std::pow(collection_size, i));
                }

                return amount_of_possible_strings;
            }

            template<typename BitGen>
            std::string operator()(BitGen& gen) {
                str_sz str_size = str_size_dist_(gen);

                std::string rand_str;
                rand_str.reserve(str_size);

                for (str_sz i = 0; i < str_size; ++i) {
                    rand_str += char_collection_[char_dist_(gen)];
                }

                return rand_str;
            }

        };  // struct StringValueGenerator

    }  // namespace generators

    class Generator final {
     private:
        std::random_device rd_;
        std::mt19937 gen_;

     public:
        Generator() : rd_(), gen_(rd_()) {}

        template<SequentialContainer Container,
                 typename size_type = typename Container::size_type,
                 typename ValueGenerator = generators::ArithmeticValueGenerator<typename Container::value_type>>
        Container generate(size_type size, ValueGenerator val_gen)
        {
            Container container(size);
            std::generate(container.begin(), container.end(), [&]() { return val_gen(gen_); });
            return container;
        }

        template<StaticArray Container,
                 typename ValueGenerator = generators::ArithmeticValueGenerator<typename Container::value_type>>
        Container generate(ValueGenerator val_gen)
        {
            Container container;
            std::generate(container.begin(), container.end(), [&]() { return val_gen(gen_); });
            return container;
        }


        template<SetContainer Container,
                 typename size_type = typename Container::size_type,
                 typename ValueGenerator = generators::ArithmeticValueGenerator<typename Container::value_type>>
        Container generate(size_type size, ValueGenerator key_gen)
        {
            if constexpr (std::is_integral_v<typename Container::value_type>) {
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
                 typename KeyGenerator = generators::ArithmeticValueGenerator<typename Container::key_type>,
                 typename ValueGenerator = generators::ArithmeticValueGenerator<typename Container::mapped_type>>
        Container generate(size_type size, KeyGenerator key_gen, ValueGenerator val_gen)
        {
            if constexpr (std::is_integral_v<typename Container::key_type>) {
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
