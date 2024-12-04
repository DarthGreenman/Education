// phone_book.h

#ifndef PHONE_BOOK_H
#define PHONE_BOOK_H

#include "contact.h"

#include <concepts>
#include <pqxx/pqxx>
#include <pqxx/result.hxx>
#include <string>
#include <type_traits>
#include <vector>

namespace phone
{
	template<typename T>
	concept Is_field_data_types = std::is_same_v<T, std::size_t> || std::is_same_v<T, std::string>;

	class phone_book
	{
	public:
		using name_type = contact::name_type;
		using email_address_type = contact::email_address_type;
		using phone_number_type = contact::phone_number_type;
		
		phone_book() = delete;
		phone_book(const std::string& connection_string);
		phone_book(const std::string& connection_string, const std::vector<contact>& persons);

		phone_book(const phone_book&) = delete;
		phone_book(phone_book&&) = default;

		phone_book& operator=(const phone_book&) = delete;
		phone_book& operator=(phone_book&&) = delete;

		~phone_book() = default;
		
	public:
		bool add_contact(const contact& person);
		bool add_phone(const name_type& name, const phone_number_type& phone_number);
		bool add_phone(std::size_t person_id, const phone_number_type& phone_number);

		bool del_contact(std::size_t person_id);
		bool del_phone(std::size_t phone_number_id);

		template<Is_field_data_types ... Args>
		pqxx::internal::result_iteration<Args ...> get(const std::string& query)
		{
			pqxx::work wk{ connection_ };
			return wk.query<Args ...>(query);
		}

	private:
		void create_structure(const std::string& query);
		void create_structure();
		bool record_exists(const contact& person);
		bool execute(const std::string& query);

	private:
		pqxx::connection connection_{};
	};
}

#endif // !PHONE_BOOK_H