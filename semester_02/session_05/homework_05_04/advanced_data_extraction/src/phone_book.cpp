//phone_book.h

#include "../includes/contact.h"
#include "../includes/phone_book.h"

#include <pqxx/result.hxx>
#include <pqxx/transaction.hxx>

#include <iomanip>
#include <ios>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace phone
{
	using name_type = phone_book::name_type;
	using email_address_type = phone_book::email_address_type;
	using phone_number_type = phone_book::phone_number_type;

	phone_book::phone_book(pqxx::connection&& connection) :
		connection_{ std::move(connection) }
	{
		create_structure(
			"CREATE TABLE IF NOT EXISTS subscriber "
			"("
			"id serial, "
			"first_name varchar(128) NOT NULL, "
			"last_name varchar(128) NOT NULL, "
			"email varchar(128), "
			"UNIQUE (first_name, last_name, email), "
			"PRIMARY KEY (id)"
			");"
		);

		create_structure(
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

	phone_book::phone_book(pqxx::connection&& connection, const std::vector<contact>& members) :
		phone_book(std::move(connection))
	{
		for (const auto& member : members)
		{
			if (!record_exists(member))
			{
				add_contact(member);
				add_number(member);
			}
		}
	}

	pqxx::result phone_book::add_contact(const contact& member)
	{
		const auto& [name, email, numbers] = member.get();
		const auto& [forename, surname] = name.get();
		
		pqxx::work wk{ connection_ };
		const std::string query{
			"INSERT INTO subscriber(first_name, last_name, email) "
			"VALUES('" + forename + "', '" + surname + "', '" + email.get() + "');"
		};
		const pqxx::result query_result{ wk.exec(query) };
		wk.commit();

		return query_result;
	}

	pqxx::result phone_book::add_number(const contact& member)
	{
		const auto& [name, email, numbers] = member.get();
		pqxx::result query_result{};
		for (const auto& number : numbers)
			query_result = add_number(name, number);

		return query_result;
	}

	pqxx::result phone_book::add_number(const name_type& name, const phone_number_type& number)
	{
		const auto& [forename, surname] = name.get();

		pqxx::work wk{ connection_ };
		const std::string query{
			"INSERT INTO phone_numbers(subscriber_id, number) "
			"VALUES("
				"(SELECT id FROM subscriber WHERE first_name='" + forename + "' AND "
				"last_name='" + surname + "'), '" + number.get() + "');"
		};
		const pqxx::result query_result{ wk.exec(query) };
		wk.commit();

		return query_result;
	}

	pqxx::internal::result_iteration<std::size_t, std::string, std::string, std::string> phone_book::get()
	{
		pqxx::work wk{ connection_ };
		return wk.query<std::size_t, std::string, std::string, std::string>("SELECT * FROM subscriber");
	}

	void phone_book::create_structure(const std::string& query)
	{
		pqxx::work wk{ connection_ };
		wk.exec(query);
		wk.commit();
	}

	bool phone_book::record_exists(const contact& member)
	{
		const auto& [full_name, email, numbers] = member.get();
		const auto& [name, surname] = full_name.get();

		const std::string query{ "SELECT id FROM subscriber WHERE first_name='" + name + "' AND "
				"last_name='" + surname + "';" };
		pqxx::work wk{ connection_ };
		const auto query_result = wk.query<std::size_t>(query);
		return query_result.begin() == query_result.end() ? false : true;
	}

	void print(const pqxx::internal::result_iteration<std::size_t, std::string, std::string, std::string>& records)
	{
		using namespace std;
		for (const auto& [id, name, surname, email] : records)
		{
			cout << setw(5) << right << id;
			cout << setw(3) << left << " |";
			cout << setw(12) << left << name;
			cout << setw(2) << left << '|';
			cout << setw(20) << left << surname;
			cout << setw(2) << left << '|';
			cout << setw(30) << left << email;
			cout << setw(2) << left << "|\n";
		}
	} 
}