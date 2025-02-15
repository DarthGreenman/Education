// phone_book.h

#ifndef PHONE_BOOK_H
#define PHONE_BOOK_H

#include "contact.h"

#include <concepts>
#include <pqxx/pqxx>
#include <pqxx/result.hxx>
#include <pqxx/transaction_base.hxx>
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

		phone_book(const phone_book&) = delete;
		phone_book(phone_book&&) = default;

		phone_book& operator=(const phone_book&) = delete;
		phone_book& operator=(phone_book&&) = delete;

		~phone_book() = default;
		
	public:
		void loading_data(const std::vector<contact>& persons);
		bool add_contact(const name_type& name);

		// В данном коде формат записи номера +19792195004, так воспользовались методом класса,
		// но пользователь может записать номер в базу м в другом формате, предварительно
		// получив доступ через метод класса get() к кодам номера: страны, зоны, узла и перс. номеру. 
		template<typename T, 
			typename = std::enable_if_t<std::is_same_v<T, name_type> || std::is_same_v<T, std::size_t>>>
		bool add_phone(const T& value, const phone_number_type& phone_number)
		{
			const auto number = phone_number.normalization();
			if (number.empty())
				return false;

			if constexpr (std::is_same_v<T, name_type>)
				return exec("add_phone_by_name", value.forename, value.surname, number);
			else
				return exec("add_phone_by_id", std::to_string(value), number);
		}

		// Формат mailbox@hostname, где hostname, например: mail.ru, google.com ...
		template<typename T,
			typename = std::enable_if_t<std::is_same_v<T, name_type> || std::is_same_v<T, std::size_t>>>
		bool add_email(const T& value, const email_address_type& email_address)
		{
			const auto& [mailbox, hostname] = email_address.get();
			if (mailbox.empty() || hostname.empty())
				return false;

			if constexpr (std::is_same_v<T, name_type>)
				return exec("add_email_by_name", value.forename, value.surname, mailbox, hostname);
			else
				return exec("add_email_by_id", std::to_string(value), mailbox, hostname);
		}

		bool del_contact(std::size_t person_id);
		bool del_phone(std::size_t phone_number_id);
		bool del_email(std::size_t email_id);

		template<Is_field_data_types ... Args>
		pqxx::internal::result_iteration<Args ...> get(const std::string& query)
		{
			pqxx::work wk{ connection_ };
			return wk.query<Args ...>(query);
		}

	private:
		void create_structure(const std::string& query);
		void create_structure();

		template<Is_field_data_types ... Args>
		bool exec(const std::string& query_name, const Args& ... values)
		{
			pqxx::work wk{ connection_ };
			const pqxx::result query_result = wk.exec_prepared(query_name, values ...);
			wk.commit();

			return query_result.affected_rows() == 0 ? false : true;
		}

	private:
		pqxx::connection connection_{};
	};
}

#endif // !PHONE_BOOK_H