// types.h

#include "../includes/my_ascii.h"
#include "../includes/my_utilities.h"
#include "../includes/types.h"

#include <algorithm>
#include <exception>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>

namespace my
{
	// class email_address //////////////////////////////////////////////////////////////////////////////////////////////////
	email_address::email_address(const std::string& address)
	{
		std::istringstream tmp{ address };
		const auto list = my::split(tmp, ascii::at_sign);
		mailbox_ = "";  hostname_ = "";
	}
	
	std::string email_address::get() const
	{
		return std::string();
	}


	// class phone_number //////////////////////////////////////////////////////////////////////////////////////////////////
	phone_number::phone_number(const std::string& number)
	{		
		try	{
			normalization(number); // Приводим к нормализованному виду
			check(); // Проверяем длину номера, должен быть 12 символов: +19792195004

		}
		catch (const std::exception& err) {
			throw err;
		}

		using namespace std;
		const vector<std::size_t> code_sizes{ code_sizes::country, code_sizes::zone, code_sizes::node, code_sizes::number };
		codes_.reserve(code_sizes.size());

		auto pos = 0ull;
		for_each(begin(code_sizes), end(code_sizes),
			[this, &pos](const typename iterator_traits<decltype(begin(code_sizes))>::value_type size)
			{
				string code{};
				copy_n(next(begin(normalized_number_), pos), size, back_inserter(code)); pos += size;
				codes_.push_back(code);
			}
		);
	}

	std::string phone_number::get() const
	{
		return normalized_number_;
	}

	void phone_number::normalization(const std::string& number)
	{
		using namespace std;
		using namespace ascii;
		
		normalized_number_.reserve(number.size());
		// Копируем только цифры и '+'
		copy_if(cbegin(number), cend(number), back_inserter(normalized_number_), [](char ch)
			{ return my::is_digit(ch) || ch == plus; });
	}
	void phone_number::check() const
	{
		// Проверяем длину номера, должен быть 12 символов: +19792195004
		using namespace std;
		
		if (const auto number_size = code_sizes::country + code_sizes::zone + code_sizes::node + code_sizes::number;
			normalized_number_.size() != number_size)
		{
			throw length_error{ "\nInsufficient number of digits of the number.\nline: " +
				to_string(__LINE__) + ", file:\n" + string{ __FILE__ } + '\n' };
		}
	}
}