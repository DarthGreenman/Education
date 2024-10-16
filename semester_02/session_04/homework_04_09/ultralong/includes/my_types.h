// my_types.h

#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <string>
#include <type_traits>

namespace my
{
	template<typename Char_type>
	constexpr auto offset()
	{
		return std::char_traits<Char_type>::to_int_type('0');
	}

	template<typename Char_type>
	constexpr auto to_numeric(Char_type ch)
	{
		return std::char_traits<Char_type>::to_int_type(ch) - offset<Char_type>();
	}

	template<typename Char_type>
	constexpr auto to_char(int numeric)
	{
		return std::char_traits<Char_type>::to_char_type(numeric + offset<Char_type>());
	}

	template<typename T>
	struct is_chars
	{
		static const auto value = std::is_same_v<T, char> || std::is_same_v<T, wchar_t> ||
			std::is_same_v<T, char8_t> || std::is_same_v<T, char16_t> || std::is_same_v<T, char32_t>;
	};
	template<typename T>
	constexpr auto is_chars_v = is_chars<T>::value;

	template<typename T>
	struct is_strings
	{
		static const auto value = std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring> ||
			std::is_same_v<T, std::u8string> || std::is_same_v<T, std::u16string> || std::is_same_v<T, std::u32string>;
	};
	template<typename T>
	constexpr auto is_strings_v = is_strings<T>::value;
}
#endif // !MY_TYPES_H