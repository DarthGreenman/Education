// contact.h

#ifndef CONTACT_H
#define CONTACT_H

#include <string>
#include <utility>
#include <vector>

namespace phone
{
	class contact
	{
	public:
		using pair_type = std::pair<std::string, std::string>;
		using string_type = std::string;
		using vector_type = std::vector<std::string>;

		contact() = delete;
		contact(const pair_type& name, const string_type& email, const vector_type& numbers);
		contact(const contact&) = default;
		contact(contact&&) = delete;
		~contact() = default;
		
		contact& operator=(const contact&) = delete;
		contact& operator=(contact&&) = delete;

		std::tuple<pair_type, string_type, vector_type> get() const;

	private:
		pair_type name_{};
		string_type email_{};
		vector_type numbers_{};
	};
}

#endif // !CONTACT_H