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
#include <string>
#include <type_traits>


namespace phone
{
	simple_db_viewer::simple_db_viewer(phone_book&& contacts) :
		contacts_{ std::move(contacts) } {}
	
	void simple_db_viewer::action()
	{
		using namespace std;
		using namespace my;
		using contact = typename phone::contact;

		system("cls");
		view();
		cout << "\n\nВЫБЕРЕТЕ ДЕЙСТВИЕ:";
		cout << setw(30) << "\nКОМАНДА" << "ID";
		cout << setw(30) << "\nДанные контакта" << '1';
		cout << setw(30) << "\nДобавить контакт" << '2';
		cout << setw(30) << "\nДобавить номер телефона" << '3';
		cout << setw(30) << "\nУдалить контакт" << '4';
		cout << setw(30) << "\nУдалить номер телефона" << '5';
		cout << setw(30) << "\nВыход" << '0';

		switch (my::get_input_value<size_t>("\n\nВведите ID команды: "))
		{
		case 1:
		{
			const auto person_id = get_input_value<size_t>("Введите ID контакта: ");
			system("cls");
			view(person_id);
			system("pause");
			
			action();
			break;
		}
		case 2: // Добавить контакт
		{
			const contact::name_type name{
				get_input_value<string>("Имя: "),
				get_input_value<string>("Фамилия: ")
			};
			contacts_.add_contact(contact{ name, get_input_value<string>("Email: ") });
			contacts_.add_phone(name, get_input_value<string>("Номер телефона: "));

			action();
			break;
		}
		case 3: // Добавить номер
		{
			const auto person_id = get_input_value<size_t>("Введите ID контакта: ");
			system("cls");
			view(person_id);
			contacts_.add_phone(person_id, get_input_value<string>("Номер телефона: "));
			
			action();
			break;
		}
		case 4: // Удалить контакт
		{
			contacts_.del_contact(get_input_value<size_t>("Введите ID контакта: "));
			
			action();
			break;
		}
		case 5: // Удалить номер
		{
			const auto person_id = get_input_value<size_t>("Введите ID контакта: ");
			system("cls");
			view(person_id);
			contacts_.del_phone(get_input_value<size_t>("Введите ID номера телефона: "));

			action();
			break;
		}
		case 0:
			return;
		}
	}

	void simple_db_viewer::view()
	{
		// Напечатать данные всех персон
		using namespace std;
		cout << '|'
			<< setw(3) << right << "ID" << " | "
			<< setw(20) << left << "ИМЯ" << " | "
			<< setw(30) << left << "EMAIL" << " |\n";

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
				cout << setw(20) << left << "ID:" << setw(3) << left << id << '\n';
				cout << setw(20) << left << "ИМЯ:" << setw(20) << left << name << '\n';
				cout << setw(20) << left << "EMAIL:" << setw(30) << left << (email == "@" ? " " : email) << '\n';
				cout << setw(20) << left << "НОМЕРА ТЕЛЕФОНОВ:   \n";
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
				cout << setw(17) << right << id << setw(15) << right << number << '\n';
			}
		);
		cout << '\n';
	}
}
