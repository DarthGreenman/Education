// my_ultralong_ext.h

#ifndef MY_ULTRALONG_EXT_H
#define MY_ULTRALONG_EXT_H

#include "my_types.h"
#include "my_ultralong.h"
#include "my_utilities.h"

#include <algorithm>
#include <bitset>
#include <iosfwd>
#include <iterator>
#include <string>
#include <type_traits>

namespace my
{
	namespace helper
	{
		template<typename String_type = std::string, std::size_t N,
			typename = std::enable_if_t<my::is_strings_v<String_type>>>
		auto to_string_raw(const std::bitset<N>& number, std::back_insert_iterator<String_type> it, int offset)
		{
			if (offset < 0)
				return;

			using properties_numeric = typename ultralong<N>::properties_numeric;
			const auto numeric = bit::to_numeric(number >> offset & properties_numeric::lsb);
			*it = to_char<typename String_type::value_type>(numeric);

			to_string_raw(number, it, offset -= properties_numeric::width);
		}
	}

	// 1. Создаем пустую строку.
	// 2. Резервируем память для необходимого числа разрядов и знака.
	// 3. Устанавливаем знак числа, если отрицательное, то '-'.
	// 4. Получаем строку, если необходимо, то удаляем лидирующие нули.

	template<typename String_type = std::string, std::size_t N>
		requires Strings<String_type>
	auto to_string(const ultralong<N>& number)
	{
		using namespace std;
		using properties_numeric = typename ultralong<N>::properties_numeric;

		String_type number_as_string{};
		number_as_string.reserve(N / properties_numeric::width);
		if (ultralong<N>::is_negative(number.get()))
			number_as_string.push_back('-');

		helper::to_string_raw(number.get(), back_insert_iterator<decltype(number_as_string)>(number_as_string), 
			N - properties_numeric::width * 2ull);

		if (const auto not_zero = find_if_not(ultralong<N>::is_negative(number.get()) ? next(cbegin(number_as_string)) :
			cbegin(number_as_string), cend(number_as_string),
			[](typename iterator_traits<decltype(cbegin(number_as_string))>::value_type elem)
			{
				return elem == static_cast<decltype(elem)>('0');
			}
		); not_zero != cend(number_as_string))
		{
			number_as_string.erase(ultralong<N>::is_negative(number.get()) ? next(cbegin(number_as_string)) :
				cbegin(number_as_string), not_zero);
			number_as_string.shrink_to_fit();
		}
		return number_as_string;
	}
	
	template<std::size_t N>
	std::ostream& operator<<(std::ostream& os, const ultralong<N>& number)
	{
		os << to_string(number);
		return os;
	}
}
#endif // !MY_ULTRALONG_EXT_H