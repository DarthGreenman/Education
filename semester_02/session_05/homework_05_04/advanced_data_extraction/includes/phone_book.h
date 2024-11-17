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
		phone_book(pqxx::connection&& connection);
		phone_book(pqxx::connection&& connection, const std::vector<contact>& members);

		phone_book(const phone_book&) = delete;
		phone_book(phone_book&&) = delete;

		phone_book& operator=(const phone_book&) = delete;
		phone_book& operator=(phone_book&&) = delete;

		~phone_book() = default;
	
	public:
		pqxx::result add_contact(const contact& member);
		pqxx::result add_number(const contact& member);
		pqxx::result add_number(const name_type& name, const phone_number_type& number);
		pqxx::internal::result_iteration<std::size_t, std::string, std::string, std::string> get();

	private:
		void create_structure(const std::string& query);
		bool record_exists(const contact& member);

	private:
		pqxx::connection connection_{};
	};

	void print(const pqxx::internal::result_iteration<std::size_t, std::string, std::string, std::string>& records);
}

#endif // !PHONE_BOOK_H