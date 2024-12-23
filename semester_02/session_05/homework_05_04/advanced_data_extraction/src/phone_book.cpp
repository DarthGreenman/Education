//phone_book.h

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
	using size_t = typename std::size_t;
	using string = typename std::string;
	using name_type = typename phone_book::name_type;
	using email_address_type = typename phone_book::email_address_type;
	using phone_number_type = typename phone_book::phone_number_type;

	phone_book::phone_book(const std::string& connection_string) :
		connection_{ pqxx::connection{connection_string} }
	{
		create_structure(); // Создать таблицы, если их не существует //////////////////////////////////////////////////////
		connection_.prepare("add_contact", "INSERT INTO subscriber(forename, surname) VALUES($1, $2)");
		// Подготовленные запросы на добавление нового номера //////////////////////////////////////////////////////////////
		connection_.prepare("add_phone_by_name", "INSERT INTO phone_numbers(subscriber_id, number) "
			"VALUES((SELECT id FROM subscriber WHERE forename = $1 AND surname = $2), $3)");
		connection_.prepare("add_phone_by_id", "INSERT INTO phone_numbers(subscriber_id, number) "
			"VALUES((SELECT id FROM subscriber WHERE id = $1), $2)");
		// Подготовленные запросы на добавление нового email ///////////////////////////////////////////////////////////////
		connection_.prepare("add_email_by_name", "INSERT INTO email_address(subscriber_id, mailbox, hostname) "
			"VALUES((SELECT id FROM subscriber WHERE forename = $1 AND surname = $2), $3, $4)");
		connection_.prepare("add_email_by_id", "INSERT INTO email_address(subscriber_id, mailbox, hostname) "
			"VALUES((SELECT id FROM subscriber WHERE id = $1), $2, $3)");
		// Подготовленные запросы на удаление //////////////////////////////////////////////////////////////////////////////
		connection_.prepare("del_contact", "DELETE FROM subscriber WHERE id = $1");
		connection_.prepare("del_phone", "DELETE FROM phone_numbers WHERE id = $1");
		connection_.prepare("del_email", "DELETE FROM email_address WHERE id = $1");
		// Подготовленные запросы на изменение /////////////////////////////////////////////////////////////////////////////
		connection_.prepare("mod_forename", "UPDATE subscriber SET forename = $1 WHERE id = $2");
		connection_.prepare("mod_surname", "UPDATE subscriber SET surname = $1 WHERE id = $2");
		connection_.prepare("mod_phone", "UPDATE phone_numbers SET number = $1 WHERE id = $2");
		connection_.prepare("mod_email", "UPDATE email_address SET mailbox = $1, hostname = $2 WHERE id = $3");
	}

	void phone_book::loading_data(const std::vector<contact>& persons)
	{
		using namespace std;
		auto add_person = [&](const contact& person)
			{
				const auto& [name, email_addresses, phone_numbers] = person.get();
				if (add_contact(name))
				{
					using Email_address_type = typename iterator_traits<decltype(cbegin(email_addresses))>::value_type;
					auto add_email_address = [&](const Email_address_type& email)
						{
							add_email(name, email);
						};
					for_each(cbegin(email_addresses), cend(email_addresses), add_email_address);

					using Phone_number_type = typename iterator_traits<decltype(cbegin(phone_numbers))>::value_type;
					auto add_phone_number = [&](const Phone_number_type& phone_number)
						{
							add_phone(name, phone_number);
						};
					for_each(cbegin(phone_numbers), cend(phone_numbers), add_phone_number);
				}
			};
		for_each(cbegin(persons), cend(persons), add_person); // Заполнить новыми данными
	}

	bool phone_book::add_contact(const name_type& name)
	{		
		return exec("add_contact", 
			name.forename, name.surname);
	}

	bool phone_book::del_contact(size_t person_id)
	{
		return exec("del_contact",
			std::to_string(person_id));
	}

	bool phone_book::del_phone(size_t phone_number_id)
	{
		return exec("del_phone", 
			std::to_string(phone_number_id));
	}

	bool phone_book::del_email(size_t email_id)
	{
		return exec("del_email", 
			std::to_string(email_id));
	}

	bool phone_book::mod_forename(size_t person_id, 
		const string& forename)
	{
		return exec("mod_forename", 
			forename, person_id);
	}

	bool phone_book::mod_surname(size_t person_id, 
		const string& surname)
	{
		return exec("mod_surname", 
			surname, person_id);
	}

	bool phone_book::mod_phone(size_t phone_number_id, 
		const phone_number_type& phone_number)
	{
		return exec("mod_phone", 
			phone_number.normalization(), phone_number_id);
	}

	bool phone_book::mod_email(size_t email_id, 
		const email_address_type& email_address)
	{
		const auto& [mailbox, hostname] = email_address.get();
		return exec("mod_email",
			mailbox, hostname, email_id);
	}
		
	void phone_book::create_structure(const string& query)
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
			"PRIMARY KEY (id)"
			")"
		);

		create_structure
		(
			"CREATE TABLE IF NOT EXISTS email_address "
			"("
			"id serial, "
			"subscriber_id integer, "
			"mailbox varchar(64), "
			"hostname varchar(253), "
			"UNIQUE(subscriber_id, mailbox, hostname), "
			"PRIMARY KEY (id), "
			"FOREIGN KEY (subscriber_id) REFERENCES subscriber (id) ON DELETE CASCADE"
			")"
		);

		create_structure
		(
			"CREATE TABLE IF NOT EXISTS phone_numbers "
			"("
			"id serial, "
			"subscriber_id integer, "
			"number varchar(12), "
			"UNIQUE (subscriber_id, number), "
			"PRIMARY KEY (id), "
			"FOREIGN KEY (subscriber_id) REFERENCES subscriber (id) ON DELETE CASCADE"
			")"
		);
	}
} 