// types.h

#include "../includes/my_ascii.h"
#include "../includes/types.h"

#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace my
{
	// 	class person_name //////////////////////////////////////////////////////////////////////////////////////////////////

	person_name::person_name(const std::string& forename, const std::string& surname)
	{
		using namespace std;
		if (const auto pos_n = find_if(cbegin(forename), cend(forename), [](char ch) { return my::is_digit(ch); }),
			pos_s = find_if(cbegin(surname), cend(surname), [](char ch) { return my::is_digit(ch); });
			pos_n != cend(forename) || pos_s != cend(surname))
		{
			throw invalid_argument{ "\nUnresolved character - value must be a letter.\nline: " +
				to_string(__LINE__) + ", file:\n" + std::string{ __FILE__ } + '\n' };
		}

		forename_ = forename;
		surname_ = surname;
	}

	std::pair<std::string, std::string> person_name::get() const
	{
		return std::make_pair(forename_, surname_);
	}

	// class email_address //////////////////////////////////////////////////////////////////////////////////////////////////
	std::string email_address::get() const
	{
		return std::string();
	}


	// class phone_number //////////////////////////////////////////////////////////////////////////////////////////////////
	
	phone_number::phone_number(const std::string& number)
	{		
		normalization(number); // приведение к нормализованному виду
		
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
		
		std::string pre_normalized_number{};
		pre_normalized_number.reserve(number.size());

		// Копируем только цифры и '+'
		copy_if(cbegin(number), cend(number), back_inserter(pre_normalized_number), [](char ch)
			{ return my::is_digit(ch) || ch == plus; });
		
		// Проверяем длину номера, должен быть 12 символов: +19792195004
		if (const auto number_size = code_sizes::country + code_sizes::zone + code_sizes::node + code_sizes::number;
			pre_normalized_number.size() != number_size)
		{
			throw length_error{ "\nInsufficient number of digits of the number.\nline: " +
				to_string(__LINE__) + ", file:\n" + std::string{ __FILE__ } + '\n' };
		}
		
		pre_normalized_number.shrink_to_fit();
		normalized_number_ = pre_normalized_number;
	}
}