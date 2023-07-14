#pragma once

#include <type_traits>

template <typename Container>
constexpr auto test_push_back(int) -> decltype(std::declval<Container>().push_back(std::declval<typename Container::size_type>()), std::true_type{});

template <typename Container>
constexpr auto test_push_back(...) -> std::false_type;

template <typename Container>
constexpr bool has_push_back_method() {
    return decltype(test_push_back<Container>(0))::value;
}

// --------------------------------------------------------------------------------------------------------------------

template <typename Container>
constexpr auto test_push_front(int) -> decltype(std::declval<Container>().push_front(std::declval<typename Container::size_type>()), std::true_type{});

template <typename Container>
constexpr auto test_push_front(...) -> std::false_type;

template <typename Container>
constexpr bool has_push_front_method() {
    return decltype(test_push_front<Container>(0))::value;
}

// --------------------------------------------------------------------------------------------------------------------

template <typename Container>
constexpr auto test_insert(int) -> decltype(std::declval<Container>().insert(std::declval<typename Container::size_type>()), std::true_type{});

template <typename Container>
constexpr auto test_insert(...) -> std::false_type;

template <typename Container>
constexpr bool has_insert_method() {
    return decltype(test_insert<Container>(0))::value;
}

// --------------------------------------------------------------------------------------------------------------------

template <typename Container>
constexpr auto test_reserve(int) -> decltype(std::declval<Container>().reserve(std::declval<typename Container::size_type>()), std::true_type{});

template <typename Container>
constexpr auto test_reserve(...) -> std::false_type;

template <typename Container>
constexpr bool has_reserve_method() {
    return decltype(test_reserve<Container>(0))::value;
}

// --------------------------------------------------------------------------------------------------------------------