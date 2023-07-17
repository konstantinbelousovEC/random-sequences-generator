#pragma once

#include <type_traits>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>

template <typename Container>
constexpr auto test_push_back(typename Container::value_type) -> decltype(std::declval<Container>().push_back(std::declval<typename Container::value_type>()), std::true_type{});

template <typename Container>
constexpr auto test_push_back(...) -> std::false_type;

template <typename Container>
constexpr bool has_push_back_method() {
    return decltype(test_push_back<Container>(typename Container::value_type{}))::value;
}

// --------------------------------------------------------------------------------------------------------------------

template <typename Container>
constexpr auto test_push_front(typename Container::value_type) -> decltype(std::declval<Container>().push_front(std::declval<typename Container::value_type>()), std::true_type{});

template <typename Container>
constexpr auto test_push_front(...) -> std::false_type;

template <typename Container>
constexpr bool has_push_front_method() {
    return decltype(test_push_front<Container>(typename Container::value_type{}))::value;
}

// --------------------------------------------------------------------------------------------------------------------

template <typename Container>
constexpr auto test_insert(typename Container::value_type) -> decltype(std::declval<Container>().insert(std::declval<typename Container::value_type>()), std::true_type{});

template <typename Container>
constexpr auto test_insert(...) -> std::false_type;

template <typename Container>
constexpr bool has_insert_method() {
    return decltype(test_insert<Container>(typename Container::value_type{}))::value;
}

// --------------------------------------------------------------------------------------------------------------------

template <typename Container>
constexpr auto test_reserve(typename Container::size_type) -> decltype(std::declval<Container>().reserve(std::declval<typename Container::size_type>()), std::true_type{});

template <typename Container>
constexpr auto test_reserve(...) -> std::false_type;

template <typename Container>
constexpr bool has_reserve_method() {
    return decltype(test_reserve<Container>(typename Container::size_type{}))::value;
}

// --------------------------------------------------------------------------------------------------------------------

template <typename T>
struct is_std_string : std::false_type {};

template <>
struct is_std_string<std::string> : std::true_type {};

template <typename T>
constexpr bool is_std_string_v = is_std_string<T>::value;

// --------------------------------------------------------------------------------------------------------------------

template <typename T>
struct is_std_vector : std::false_type {};

template <typename U>
struct is_std_vector<std::vector<U>> : std::true_type {};

template <typename T>
constexpr bool is_std_vector_v = is_std_vector<T>::value;


// --------------------------------------------------------------------------------------------------------------------

template <typename T>
struct is_std_deque : std::false_type {};

template <typename U>
struct is_std_deque<std::deque<U>> : std::true_type {};

template <typename T>
constexpr bool is_std_deque_v = is_std_deque<T>::value;

// --------------------------------------------------------------------------------------------------------------------

template <typename T>
struct is_std_list : std::false_type {};

template <typename U>
struct is_std_list<std::list<U>> : std::true_type {};

template <typename T>
constexpr bool is_std_list_v = is_std_list<T>::value;

// --------------------------------------------------------------------------------------------------------------------

template <typename T>
struct is_std_forward_list : std::false_type {};

template <typename U>
struct is_std_forward_list<std::forward_list<U>> : std::true_type {};

template <typename T>
constexpr bool is_std_forward_list_v = is_std_forward_list<T>::value;

// --------------------------------------------------------------------------------------------------------------------

template <typename T>
struct is_std_seq_container : std::false_type {};

template <typename T>
struct is_std_seq_container<std::vector<T>> : std::true_type {};

template <typename T>
struct is_std_seq_container<std::deque<T>> : std::true_type {};

template <typename T>
struct is_std_seq_container<std::list<T>> : std::true_type {};

template <typename T>
struct is_std_seq_container<std::forward_list<T>> : std::true_type {};

template <typename T>
constexpr bool is_std_seq_container_v = is_std_seq_container<T>::value;
