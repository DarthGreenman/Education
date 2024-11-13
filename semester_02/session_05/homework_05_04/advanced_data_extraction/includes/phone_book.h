// phone_book.h

#ifndef PHONE_BOOK_H
#define PHONE_BOOK_H

#include <pqxx/pqxx>
#include <pqxx/result.hxx>
#include <string>
#include <utility>
#include <vector>

namespace phone
{
	struct contact
	{
		std::pair<std::string, std::string> name{};
		std::string email{};
		std::vector<std::string> numbers{};
	};

	class phone_book
	{
	public:
		using string = std::string;
		using pair = std::pair<std::string, std::string>;
		
		phone_book() = delete;
		phone_book(pqxx::connection&& connection);
		phone_book(pqxx::connection&& connection, const std::vector<contact>& members);

		phone_book(const phone_book&) = delete;
		phone_book(phone_book&&) = delete;

		phone_book& operator=(const phone_book&) = delete;
		phone_book& operator=(phone_book&&) = delete;

		~phone_book() = default;

		pqxx::result add_contact(const contact& member);
		pqxx::result add_number(const contact& member);
		pqxx::result add_number(const pair& name, const string& number);

	private:
		void create_structure(const char* query);

	private:
		pqxx::connection connection_{};
	};
}

#endif // !PHONE_BOOK_H