// utilities.h

#ifndef UTILITIES_H
#define UTILITIES_H

#include <stddef.h>
#include <utility>

namespace seq{
	template<typename T> const T sum(const T*, const T*);
    template<typename T> void swap(T&, T&);
    template<typename T> void sort(T*, T*);
    template<typename T> void reverse(T*, T*);
}

namespace seq {
    template<typename T>
    const T sum(const T* begin, const T* end)
    {
        T sum{};
        for (auto it = begin; it != end; ++it) {
            sum += *it;
        }
        return sum;
    }

    template<typename T>
    void swap(T& lhs, T& rhs)
    {
        if (&lhs != &rhs) {
            T tmp{ lhs };
            lhs = rhs; rhs = tmp;
        }
    }

    template<typename T>
    void sort(T* begin, T* end)
    {
        for (auto take = begin; take != end; ++take) {
            bool is_sorted{ true };
            const auto diff = end - take;
            const auto fin = begin - 1 + diff;

            for (auto ptr = begin; ptr != fin; ++ptr) {
                if (*ptr > *(ptr + 1)) {
                    seq::swap(*ptr, *(ptr + 1));
                    is_sorted = false;
                }
            }
            if (is_sorted) {
                break;
            }
        }
    }

    template<typename T>
    void reverse(T* begin, T* end) 
    {
        auto ps = begin, pt = end;
        const std::ptrdiff_t N{ (end - begin) / 2 };
        for (size_t i{}; i < static_cast<size_t>(N); ++i, ++ps, --pt) {
            if (*ps != *(pt - 1)) {
                swap(*ps, *(pt - 1));
            }
        }
    }
}
#endif /* UTILITIES_H */

