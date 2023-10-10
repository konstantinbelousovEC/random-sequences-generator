// @copyright Copyright (c) 2023. Created by Konstantin Belousov.
// All rights reserved.

#include "value-generators.h"

namespace gen::val_generators {

    StringValueGenerator::StringValueGenerator(StringValueGenerator::str_sz min_size,
                                               StringValueGenerator::str_sz max_size,
                                               std::string char_collection)
        : min_sz_(min_size < max_size ? min_size : max_size),
          max_sz_(min_size < max_size ? max_size : min_size),
          char_collection_(std::move(char_collection)),
          char_dist_(0, char_collection_.size() - 1),
          str_size_dist_(min_sz_, max_sz_) {}

    size_t StringValueGenerator::get_value_range() const {
        size_t collection_size = char_collection_.size();

        size_t amount_of_possible_strings = 0;
        size_t max_value = std::numeric_limits<size_t>::max();

        for (size_t i = min_sz_; i <= max_sz_; i++) {
            if (amount_of_possible_strings > max_value / collection_size) return max_value;
            amount_of_possible_strings += static_cast<size_t>(std::pow(collection_size, i));
        }

        return amount_of_possible_strings;
    }

}  // namespace gen::val_generators
