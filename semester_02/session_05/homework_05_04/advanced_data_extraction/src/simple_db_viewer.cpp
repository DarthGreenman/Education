// simple_db_viewer.cpp

#include "../includes/contact.h"
#include "../includes/my_input.h"
#include "../includes/phone_book.h"
#include "../includes/simple_db_viewer.h"

#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <iostream>
#include <pqxx/pqxx>
#include <pqxx/result.hxx>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>


namespace phone
{
	static void show_menu()
	{
		using namespace std;
		cout << "\nКОМАНДА                     " << "ID";
		cout << "\nРедактировать контакт       " << '1';
		cout << "\nДобавить контакт            " << '2';
		cout << "\nУдалить контакт             " << '3';
		cout << "\nВыход                       " << '0';
	}

	static void show_submenu()
	{
		using namespace std;
		cout << "\nКОМАНДА                     " << "ID";
		cout << "\nДобавить номер телефона     " << '4';
		cout << "\nУдалить номер телефона      " << '5';
		cout << "\nВыход                       " << '0';
	}

	using user_message = typename simple_db_viewer::user_message;

	simple_db_viewer::simple_db_viewer(phone_book&& contacts) :
		contacts_{ std::move(contacts) } {}
	
	void simple_db_viewer::exec()
	{
		/*
		* true false UM_ADD_CONTACT
		* true false UM_ADD_PHONE
		* true       UM_QUIT
		*/
		auto message = std::make_pair(true, UM_REPEAT);
		while (message.first && message.second != UM_QUIT)
			message = work();
		
		if (!message.first)
		{
			const auto um = message.second;
			const std::string error_location{
				um == UM_ADD_CONTACT ? "UM_ADD_CONTACT" :
				um == UM_DEL_CONTACT ? "UM_DEL_CONTACT" :
				um == UM_ADD_PHONE ? "UM_ADD_PHONE" :
				um == UM_DEL_PHONE ? "UM_DEL_PHONE" :
				"UNKNOWN_CAUSE"
			};

			throw std::invalid_argument{ "Error while executing the command: " + error_location };
		}
	}

	std::pair<bool, user_message> simple_db_viewer::work()
	{
		system("cls");
		view();

		using namespace std;
		using namespace my;

		switch (get_message(show_menu))
		{
		case UM_EDIT:
		{
			auto message = std::make_pair(true, UM_REPEAT);
			for (const auto person_id = get_input_value<size_t>("Введите ID контакта: "); 
				message.first && message.second != UM_QUIT;)
			{
				if (message = work(person_id); !message.first)
					return make_pair(false, message.second);
			}
			return make_pair(true, UM_EDIT);
		}
		case UM_ADD_CONTACT:
		{
			const contact::name_type name{
				get_input_value<string>("Имя: "),
				get_input_value<string>("Фамилия: ")
			};
			if (contacts_.add_contact(contact{ name, get_email_address("Электронная почты: ") }))
				return make_pair(get_phone_number(name, "Номер телефона: "), UM_ADD_PHONE);
			return make_pair(false, UM_ADD_CONTACT);
		}
		case UM_DEL_CONTACT:
			return make_pair(contacts_.del_contact(get_input_value<size_t>("Введите ID контакта: ")),
				UM_DEL_CONTACT);
		case UM_QUIT:
			return make_pair(true, UM_QUIT);
		}
	}

	std::pair<bool, user_message> simple_db_viewer::work(std::size_t person_id)
	{
		system("cls");
		view(person_id);

		using namespace std;
		using namespace my;

		switch (get_message(show_submenu))
		{
		case UM_ADD_PHONE:
			return !get_phone_number(person_id, "Номер телефона: ") ?
				make_pair(false, UM_ADD_PHONE) : make_pair(true, UM_ADD_PHONE);
		case UM_DEL_PHONE:
			return !contacts_.del_phone(get_input_value<size_t>("Введите ID телефона: ")) ?
				make_pair(false, UM_DEL_PHONE) : make_pair(true, UM_DEL_PHONE);
		case UM_QUIT:
			return make_pair(true, UM_QUIT);
		}
	}

	void simple_db_viewer::view()
	{
		// Напечатать данные всех персон
		using namespace std;
		cout << '|'
			<< setw(3) << right << "ID" << " | "
			<< setw(20) << left << "NAME" << " | "
			<< setw(30) << left << "MAIL" << " |\n";

		const auto persons = contacts_.get<size_t, string, string>(
			"SELECT id, "
			"CONCAT(surname, ' ', forename) AS name, "
			"CONCAT(mailbox, '@', hostname) AS email "
			"FROM subscriber "
			"ORDER BY name;"
		);
		
		auto view = [](const typename decltype(begin(persons))::value_type& person)
			{
				const auto& [id, name, email] = person;
				cout << "|"
					<< setw(3) << right << id << " | "
					<< setw(20) << left << name << " | "
					<< setw(30) << left << (email == "@" ? " " : email) << " |\n";
			};
		for_each(begin(persons), cend(persons), view);
	}

	void simple_db_viewer::view(std::size_t person_id)
	{
		// Напечатать данные персоны
		using namespace std;
		const auto persons = contacts_.get<size_t, string, string>(
			"SELECT id, "
			"CONCAT(surname, ' ', forename) AS name, "
			"CONCAT(mailbox, '@', hostname) AS email "
			"FROM subscriber "
			"WHERE id = '" + to_string(person_id) + "' "
			"ORDER BY name;"
		);

		auto view = [](const typename decltype(begin(persons))::value_type& person)
			{
				const auto& [id, name, email] = person;
				cout << "ID:       " << id << '\n';
				cout << "Name:     " << name << '\n';
				cout << "Mail:     " << (email == "@" ? " " : email) << '\n';
				cout << "Phones\n";
			};
		for_each(begin(persons), cend(persons), view);

		const auto phone_numbers = contacts_.get<size_t, size_t, string>(
			"SELECT id, subscriber_id, number "
			"FROM phone_numbers "
			"WHERE subscriber_id = '" + to_string(person_id) + "' "
			"ORDER BY number;"
		);

		for_each(begin(phone_numbers), cend(phone_numbers),
			[](const typename decltype(begin(phone_numbers))::value_type& phone_number)
			{
				const auto& [id, subscriber_id, number] = phone_number;
				cout << setw(10) << right << "ID: " << setw(2) << id << setw(15) << number << '\n';
			}
		);
		cout << '\n';
	}

	user_message simple_db_viewer::get_message(void(*show_menu)())
	{
		show_menu();
		return static_cast<user_message>(
			my::get_input_value<size_t>("\n\nВведите ID команды: ")
			);
	}

	std::string simple_db_viewer::get_email_address(const std::string& invitation)
	{
		const auto value = my::get_input_value("Электронная почты: ");
		return value;
	}
}
