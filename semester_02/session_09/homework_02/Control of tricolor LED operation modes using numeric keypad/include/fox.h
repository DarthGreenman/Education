// fox.h

#ifndef FOX_H_FOR_THE_ARDUINO_UNO_PROJECT
#define FOX_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <stddef.h>

namespace fox {
struct expand_parameter_package {
    template <typename... T>
    expand_parameter_package(T...) { }
};
template <typename T, typename U>
struct pair {
    T first;
    U second;
};

template <typename T, typename U>
constexpr fox::pair<T, U> make_pair(T&& first, U&& second) { return fox::pair<T, U> { first, second }; }

template <typename T, size_t N>
size_t find(const T (&elems)[N], T elem)
{
    for (size_t n { }; n < N; ++n)
        if (elems[n] == elem)
            return n;
    return N;
}
template <typename T, typename Condition, size_t N>
size_t find_if(const T (&elems)[N], Condition cond)
{
    for (size_t n { }; n < N; ++n)
        if (cond(n))
            return n;
    return N;
}
template <typename T>
float map(T x, T in_min, T in_max, T out_min, T out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <class T, class U>
struct is_same;

template <class T>
struct is_same<T, T> {
    static constexpr bool value = true;
};

template <class T, class U>
struct is_same {
    static constexpr bool value = false;
};

template <bool B, typename T = void>
struct enable_if;

template <class T>
struct enable_if<true, T> {
    typedef T type;
};
} // namespace helper

#endif // !FOX_H_FOR_THE_ARDUINO_UNO_PROJECT