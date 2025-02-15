// types.h

#include "../includes/my_ascii.h"
#include "../includes/my_utilities.h"
#include "../includes/types.h"

#include <algorithm>
#include <cctype>
#include <exception>
#include <iosfwd>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace my
{
	params& operator++(params& param)
	{
		param = static_cast<params>(param + 1);
		return param;
	}

	params operator++(params& param, int)
	{
		params prev{ param };
		param = static_cast<params>(param + 1);
		return prev;
	}

	params beg_params()	{ return params::first_param;	}

	params end_params()
	{
		params last = params::last_param;
		return ++last;
	}

	// class email_address //////////////////////////////////////////////////////////////////////////////////////////////////
	email_address::email_address(const std::string& address)
	{
		const auto parts = my::split(address, ascii::at_sign);
		if (parts.size() != 2ull)
		{
			using namespace std;
			throw invalid_argument{ "\nIncorrect address format.\nline: " +
				to_string(__LINE__) + ", file:\n" + string{ __FILE__ } + '\n' };
		}

		try {
			check(parts);
		}
		catch (const std::exception& err) {
			throw err;
		}
		address_.first = parts[mailbox]; address_.second = parts[hostname];
	}
	
	std::pair<std::string, std::string> email_address::get() const
	{
		return address_;
	}

	void email_address::check(const std::vector<std::string>& parts) const
	{
		for (auto param = beg_params(); param < end_params(); ++param)
		{
			try {
				check(parts, param);
			}
			catch (const std::exception& err) {
				throw err;
			}
		}
	}

	void email_address::check(const std::vector<std::string>& parts, params param) const
	{
		using namespace std;
		using namespace my::ascii;
		switch (param)
		{
		case mailbox:
			// Например: проверка имени на уникальность в пределах доступных адресов
			break;
		case hostname:
			{
				auto rule = [](char ch) { return isalpha(ch) || ch != comma; };
				if (const auto pos = find_if_not(begin(parts[hostname]), end(parts[hostname]), rule);
					pos != end(parts[hostname]))
				{
					throw invalid_argument{ "\nInvalid character in hostname.\nline: " +
						to_string(__LINE__) + ", file:\n" + string{ __FILE__ } + '\n' };
				}
				break;
			}
		}
	}

	// class phone_number //////////////////////////////////////////////////////////////////////////////////////////////////
	phone_number::phone_number(const std::string& number)
	{
		const auto normalized_number = normalization(number); // Приводим к нормализованному виду
		
		try	{
			check(normalized_number); // Проверяем длину номера, должен быть 12 символов: +19792195004
		}
		catch (const std::exception& err) {
			throw err;
		}

		using namespace std;
		const vector<std::size_t> size_codes{ size_codes::country, size_codes::zone, size_codes::node, size_codes::number };
		codes_.reserve(size_codes.size());

		auto pos = 0ull;
		for_each(begin(size_codes), end(size_codes),
			[this, &pos, &normalized_number](const typename iterator_traits<decltype(begin(size_codes))>::value_type size)
			{
				string code{};
				copy_n(next(begin(normalized_number), pos), size, back_inserter(code)); pos += size;
				codes_.push_back(code);
			}
		);
	}

	std::string phone_number::normalization() const
	{
		return std::string{ codes_[0] + codes_[1] + codes_[2] + codes_[3] };
	}

	std::tuple<std::string, std::string, std::string, std::string> phone_number::get() const
	{
		return std::make_tuple(codes_[0], codes_[1], codes_[2], codes_[3]);
	}

	std::string phone_number::normalization(const std::string& number) const
	{
		using namespace std;
		using namespace ascii;

		string normalized_number{};
		normalized_number.reserve(number.size());
		
		// Копируем только цифры и '+'
		copy_if(cbegin(number), cend(number), back_inserter(normalized_number), [](char ch)
			{ return my::is_digit(ch) || ch == plus; });
		
		return normalized_number;
	}
	void phone_number::check(const std::string& normalized_number) const
	{
		// Проверяем длину номера, должен быть 12 символов: +19792195004
		using namespace std;
		
		if (const auto number_size = size_codes::country + size_codes::zone + size_codes::node + size_codes::number;
			normalized_number.size() != number_size)
		{
			throw length_error{ "\nInsufficient number of digits of the number.\nline: " +
				to_string(__LINE__) + ", file:\n" + string{ __FILE__ } + '\n' };
		}
	}
}