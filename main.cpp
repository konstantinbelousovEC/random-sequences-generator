// @copyright Copyright (c) 2023. Created by Konstantin Belousov.
// All rights reserved.

#include <vector>
#include <deque>
#include <array>
#include <list>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <regex>
#include "generator.h"
#include "value-generators.h"

int main() {

    gen::Generator generator;

    // -----------------------------------------------------------------------------------------------------------------

    {
        constexpr size_t v_size = 10;
        constexpr int minimum = 0;
        constexpr int maximum = 1024;

        gen::SequentialContainer auto rand_vector =
                generator.generate<std::vector<int>>(v_size, {minimum, maximum});

        assert(rand_vector.size() == v_size);
        assert(*(std::min_element(rand_vector.cbegin(), rand_vector.cend())) >= minimum);
        assert(*(std::max_element(rand_vector.cbegin(), rand_vector.cend())) <= maximum);
    }
    {
        constexpr size_t v_size = 10;
        constexpr unsigned char minimum = 65;
        constexpr unsigned char maximum = 126;

        gen::SequentialContainer auto rand_char_vector =
                generator.generate<std::vector<unsigned char>>(v_size, {minimum, maximum});

        assert(rand_char_vector.size() == v_size);
        assert(*(std::min_element(rand_char_vector.cbegin(), rand_char_vector.cend())) >= minimum);
        assert(*(std::max_element(rand_char_vector.cbegin(), rand_char_vector.cend())) <= maximum);
    }
    {
        constexpr size_t v_size = 10;
        constexpr int minimum = -5000;
        constexpr int maximum = -4000;

        gen::SequentialContainer auto rand_deque =
                generator.generate<std::deque<int>>(v_size, {minimum, maximum});

        assert(rand_deque.size() == v_size);
        assert(*(std::min_element(rand_deque.cbegin(), rand_deque.cend())) >= minimum);
        assert(*(std::max_element(rand_deque.cbegin(), rand_deque.cend())) <= maximum);
    }
    {
        constexpr size_t v_size = 100;
        constexpr double minimum = -0.1;
        constexpr double maximum = 0.1;

        gen::StaticArray auto rand_array =
                generator.generate<std::array<double, v_size>>({minimum, maximum});

        assert(rand_array.size() == v_size);
        assert(*(std::min_element(rand_array.cbegin(), rand_array.cend())) >= minimum);
        assert(*(std::max_element(rand_array.cbegin(), rand_array.cend())) <= maximum);
    }
    {
        constexpr size_t v_size = 10;
        constexpr char minimum = 'A';
        constexpr char maximum = 'Z';

        gen::SequentialContainer auto rand_list =
                generator.generate<std::list<unsigned char>>(v_size, {minimum, maximum});

        assert(rand_list.size() == v_size);
        assert(*(std::min_element(rand_list.cbegin(), rand_list.cend())) >= minimum);
        assert(*(std::max_element(rand_list.cbegin(), rand_list.cend())) <= maximum);
    }
    {
        constexpr size_t v_size = 10;
        constexpr int minimum = -100;
        constexpr int maximum = 150;

        gen::SequentialContainer auto rand_forward_list =
                generator.generate<std::forward_list<int>>(v_size, {minimum, maximum});

        assert(std::count_if(rand_forward_list.cbegin(), rand_forward_list.cend(), [](auto){return true;}) == v_size);
        assert(*(std::min_element(rand_forward_list.cbegin(), rand_forward_list.cend())) >= minimum);
        assert(*(std::max_element(rand_forward_list.cbegin(), rand_forward_list.cend())) <= maximum);
    }

    // -----------------------------------------------------------------------------------------------------------------

    {
        constexpr size_t v_size = 10;
        constexpr int minimum = 0;
        constexpr int maximum = 9;

        gen::SetContainer auto rand_set = generator.generate<std::set<int>>(v_size, {minimum, maximum});

        assert(rand_set.size() == v_size);
        assert(*(std::min_element(rand_set.cbegin(), rand_set.cend())) >= minimum);
        assert(*(std::max_element(rand_set.cbegin(), rand_set.cend())) <= maximum);
    }
    {
        constexpr size_t v_size = 10;
        constexpr int minimum = -100;
        constexpr int maximum = 150;

        gen::SetContainer auto rand_unordered_set =
                generator.generate<std::unordered_set<int>>(v_size, {minimum, maximum});

        assert(rand_unordered_set.size() == v_size);
        assert(*(std::min_element(rand_unordered_set.cbegin(), rand_unordered_set.cend())) >= minimum);
        assert(*(std::max_element(rand_unordered_set.cbegin(), rand_unordered_set.cend())) <= maximum);
    }

    // -----------------------------------------------------------------------------------------------------------------

    {
        constexpr size_t v_size = 32;
        constexpr int minimum_key = -100;
        constexpr int maximum_key = 150;
        constexpr float minimum_value = 10.0;
        constexpr float maximum_value = 11.0;

        gen::MapContainer auto rand_map =
                generator.generate<std::map<int, float>>(v_size,
                                                         {minimum_key, maximum_key},
                                                         {minimum_value, maximum_value});

        assert(rand_map.size() == v_size);
        assert(std::min_element(rand_map.cbegin(), rand_map.cend())->first >= minimum_key);
        assert(std::min_element(rand_map.cbegin(), rand_map.cend())->second >= minimum_value);
        assert(std::max_element(rand_map.cbegin(), rand_map.cend())->first <= maximum_key);
        assert(std::max_element(rand_map.cbegin(), rand_map.cend())->second <= maximum_value);
    }
    {
        constexpr size_t v_size = 32;
        constexpr int minimum_key = -100;
        constexpr int maximum_key = 150;
        constexpr float minimum_value = 10.0;
        constexpr float maximum_value = 11.0;

        gen::MapContainer auto rand_unordered_map =
                generator.generate<std::unordered_map<int, float>>(v_size,
                                                                   {minimum_key, maximum_key},
                                                                   {minimum_value, maximum_value});

        assert(rand_unordered_map.size() == v_size);
        assert(std::min_element(rand_unordered_map.cbegin(), rand_unordered_map.cend())->first >= minimum_key);
        assert(std::min_element(rand_unordered_map.cbegin(), rand_unordered_map.cend())->second >= minimum_value);
        assert(std::max_element(rand_unordered_map.cbegin(), rand_unordered_map.cend())->first <= maximum_key);
        assert(std::max_element(rand_unordered_map.cbegin(), rand_unordered_map.cend())->second <= maximum_value);
    }

    // -----------------------------------------------------------------------------------------------------------------

    {
        constexpr size_t v_size = 10;
        gen::val_gens::StringValueGenerator str_gen{5, 10, "abcde"};

        gen::SequentialContainer auto rand_vector =
                generator.generate<std::vector<std::string>>(v_size, str_gen);

        assert(rand_vector.size() == v_size);

        std::regex pattern("[abcde]{5,10}");
        for (const auto& str : rand_vector) {
            assert(std::regex_match(str, pattern));
        }
    }

    {
        constexpr size_t v_size = 10;
        gen::val_gens::StringValueGenerator str_gen{5, 10, "abc12"};

        gen::MapContainer auto rand_map =
                generator.generate<std::map<std::string, std::string>>(v_size, str_gen, str_gen);

        assert(rand_map.size() == v_size);

        std::regex pattern("[abc12]{5,10}");
        for (auto&& [key, value] : rand_map) {
            assert(std::regex_match(key, pattern));
            assert(std::regex_match(value, pattern));
        }
    }

    // -----------------------------------------------------------------------------------------------------------------

    return 0;
}
