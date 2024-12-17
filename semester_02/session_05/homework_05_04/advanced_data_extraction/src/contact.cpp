// contact.cpp

#include "../includes/contact.h"

#include <tuple>

namespace phone
{
	using name_type = typename contact::name_type;
	using email_list_type = typename contact::email_list_type;
	using number_list_type = typename contact::number_list_type;

	contact::contact(const name_type& name, const email_list_type& emails, const number_list_type& phone_numbers) :
		name_{ name }, emails_{ emails }, phone_numbers_{ phone_numbers }	{}

	std::tuple<name_type, email_list_type, number_list_type> phone::contact::get() const
	{
		return  std::make_tuple(name_, emails_, phone_numbers_);
	}
}