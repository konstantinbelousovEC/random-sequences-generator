#pragma once

#include <type_traits>

template <typename Container>
constexpr auto TestPushBack(int) -> decltype(std::declval<Container>().push_back(std::declval<typename Container::size_type>()), std::true_type{});

template <typename Container>
constexpr auto TestPushBack(...) -> std::false_type;

template <typename Container>
constexpr bool HasPushBackMethod() {
    return decltype(TestPushBack<Container>(0))::value;
}

template <typename Container>
constexpr auto TestReserve(int) -> decltype(std::declval<Container>().reserve(std::declval<typename Container::size_type>()), std::true_type{});

template <typename Container>
constexpr auto TestReserve(...) -> std::false_type;

template <typename Container>
constexpr bool has_reserve_method() {
    return decltype(TestReserve<Container>(0))::value;
}