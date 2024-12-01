// advanced_data_extraction.cpp: определяет точку входа для приложения.
//

#include "includes/contact.h"
#include "includes/my_input.h"
#include "includes/my_localisation.h"
#include "includes/phone_book.h"

#include <exception>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace context
{
	void action(phone::phone_book& contacts);
}

int main()
{
	const my::console_localisation<my::code_pages::rus> loc{};

	using contact = typename phone::contact;
	using name_type = typename phone::contact::name_type;
	using address_type = typename phone::contact::email_address_type;
	using number_type = typename phone::contact::phone_number_type;
	using number_list_type = typename phone::contact::number_list_type;

	const std::vector<contact> initial_data
	{
		contact{name_type{"Bjarne", "Stroustrup"}, address_type{"bjarne@stroustrup.com"},
		number_list_type{ number_type{"+19792195004"}, number_type{"+19792195005"}, number_type{"+19792195006"}}},

		contact{name_type{"Inbal", "Levi"}, address_type{"feedback@cppcast.com"},
		number_list_type{}},

		contact{name_type{"Nina", "Ranns"}, address_type{},
		number_list_type{number_type{"+12222532851"}}},

		contact{name_type{"Herb", "Sutter"}, address_type{"herb.sutter@gmail.com"},
		number_list_type{number_type{"+14442681754"}}},

		contact{name_type{"Michael", "Wong"}, address_type{},
		number_list_type{number_type{"+12142631853"}, number_type{"+12144531855"}, number_type{"+12142781343"}} },

		contact{ name_type{"Lorenzo", "Montanelli"}, address_type{"lorenzo_montanelli@gmail.com"},
		number_list_type{number_type{"+19257546470"}}}
	};
	
	try
	{
		const std::string connection_string
		{
			"host=localhost "
			"port=5432 "
			"dbname=telephone_book "
			"user=postgres "
			"password=DarthGreenman1969"
		};

		phone::phone_book contacts{ connection_string, initial_data };
		context::action(contacts);
	}
	catch (const std::exception& err)
	{
		std::cout << err.what();
	}
	
	return 0;
}

void context::action(phone::phone_book& contacts)
{
	using namespace std;
	using contact = typename phone::contact;
	
	system("cls");
	phone::print(contacts);
	cout << "\n\nВЫБЕРЕТЕ ДЕЙСТВИЕ:";
	cout << "\n1. Данные контакта";
	cout << "\n2. Добавить контакт";
	cout << "\n3. Добавить номер";
	cout << "\n4. Удалить контакт";
	cout << "\n5. Удалить номер";

	switch (my::get_input_value<unsigned>("\n\nВВЕДИТЕ КОМАНДУ: "))
	{
	case 1:
	{
		const auto person_id = my::get_input_value<unsigned>("Введите ID контакта: ");
		phone::print(contacts, person_id);
		system("pause");
		action(contacts);
		break;
	}
	case 2: // Добавить контакт
	{
		const contact::name_type name{
			my::get_input_value<string>("Имя: "),
			my::get_input_value<string>("Фамилия: ")
		};
		const contact::email_address_type email{ my::get_input_value<string>("Email: ") };
		const contact::phone_number_type phone_number{ my::get_input_value<string>("Номер телефона: ") };
		
		contacts.add(contact{ name, email });
		contacts.add(name, phone_number);
		action(contacts);
		break;
	}
	case 3: // Добавить номер
	{
		const auto person_id = my::get_input_value<size_t>("Введите ID контакта: ");
		const contact::phone_number_type phone_number{ my::get_input_value<string>("Номер телефона: ") };
		
		contacts.add(person_id, phone_number);
		action(contacts);
		break;
	}
	case 4: // Удалить контакт
	{
		const auto person_id = my::get_input_value<size_t>("Введите ID контакта: ");

		action(contacts);
		break;
	}
	case 5: // Удалить номер
	{
		action(contacts);
		break;
	}
	case 0:
		return;
	}
}

