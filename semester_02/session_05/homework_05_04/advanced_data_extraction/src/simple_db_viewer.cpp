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
		cout << "\nИзменить имя                " << '8';
		cout << "\nИзменить фамилию            " << '9';
		cout << "\nИзменить номер телефона     " << "10";
		cout << "\nИзменить email              " << "11";
		cout << "\nВыход                       " << '0';
	}

	using size_t = typename std::size_t;
	using string = typename std::string;
	using user_message = typename simple_db_viewer::user_message;
	using phone_number_type = typename phone_book::phone_number_type;
	using email_address_type = typename phone_book::email_address_type;

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
				um == UM_MOD_FORENAME ? "UM_MOD_FORENAME" :
				um == UM_MOD_SURNAME ? "UM_MOD_SURNAME" :
				um == UM_MOD_PHONE ? "UM_MOD_PHONE" :
				um == UM_MOD_EMAIL ? "UM_MOD_EMAIL" :
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
		{
			const auto person_id = get_input_value<size_t>("Введите ID контакта: ");
			return make_pair(contacts_.del_contact(person_id), UM_DEL_CONTACT);
		}
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
		{
			const auto phone_number = get_phone("Номер телефона: ");
			return make_pair(contacts_.add_phone(person_id, phone_number), UM_ADD_PHONE);
		}
		case UM_DEL_PHONE:
		{
			const auto phone_number_id = get_input_value<size_t>("Введите ID телефона: ");
			return make_pair(contacts_.del_phone(phone_number_id), UM_DEL_PHONE);
		}
		case UM_ADD_EMAIL:
		{
			const auto email = get_email("Email: ");
			return make_pair(contacts_.add_email(person_id, email), UM_ADD_EMAIL);
		}
		case UM_DEL_EMAIL:
		{
			const auto email_id = get_input_value<size_t>("Введите ID email: ");
			return make_pair(contacts_.del_email(email_id), UM_DEL_EMAIL);
		}
		case UM_MOD_FORENAME:
		{
			const auto new_forename = get_input_value("Новое имя: ");
			return make_pair(contacts_.mod_forename(person_id, new_forename), UM_MOD_FORENAME);
		}
		case UM_MOD_SURNAME:
		{
			const auto new_surname = get_input_value("Новая фамилия: ");
			return make_pair(contacts_.mod_surname(person_id, new_surname), UM_MOD_SURNAME);
		}
		case UM_MOD_PHONE:
		{
			const auto phone_number_id = get_input_value<size_t>("Введите ID телефона: ");
			const auto new_phone_number = get_phone("Новый номер: ");
			return make_pair(contacts_.mod_phone(phone_number_id, new_phone_number), UM_MOD_PHONE);
		}
		case UM_MOD_EMAIL:
		{
			const auto email_id = get_input_value<size_t>("Введите ID email: ");
			const auto new_email = get_email("Новый email: ");
			return make_pair(contacts_.mod_email(email_id, new_email), UM_MOD_EMAIL);
		}
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
		
		auto print = [](const typename decltype(cbegin(persons))::value_type& person)
			{
				const auto& [id, name] = person;
				cout << "|"
					<< setw(3) << right << id << " | "
					<< setw(20) << left << name << " |\n";
			};
		for_each(cbegin(persons), cend(persons), print);
	}

	void simple_db_viewer::view(size_t person_id)
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
		auto print = [](const typename decltype(cbegin(persons))::value_type& person)
			{
				const auto& [id, name] = person;
				cout << "ID:       " << id << '\n';
				cout << "Name:     " << name << '\n';
			};
		for_each(cbegin(persons), cend(persons), print);

		// Email address ///////////////////////////////////////////////////////////////////////////////////////////////////
		cout << "Email address\n";
		const auto email_addresses = contacts_.get<size_t, size_t, string>(
			"SELECT id, subscriber_id, "
			"CONCAT(mailbox, '@', hostname) AS email "
			"FROM email_address "
			"WHERE subscriber_id = '" + to_string(person_id) + "' "
			"ORDER BY id"
		);
		view(email_addresses);

		// Phone numbers ///////////////////////////////////////////////////////////////////////////////////////////////////
		cout << "Phone numbers\n";
		const auto phone_numbers = contacts_.get<size_t, size_t, string>(
			"SELECT id, subscriber_id, number "
			"FROM phone_numbers "
			"WHERE subscriber_id = '" + to_string(person_id) + "' "
			"ORDER BY id"
		);
		view(phone_numbers);

		cout << '\n';
	}

	void simple_db_viewer::view(const pqxx::internal::result_iteration<size_t, size_t, string>& recordset)
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

	phone_book::phone_number_type simple_db_viewer::get_phone(const string& invitation) const
		try
	{
		return my::get_input_value(invitation);
	}
	catch (...)
	{
		std::cout << "Введите одиннадцатизначный номер в формате +ХХХХХХХХХХХ";
		return get_phone(": ");
	}

	phone_book::email_address_type simple_db_viewer::get_email(const string& invitation) const
		try
	{
		return my::get_input_value(invitation);
	}
	catch (...)
	{
		std::cout << "Введите адрес эл. почты в формате mailbox@hostname";
		return get_email(": ");
	}

}
