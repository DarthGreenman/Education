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
		using pair_type = std::pair<std::string, std::string>;
		using string_type = std::string;
		using size_type = std::size_t;
		
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
		pqxx::result add_number(const pair_type& name, const string_type& number);
		pqxx::internal::result_iteration<size_type, string_type, string_type, string_type> get();

	private:
		void create_structure(const string_type& query);
		bool record_exists(const contact& member);

	private:
		pqxx::connection connection_{};
	};

	void print(const pqxx::internal::result_iteration<std::size_t, std::string, std::string, std::string>& records);
}

#endif // !PHONE_BOOK_H