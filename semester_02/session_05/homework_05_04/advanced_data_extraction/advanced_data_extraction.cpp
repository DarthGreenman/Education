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
		phone::print(contacts);			
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
		const auto person_id = my::get_input_value<unsigned>("\nВВЕДИТЕ ID КОНТАКТА: ");
		phone::print(contacts, person_id);
		action(contacts);
	}
	case 2:
	{
		using contact = typename phone::contact;
		const contact person{
			contact::name_type{"Lina", "Rannsun"}, 
			contact::email_address_type{"lina.rannsun@gmail.com"},
			contact::number_list_type{contact::phone_number_type{"+12222532859"}} 
		};
		contacts.add(person);
		action(contacts);
	}
	case 3:
	{
		const auto person_id = my::get_input_value<unsigned>("\nВВЕДИТЕ ID КОНТАКТА: ");
		contacts.add(person_id, phone::contact::phone_number_type{ "+12222432869" });
		action(contacts);
	}
	default:
		break;
	}
}

