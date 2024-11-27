// phone_book.h

#ifndef PHONE_BOOK_H
#define PHONE_BOOK_H

#include "contact.h"

#include <pqxx/pqxx>
#include <pqxx/result.hxx>
#include <string>
#include <utility>
#include <vector>

namespace phone
{
	class phone_book
	{
	public:
		using name_type = phone::contact::name_type;
		using email_address_type = phone::contact::email_address_type;
		using phone_number_type = phone::contact::phone_number_type;
		
		phone_book() = delete;
		phone_book(const std::string& connection_string);
		phone_book(const std::string& connection_string, const std::vector<contact>& persons);

		phone_book(const phone_book&) = delete;
		phone_book(phone_book&&) = delete;

		phone_book& operator=(const phone_book&) = delete;
		phone_book& operator=(phone_book&&) = delete;

		~phone_book() = default;
	
	public:
		bool add_contact(const contact& person);
		bool add_phone_number(const contact& person);
		bool add_phone_number(const name_type& name, const phone_number_type& number);
		pqxx::internal::result_iteration<std::size_t, std::string, std::string> get();

	private:
		void create_structure(const std::string& query);
		void create_structure();
		bool record_exists(const contact& person);

	private:
		pqxx::connection connection_{};
	};

	void print(const pqxx::internal::result_iteration<std::size_t, std::string, std::string>& records);
}

#endif // !PHONE_BOOK_H