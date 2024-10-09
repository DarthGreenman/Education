// my_types.h

#ifndef MY_LIBRARY_TYPES_H
#define MY_LIBRARY_TYPES_H

#include <iterator>
#include <type_traits>

namespace seq
{
    template<typename Iter>
    concept Input_iterator = std::input_iterator<Iter> ||
        std::is_pointer<Iter>::value;

    template<typename Iter, typename Type>
    concept Output_iterator = std::output_iterator<Iter, Type>;

    template<typename Iter>
    concept Iterator_integer_types = 
        std::is_integral<typename std::iterator_traits<Iter>::value_type>::value;
}

#endif // MY_LIBRARY_TYPES_H