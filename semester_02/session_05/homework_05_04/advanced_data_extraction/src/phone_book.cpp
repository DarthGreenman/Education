//phone_book.h

#include "../includes/contact.h"
#include "../includes/phone_book.h"

#include <pqxx/result.hxx>
#include <pqxx/transaction.hxx>

#include <iomanip>
#include <ios>
#include <iostream>
#include <pqxx/connection.hxx>
#include <pqxx/internal/result_iter.hxx>
#include <string>
#include <type_traits>
#include <vector>

namespace phone
{
	using name_type = phone_book::name_type;
	using email_address_type = phone_book::email_address_type;
	using phone_number_type = phone_book::phone_number_type;

	phone_book::phone_book(const std::string& connection_string) :
		connection_{ pqxx::connection{connection_string} }	{}

	phone_book::phone_book(const std::string& connection_string, const std::vector<contact>& persons) :
		phone_book(connection_string)
	{
		// Создать таблицы, если их не существует
		create_structure();

		// Заполнить новыми данными
		for (const auto& person : persons)
			if (add_contact(person))
				if (const auto& [name, email, phone_numbers] = person.get(); 
					!phone_numbers.empty())
					add_phone_number(person);
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
		const std::string query
		{
			"INSERT INTO subscriber(forename, surname, mailbox, hostname) "
			"VALUES('" + forename + "', '" + surname + "', '" + mailbox + "', '" + hostmail + "');"
		};
		
		pqxx::work wk{ connection_ };
		const pqxx::result query_result = wk.exec(query);
		wk.commit();

		return query_result.affected_rows() == 0 ? false : true;
	}

	bool phone_book::add_phone_number(const contact& person)
	{
		const auto& [name, email, phone_numbers] = person.get();		
		for (const auto& phone_number : phone_numbers)
			if (!add_phone_number(name, phone_number))
				return false;

		return true;
	}

	bool phone_book::add_phone_number(const name_type& name, const phone_number_type& number)
	{
		const auto& [forename, surname] = name;
		// В данном коде формат записи номера +19792195004, так воспользовались методом класса,
		// но пользователь может записать номер в базу м в другом формате, предварительно
		// получив доступ через метод класса get() к кодам номера: страны, зоны, узла и перс. номеру. 
		const std::string query
		{
			"INSERT INTO phone_numbers(subscriber_id, number) "
			"VALUES("
				"(SELECT id FROM subscriber WHERE forename='" + forename + "' AND "
				"surname='" + surname + "'), '" + number.normalization() + "');"
		};

		pqxx::work wk{ connection_ };
		const pqxx::result query_result{ wk.exec(query) };
		wk.commit();

		return query_result.affected_rows() == 0 ? false : true;
	}

	pqxx::internal::result_iteration<std::size_t, std::string, std::string> phone_book::get()
	{
		const std::string query
		{
			"SELECT id, "
			"CONCAT(surname, ' ', forename) AS name, "
			"CONCAT(mailbox, '@', hostname) AS email "
			"FROM subscriber "
			"ORDER BY name;"
		};

		pqxx::work wk{ connection_ };
		return wk.query<std::size_t, std::string, std::string>(query);
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

	void print(const pqxx::internal::result_iteration<std::size_t, std::string, std::string>& records)
	{
		using namespace std;
		for (const auto& [id, name, email] : records)
		{
			cout << setw(5) << right << id;
			cout << setw(3) << left << " |";
			cout << setw(24) << left << name;
			cout << setw(2) << left << " |";
			cout << setw(35) << left << (email == "@" ? " " : email);
			cout << setw(2) << left << "|\n";
		}
	} 
}