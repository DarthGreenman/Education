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
		cout << "\nДобавить email              " << '6';
		cout << "\nУдалить email               " << '7';
		cout << "\nВыход                       " << '0';
	}

	using user_message = typename simple_db_viewer::user_message;

	simple_db_viewer::simple_db_viewer(phone_book&& contacts) :
		contacts_{ std::move(contacts) } {}
	
	void simple_db_viewer::exec()
	{
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
				um == UM_ADD_EMAIL ? "UM_ADD_EMAIL" :
				um == UM_DEL_EMAIL ? "UM_DEL_EMAIL" :
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
			
			return make_pair(contacts_.add_contact(name), UM_ADD_CONTACT);
		}
		case UM_DEL_CONTACT:
			return make_pair(contacts_.del_contact(get_input_value<size_t>("Введите ID контакта: ")),
				UM_DEL_CONTACT);
		case UM_QUIT:
			return make_pair(true, UM_QUIT);
		default:
			return make_pair(true, UM_REPEAT);
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
			return make_pair(get_phone_number(person_id, "Номер телефона: "), 
				UM_ADD_PHONE);
		case UM_DEL_PHONE:
			return make_pair(contacts_.del_phone(get_input_value<size_t>("Введите ID телефона: ")), 
				UM_DEL_PHONE);
		case UM_ADD_EMAIL:
			return make_pair(get_email_address(person_id, "Email: "), 
				UM_ADD_EMAIL);
		case UM_DEL_EMAIL:
			return make_pair(contacts_.del_email(get_input_value<size_t>("Введите ID email: ")),
				UM_DEL_EMAIL);
		case UM_QUIT:
			return make_pair(true, UM_QUIT);
		default:
			return make_pair(true, UM_REPEAT);
		}
	}

	void simple_db_viewer::view()
	{
		// Напечатать данные всех персон
		using namespace std;
		cout << '|'
			<< setw(3) << right << "ID" << " | "
			<< setw(20) << left << "NAME" << " |\n";

		const auto persons = contacts_.get<size_t, string>(
			"SELECT id, "
			"CONCAT(surname, ' ', forename) AS name "
			"FROM subscriber "
			"ORDER BY name"
		);
		
		auto view = [](const typename decltype(cbegin(persons))::value_type& person)
			{
				const auto& [id, name] = person;
				cout << "|"
					<< setw(3) << right << id << " | "
					<< setw(20) << left << name << " |\n";
			};
		for_each(cbegin(persons), cend(persons), view);
	}

	void simple_db_viewer::view(std::size_t person_id)
	{
		// Напечатать данные персоны
		using namespace std;
		const auto persons = contacts_.get<size_t, string>(
			"SELECT id, "
			"CONCAT(surname, ' ', forename) AS name "
			"FROM subscriber "
			"WHERE id = '" + to_string(person_id) + "' "
			"ORDER BY name"
		);

		// ID, Name ////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto view = [](const typename decltype(cbegin(persons))::value_type& person)
			{
				const auto& [id, name] = person;
				cout << "ID:       " << id << '\n';
				cout << "Name:     " << name << '\n';
			};
		for_each(cbegin(persons), cend(persons), view);

		// Email address ///////////////////////////////////////////////////////////////////////////////////////////////////
		cout << "Email address\n";
		const auto email_addresses = contacts_.get<size_t, size_t, string>(
			"SELECT id, subscriber_id, "
			"CONCAT(mailbox, '@', hostname) AS email "
			"FROM email_address "
			"WHERE subscriber_id = '" + to_string(person_id) + "' "
			"ORDER BY id"
		);
		this->view(email_addresses);

		// Phone numbers ///////////////////////////////////////////////////////////////////////////////////////////////////
		cout << "Phone numbers\n";
		const auto phone_numbers = contacts_.get<size_t, size_t, string>(
			"SELECT id, subscriber_id, number "
			"FROM phone_numbers "
			"WHERE subscriber_id = '" + to_string(person_id) + "' "
			"ORDER BY id"
		);
		this->view(phone_numbers);

		cout << '\n';
	}

	void simple_db_viewer::view(const auto& recordset)
	{
		using namespace std;
		for_each(cbegin(recordset), cend(recordset),
			[](const typename decltype(cbegin(recordset))::value_type& elem)
			{
				const auto& [id, subscriber_id, value] = elem;
				cout << setw(10) << right << "ID: " << setw(5) << left << id <<  value << '\n';
			}
		);
	}

	user_message simple_db_viewer::get_message(void(*show_menu)())
	{
		show_menu();
		return static_cast<user_message>(
			my::get_input_value<size_t>("\n\nВведите ID команды: ")
			);
	}

	bool simple_db_viewer::get_phone_number(std::size_t person_id, const std::string& invitation) 
		try
	{
		return contacts_.add_phone(person_id, my::get_input_value(invitation));
	}
	catch (...)
	{
		std::cout << "Введите одиннадцатизначный номер в формате +ХХХХХХХХХХХ";
		return get_phone_number(person_id, ": ");
	}

	bool simple_db_viewer::get_email_address(std::size_t person_id, const std::string& invitation)
		try
	{
		return contacts_.add_email(person_id, my::get_input_value(invitation));
	}
	catch (...)
	{
		std::cout << "Введите адрес эл. почты в формате mailbox@hostname";
		return get_email_address(person_id, ": ");
	}
}
