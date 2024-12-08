﻿//phone_book.h

#include "../includes/contact.h"
#include "../includes/phone_book.h"

#include <pqxx/connection.hxx>
#include <pqxx/internal/result_iter.hxx>
#include <pqxx/result.hxx>
#include <pqxx/transaction.hxx>

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

namespace phone
{
	using name_type = phone_book::name_type;
	using email_address_type = phone_book::email_address_type;
	using phone_number_type = phone_book::phone_number_type;

	phone_book::phone_book(const std::string& connection_string) :
		connection_{ pqxx::connection{connection_string} }
	{
		// Создать таблицы, если их не существует
		create_structure();
	}

	phone_book::phone_book(const std::string& connection_string, const std::vector<contact>& persons) :
		phone_book(connection_string)
	{
		// Заполнить новыми данными
		using namespace std;
		auto add_person = [&](const contact& person)
			{
				if (add_contact(person))
				{
					const auto& [name, email, phone_numbers] = person.get();
					using Value_type = typename iterator_traits<decltype(begin(phone_numbers))>::value_type;
					auto add_phone_number = [&](const Value_type& phone_number)
						{
							add_phone(name, phone_number);
						};
					for_each(cbegin(phone_numbers), cend(phone_numbers), add_phone_number);
				}
			};
		for_each(cbegin(persons), cend(persons), add_person);
	}

	bool phone_book::add_contact(const contact& person)
	{
		if (record_exists(person))
		{
			// Если запись существует - информировать пользователя
			return false;
		}

		const auto& [name, email, phone_numbers] = person.get();
		const auto& [forename, surname] = name;
		const auto& [mailbox, hostmail] = email.get();
		const std::string query {
			"INSERT INTO subscriber(forename, surname, mailbox, hostname) "
			"VALUES('" + forename + "', '" + surname + "', '" + mailbox + "', '" + hostmail + "');"
		};
		
		return exec(query);
	}

	bool phone_book::add_phone(const name_type& name, const phone_number_type& phone_number)
	{
		const auto& [forename, surname] = name;
		// В данном коде формат записи номера +19792195004, так воспользовались методом класса,
		// но пользователь может записать номер в базу м в другом формате, предварительно
		// получив доступ через метод класса get() к кодам номера: страны, зоны, узла и перс. номеру. 
		const std::string query {
			"INSERT INTO phone_numbers(subscriber_id, number) "
			"VALUES("
				"(SELECT id FROM subscriber WHERE forename = '" + forename + "' AND "
				"surname = '" + surname + "'), '" + phone_number.normalization() + "');"
		};

		return exec(query);
	}

	bool phone_book::add_phone(std::size_t person_id, const phone_number_type& phone_number)
	{
		const std::string query {
			"INSERT INTO phone_numbers(subscriber_id, number) "
			"VALUES("
				"(SELECT id FROM subscriber WHERE id = '" + std::to_string(person_id) + "'), '"
			+ phone_number.normalization() + "');"
		};

		return exec(query);
	}

	bool phone_book::del_contact(std::size_t person_id)
	{
		const std::string query {
			"DELETE FROM subscriber WHERE id = '" + std::to_string(person_id) + "';"
		};

		return exec(query);
	}

	bool phone_book::del_phone(std::size_t phone_number_id)
	{
		const std::string query{
			"DELETE FROM phone_numbers WHERE id = '" + std::to_string(phone_number_id) + "';"
		};

		return exec(query);
	}
		
	void phone_book::create_structure(const std::string& query)
	{
		pqxx::work wk{ connection_ };
		wk.exec(query);
		wk.commit();
	}

	void phone_book::create_structure()
	{
		create_structure
		(
			"CREATE TABLE IF NOT EXISTS subscriber "
			"("
			"id serial, "
			"forename varchar(32) NOT NULL, "
			"surname varchar(32) NOT NULL, "
			"mailbox varchar(64), "
			"hostname varchar(253), "
			"UNIQUE (forename, surname, mailbox), "
			"UNIQUE (forename, surname, hostname), "
			"PRIMARY KEY (id)"
			");"
		);

		create_structure
		(
			"CREATE TABLE IF NOT EXISTS phone_numbers "
			"("
			"id serial, "
			"subscriber_id integer, "
			"number varchar(12), "
			"UNIQUE (number), "
			"PRIMARY KEY (id), "
			"FOREIGN KEY (subscriber_id) REFERENCES subscriber (id) ON DELETE CASCADE"
			");"
		);
	}

	bool phone_book::record_exists(const contact& person)
	{
		const auto& [name, email, phone_numbers] = person.get();
		const auto& [forename, surname] = name;
		const std::string query
		{
			"SELECT id FROM subscriber WHERE forename='" + forename + "' AND "
				"surname='" + surname + "';" 
		};

		pqxx::work wk{ connection_ };
		const auto query_result = wk.query<std::size_t>(query);

		return query_result.begin() == query_result.end() ? false : true;
	}

	bool phone_book::exec(const std::string& query)
	{
		pqxx::work wk{ connection_ };
		const pqxx::result query_result = wk.exec(query);
		wk.commit();

		return query_result.affected_rows() == 0 ? false : true;
	}
} 