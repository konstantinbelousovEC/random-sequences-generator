// @copyright Copyright (c) 2023. Created by Konstantin Belousov.
// All rights reserved.

#pragma once

#include <random>
#include <stdexcept>
#include <string>

namespace gen::val_generators {

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
        StringValueGenerator(str_sz min_size, str_sz max_size, std::string char_collection);

        [[nodiscard]] size_t get_value_range() const;

        template<typename BitGen>
        std::string operator()(BitGen& gen);

    };  // struct StringValueGenerator

    template<typename BitGen>
    std::string StringValueGenerator::operator()(BitGen& gen) {
        str_sz str_size = str_size_dist_(gen);

        std::string rand_str;
        rand_str.reserve(str_size);

        for (str_sz i = 0; i < str_size; ++i) {
            rand_str += char_collection_[char_dist_(gen)];
        }

        return rand_str;
    }

}  // namespace gen::val_generators
