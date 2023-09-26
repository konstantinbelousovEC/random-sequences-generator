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

#include "generator.h"

int main() {

    gen::Generator generator;

    // -----------------------------------------------------------------------------------------------------------------

    {
        constexpr size_t v_size = 10;
        constexpr int minimum = 0;
        constexpr int maximum = 1024;

        gen::SequentialContainer auto rand_vector = generator.generate<std::vector<int>>(v_size, {minimum, maximum});
        assert(rand_vector.size() == v_size);
        assert(*(std::min_element(rand_vector.begin(), rand_vector.end())) >= minimum);
        assert(*(std::max_element(rand_vector.begin(), rand_vector.end())) <= maximum);
    }
    {
        constexpr size_t v_size = 10;
        constexpr unsigned char minimum = 65;
        constexpr unsigned char maximum = 126;

        gen::SequentialContainer auto rand_char_vector = generator.generate<std::vector<unsigned char >>(v_size, {minimum, maximum});
        assert(rand_char_vector.size() == v_size);
        assert(*(std::min_element(rand_char_vector.begin(), rand_char_vector.end())) >= minimum);
        assert(*(std::max_element(rand_char_vector.begin(), rand_char_vector.end())) <= maximum);
    }
    {
        constexpr size_t v_size = 10;
        constexpr int minimum = -5000;
        constexpr int maximum = -4000;

        gen::SequentialContainer auto rand_deque = generator.generate<std::deque<int>>(v_size, {minimum, maximum});
        assert(rand_deque.size() == v_size);
        assert(*(std::min_element(rand_deque.begin(), rand_deque.end())) >= minimum);
        assert(*(std::max_element(rand_deque.begin(), rand_deque.end())) <= maximum);
    }
    {
        constexpr size_t v_size = 100;
        constexpr double minimum = -0.1;
        constexpr double maximum = 0.1;

        gen::StaticArray auto rand_array = generator.generate<std::array<double, v_size>>({minimum, maximum});
        assert(rand_array.size() == v_size);
        assert(*(std::min_element(rand_array.begin(), rand_array.end())) >= minimum);
        assert(*(std::max_element(rand_array.begin(), rand_array.end())) <= maximum);
    }
    {
        constexpr size_t v_size = 10;
        constexpr char minimum = 'A';
        constexpr char maximum = 'Z';

        gen::SequentialContainer auto rand_list = generator.generate<std::list<unsigned char>>(v_size, {minimum, maximum});
        assert(rand_list.size() == v_size);
        assert(*(std::min_element(rand_list.begin(), rand_list.end())) >= minimum);
        assert(*(std::max_element(rand_list.begin(), rand_list.end())) <= maximum);
    }
    {
        constexpr size_t v_size = 10;
        constexpr int minimum = -100;
        constexpr int maximum = 150;

        gen::SequentialContainer auto rand_forward_list = generator.generate<std::forward_list<int>>(v_size, {minimum, maximum});
        assert(std::count_if(rand_forward_list.begin(), rand_forward_list.end(), [](auto){return true;}) == v_size);
        assert(*(std::min_element(rand_forward_list.begin(), rand_forward_list.end())) >= minimum);
        assert(*(std::max_element(rand_forward_list.begin(), rand_forward_list.end())) <= maximum);
    }

    // -----------------------------------------------------------------------------------------------------------------

    {
        constexpr size_t v_size = 10;
        constexpr int minimum = 0;
        constexpr int maximum = 9;

        gen::SetContainer auto rand_set = generator.generate<std::set<int>>(v_size, {minimum, maximum});
        assert(rand_set.size() == v_size);
        assert(*(std::min_element(rand_set.begin(), rand_set.end())) >= minimum);
        assert(*(std::max_element(rand_set.begin(), rand_set.end())) <= maximum);
    }
    {
        constexpr size_t v_size = 10;
        constexpr int minimum = -100;
        constexpr int maximum = 150;

        gen::SetContainer auto rand_unordered_set = generator.generate<std::unordered_set<int>>(v_size, {minimum, maximum});
        assert(rand_unordered_set.size() == v_size);
        assert(*(std::min_element(rand_unordered_set.begin(), rand_unordered_set.end())) >= minimum);
        assert(*(std::max_element(rand_unordered_set.begin(), rand_unordered_set.end())) <= maximum);
    }

    // -----------------------------------------------------------------------------------------------------------------

    {
        constexpr size_t v_size = 32;
        constexpr int minimum_key = -100;
        constexpr int maximum_key = 150;
        constexpr float minimum_value = 10.0;
        constexpr float maximum_value = 11.0;

        gen::MapContainer auto rand_map = generator.generate<std::map<int, float>>(v_size, {minimum_key, maximum_key}, {minimum_value, maximum_value});
        assert(rand_map.size() == v_size);
        assert(std::min_element(rand_map.begin(), rand_map.end())->first >= minimum_key);
        assert(std::min_element(rand_map.begin(), rand_map.end())->second >= minimum_value);
        assert(std::max_element(rand_map.begin(), rand_map.end())->first <= maximum_key);
        assert(std::max_element(rand_map.begin(), rand_map.end())->second <= maximum_value);
    }
    {
        constexpr size_t v_size = 32;
        constexpr int minimum_key = -100;
        constexpr int maximum_key = 150;
        constexpr float minimum_value = 10.0;
        constexpr float maximum_value = 11.0;

        gen::MapContainer auto rand_unordered_map = generator.generate<std::unordered_map<int, float>>(v_size, {minimum_key, maximum_key}, {minimum_value, maximum_value});
        assert(rand_unordered_map.size() == v_size);
        assert(std::min_element(rand_unordered_map.begin(), rand_unordered_map.end())->first >= minimum_key);
        assert(std::min_element(rand_unordered_map.begin(), rand_unordered_map.end())->second >= minimum_value);
        assert(std::max_element(rand_unordered_map.begin(), rand_unordered_map.end())->first <= maximum_key);
        assert(std::max_element(rand_unordered_map.begin(), rand_unordered_map.end())->second <= maximum_value);
    }

    // -----------------------------------------------------------------------------------------------------------------

    return 0;
}
