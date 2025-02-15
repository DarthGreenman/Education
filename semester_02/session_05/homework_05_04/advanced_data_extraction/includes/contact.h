// contact.h

#ifndef CONTACT_H
#define CONTACT_H

#include "types.h"

#include <tuple>
#include <utility>
#include <vector>

namespace phone
{
	class contact
	{
	public:
		using name_type = typename my::person_name;
		using email_address_type = typename my::email_address;
		using email_list_type = std::vector<my::email_address>;
		using phone_number_type = typename my::phone_number;
		using number_list_type = std::vector<my::phone_number>;
		
		contact() = delete;
		contact(const name_type& name, const email_list_type& emails = {},
			const number_list_type& phone_numbers = {});
		contact(const contact&) = default;
		contact(contact&&) = delete;
		~contact() = default;
		
		contact& operator=(const contact&) = delete;
		contact& operator=(contact&&) = delete;

		std::tuple<name_type, email_list_type, number_list_type> get() const;

	private:
		name_type name_{};
		email_list_type emails_{};
		number_list_type phone_numbers_{};
	};
}

#endif // !CONTACT_H