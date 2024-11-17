// contact.cpp

#include "../includes/contact.h"

#include <tuple>

namespace phone
{
	using name_type = contact::name_type;
	using email_address_type = contact::email_address_type;
	using number_list_type = contact::number_list_type;

	contact::contact(const name_type& name, const email_address_type& email, const number_list_type& numbers) :
		name_{ name }, email_{ email }, numbers_{ numbers }	{}

	std::tuple<name_type, email_address_type, number_list_type> phone::contact::get() const
	{
		return  std::make_tuple(name_, email_, numbers_);
	}
}