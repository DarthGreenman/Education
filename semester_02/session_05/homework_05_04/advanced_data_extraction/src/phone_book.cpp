//phone_book.h

#include "../includes/contact.h"
#include "../includes/phone_book.h"

#include <pqxx/result.hxx>
#include <pqxx/transaction.hxx>

#include <algorithm>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <limits>
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
		using namespace std;
		auto add_person = [&](const contact& person)
			{
				if (add(person))
				{
					const auto& [name, email, phone_numbers] = person.get();
					using Value_type = typename iterator_traits<decltype(begin(phone_numbers))>::value_type;
					const decltype(phone_numbers) a{};
					auto add_phone_number = [&](const Value_type& phone_number)
						{
							add(name, phone_number);
						};
					for_each(cbegin(phone_numbers), cend(phone_numbers), add_phone_number);
				}
			};
		for_each(cbegin(persons), cend(persons), add_person);
	}

	bool phone_book::add(const contact& person)
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

	bool phone_book::add(const name_type& name, const phone_number_type& phone_number)
	{
		const auto& [forename, surname] = name;
		// В данном коде формат записи номера +19792195004, так воспользовались методом класса,
		// но пользователь может записать номер в базу м в другом формате, предварительно
		// получив доступ через метод класса get() к кодам номера: страны, зоны, узла и перс. номеру. 
		const std::string query
		{
			"INSERT INTO phone_numbers(subscriber_id, number) "
			"VALUES("
				"(SELECT id FROM subscriber WHERE forename = '" + forename + "' AND "
				"surname = '" + surname + "'), '" + phone_number.normalization() + "');"
		};

		pqxx::work wk{ connection_ };
		const pqxx::result query_result{ wk.exec(query) };
		wk.commit();

		return query_result.affected_rows() == 0 ? false : true;
	}

	bool phone_book::add(std::size_t person_id, const phone_number_type& phone_number)
	{
		const std::string query
		{
			"INSERT INTO phone_numbers(subscriber_id, number) "
			"VALUES("
				"(SELECT id FROM subscriber WHERE id = '" + std::to_string(person_id) + "'), '"
			+ phone_number.normalization() + "');"
		};

		pqxx::work wk{ connection_ };
		const pqxx::result query_result{ wk.exec(query) };
		wk.commit();

		return query_result.affected_rows() == 0 ? false : true;
	}
	
	pqxx::internal::result_iteration<std::size_t, std::string, std::string> phone_book::get_contact(std::size_t id)
	{
		const std::string query
		{
			id == (std::numeric_limits<std::size_t>::max)()
			?
			"SELECT id, "
			"CONCAT(surname, ' ', forename) AS name, "
			"CONCAT(mailbox, '@', hostname) AS email "
			"FROM subscriber "
			"ORDER BY name;" 
			:
			"SELECT id, "
			"CONCAT(surname, ' ', forename) AS name, "
			"CONCAT(mailbox, '@', hostname) AS email "
			"FROM subscriber "
			"WHERE id='" + std::to_string(id) + "' "
			"ORDER BY name;"
		};

		pqxx::work wk{ connection_ };
		return wk.query<std::size_t, std::string, std::string>(query);
	}

	pqxx::internal::result_iteration<std::size_t, std::string> phone_book::get_number(std::size_t id)
	{
		const std::string query
		{
			"SELECT subscriber_id, number "
			"FROM phone_numbers "
			"WHERE subscriber_id='" + std::to_string(id) + "' "
			"ORDER BY number;"
		};

		pqxx::work wk{ connection_ };
		return wk.query<std::size_t, std::string>(query);
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

	void print(phone_book& contacts)
	{
		const auto persons = contacts.get_contact();
		using namespace std;
		using Value_type = typename decltype(begin(persons))::value_type;
		auto view = [](const Value_type& person)
			{
				const auto& [id, name, email] = person;
				cout << "|" 
					<< setw(3) << right << id << " | " 
					<< setw(20) << left << name << " | " 
					<< setw(30) << left << (email == "@" ? " " : email) << " |\n";
			};

		cout << "|" 
			<< setw(3) << right << "ID" << " | " 
			<< setw(20) << left << "NAME" << " | " 
			<< setw(30) << left << "MAIL" << " |\n";
		
		for_each(begin(persons), cend(persons), view);
	}
	void print(phone_book& contacts, std::size_t person_id)
	{
		const auto persons = contacts.get_contact(person_id);
		using namespace std;
		auto view = [](const typename decltype(begin(persons))::value_type& person)
			{
				const auto& [id, name, email] = person;
				cout << setw(15) << left << "ID:" << setw(3) << left << id << '\n';
				cout << setw(15) << left << "NAME:" << setw(20) << left << name << '\n';
				cout << setw(15) << left << "MAIL:" << setw(30) << left << (email == "@" ? " " : email) << '\n';
			};
		for_each(begin(persons), cend(persons), view);
		
		const auto phone_numbers = contacts.get_number(person_id);
		cout << setw(15) << left << "PHONE NUMBERS:";
		
		for_each(begin(phone_numbers), cend(phone_numbers), 
			[](const typename decltype(begin(phone_numbers))::value_type& phone_number)
			{
				const auto& [id, number] = phone_number;
				cout  << number << "; ";
			}
		);
	}
} 