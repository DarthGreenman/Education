//phone_book.h

#include "../includes/phone_book.h"

#include <pqxx/result.hxx>
#include <pqxx/transaction.hxx>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

phone::phone_book::phone_book(pqxx::connection&& connection) :
	connection_{ std::move(connection) } 
{
	create_structure(
		"CREATE TABLE IF NOT EXISTS subscriber "
		"("
			"id serial, "
			"first_name varchar(128) NOT NULL, "
			"last_name varchar(128) NOT NULL, "
			"email varchar(128), "
			"UNIQUE (email), "
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

phone::phone_book::phone_book(pqxx::connection&& connection, const std::vector<contact>& members) :
	phone_book(std::move(connection))
{
	pqxx::result query_result{};
	for (const auto& member : members)
		query_result = add_contact(member);
}

pqxx::result phone::phone_book::add_contact(const contact& member)
{
	const auto& [name, email, numbers] = member;
	// Проверка корректности имени, фамилии, адреса почты
	pqxx::work wk{ connection_ };
	const std::string query{
		"INSERT INTO subscriber(first_name, last_name, email) "
		"VALUES('" + name.first + "', '" + name.second + "', '" + email + "');"
	};
	const pqxx::result query_result{ wk.exec(query) };
	wk.commit();
	
	return query_result;
}

pqxx::result phone::phone_book::add_number(const contact& member)
{
	const auto& [name, email, numbers] = member;
	pqxx::result query_result{};
	for (const auto& number : numbers)
		query_result = add_number(name, number);
	
	return query_result;
}

pqxx::result phone::phone_book::add_number(const std::pair<std::string, std::string>& name, const std::string& number)
{
	// Проверка корректности номера
	pqxx::work wk{ connection_ };
	const std::string query{
		"INSERT INTO phone_numbers(subscriber_id, number) "
		"VALUES("
			"(SELECT id FROM subscriber WHERE first_name='" + name.first + "' AND "
			"last_name='" + name.second + "'), '" + number + "');"
	};
	const pqxx::result query_result{ wk.exec(query) };
	wk.commit();

	return query_result;
}

void phone::phone_book::create_structure(const char* query)
{
	pqxx::work wk{ connection_ };
	wk.exec(query);
	wk.commit();
}
