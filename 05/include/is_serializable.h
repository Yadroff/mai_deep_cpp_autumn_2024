#pragma once

#include <type_traits>

template<typename, typename T>
struct is_serializable : std::false_type {};


template<typename C, typename T>
struct is_serializable<C, void(T)> {
private:
    template<typename U>
    static auto check(U*) -> decltype(std::declval<U>().serialize(std::declval<T>()), std::true_type{});

    template<typename>
    static std::false_type check(...);

public:
    using type = decltype(check<C>(nullptr));
    static constexpr bool value = type::value;
};

