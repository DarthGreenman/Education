// my_types.h

#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <bitset>
#include <string>
#include <type_traits>

namespace my
{
	namespace helper
	{
		template<typename Char_type>
		constexpr auto offset()
		{
			return std::char_traits<Char_type>::to_int_type('0');
		}
	}

	template<typename Char_type>
	constexpr auto to_numeric(Char_type ch)
	{
		return std::char_traits<Char_type>::to_int_type(ch) - helper::offset<Char_type>();
	}

	template<typename Char_type>
	constexpr auto to_char(int numeric)
	{
		return std::char_traits<Char_type>::to_char_type(numeric + helper::offset<Char_type>());
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

	namespace bit
	{
		template<std::size_t N>
		struct properties_numeric
		{
			static constexpr auto bit_width = 8ull;
			static constexpr std::bitset<N> lsb{ 0b0000'1111 };
			static constexpr std::bitset<N> msb{ 0b1111'0000 };
			static constexpr std::bitset<N> adj{ 0b1111'0110 };
			static auto is_adjust(const std::bitset<N>& numeric)
			{
				return numeric.to_ullong() > 9ull ? true : false;
			}
		};

		namespace helper
		{
			template<std::size_t N>
			auto to_numeric_impl(const std::bitset<N>& bit, std::size_t pos = 0ull, int numeric = 0) 
			{
				if (pos == properties_numeric<N>::bit_width)
					return numeric;
				const decltype(numeric) num{ bit.test(pos) << pos };
				return to_numeric_impl(bit, ++pos, numeric += num);
			}
		}

		template<std::size_t N>
		auto to_numeric(const std::bitset<N>& bit)
		{
			return helper::to_numeric_impl(bit);
		}	
	}
}
#endif // !MY_TYPES_H