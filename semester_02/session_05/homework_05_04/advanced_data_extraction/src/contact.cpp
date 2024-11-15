// contact.cpp

#include "../includes/contact.h"

#include <tuple>

namespace phone
{
	using pair_type = contact::pair_type;
	using string_type = contact::string_type;
	using vector_type = contact::vector_type;

	contact::contact(const pair_type& name, const string_type& email, const vector_type& numbers)
	{
		// Здесь должна быть выполнена проверка формата данных
		name_ = name;
		email_ = email;
		numbers_ = numbers;
	}

	std::tuple<pair_type, string_type, vector_type> phone::contact::get() const
	{
		return  std::make_tuple(name_, email_, numbers_);
	}
}