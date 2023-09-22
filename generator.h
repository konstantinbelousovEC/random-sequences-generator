#pragma once

#include <random>

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

        template<typename T> requires std::is_arithmetic_v<T>
        struct ArithmeticValueGenerator final {
         private:
            T min_ = std::numeric_limits<T>::min();
            T max_ = std::numeric_limits<T>::max();

         public:
            ArithmeticValueGenerator() = default;

            ArithmeticValueGenerator(T min, T max) : min_(min), max_(max) {
                if (max_ < min_) std::swap(max_, min_);
            }

            T get_value_range() const noexcept {
                return static_cast<T>(std::abs(max_ - min_));
            }

            template<typename BitGen>
            T operator()(BitGen& gen) {
                if constexpr (std::is_integral_v<T>) {
                    return std::uniform_int_distribution<T>{min_, max_}(gen);
                } else {
                    return std::uniform_real_distribution<T>{min_, max_}(gen);
                }
            }

        };  // struct ArithmeticValueGenerator

        struct StringValueGenerator final {
         private:
            using str_sz = std::string::size_type;

         private:
            str_sz min_sz_;
            str_sz max_sz_;
            std::string char_coll_;

         public:
            StringValueGenerator(str_sz min_size, str_sz max_size, std::string char_coll)
                    : min_sz_(min_size), max_sz_(max_size), char_coll_(std::move(char_coll)) {}

            template<typename BitGen>
            std::string operator()(BitGen& gen) {
                std::uniform_int_distribution<str_sz> str_size_dist(min_sz_, max_sz_);
                str_sz str_size = str_size_dist(gen);

                std::string rand_str;
                rand_str.reserve(str_size);

                std::uniform_int_distribution<str_sz> dist(0, char_coll_.size() - 1);

                for (str_sz i = 0; i < str_size; ++i) {
                    rand_str += char_coll_[dist(gen)];
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
        Container generate(size_type size, ValueGenerator val_gen = {})
        {
            Container container(size);
            std::generate(container.begin(), container.end(), [&]() { return val_gen(gen_); });
            return container;
        }

        template<StaticArray Container,
                 typename ValueGenerator = generators::ArithmeticValueGenerator<typename Container::value_type>>
        Container generate(ValueGenerator val_gen = {})
        {
            Container container;
            std::generate(container.begin(), container.end(), [&]() { return val_gen(gen_); });
            return container;
        }


        template<SetContainer Container,
                 typename size_type = typename Container::size_type,
                 typename ValueGenerator = generators::ArithmeticValueGenerator<typename Container::value_type>>
        Container generate(size_type size, ValueGenerator key_gen = {})
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
        Container generate(size_type size, KeyGenerator key_gen = {}, ValueGenerator val_gen = {})
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
