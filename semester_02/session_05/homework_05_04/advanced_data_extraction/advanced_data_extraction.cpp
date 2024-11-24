// advanced_data_extraction.cpp: определяет точку входа для приложения.
//

#include "includes/contact.h"
#include "includes/my_localisation.h"
#include "includes/phone_book.h"

#include <exception>
#include <iostream>
#include <pqxx/connection.hxx>
#include <string>
#include <vector>

int main()
{
	const my::console_localisation<my::code_pages::rus> loc{};

	const std::string connection_string
	{
		"host=localhost "
		"port=5432 "
		"dbname=telephone_book "
		"user=postgres "
		"password=DarthGreenman1969"
	};

	using contact = phone::contact;
	using name_type = phone::contact::name_type;
	using address_type = phone::contact::email_address_type;
	using number_type = phone::contact::phone_number_type;
	using number_list_type = phone::contact::number_list_type;
	
	const contact lorenzo{ name_type{"Lorenzo", "Montanelli"}, address_type{"lorenzo_montanelli@gmail.com"},
		number_list_type{ number_type{"+19792195004"}, number_type{"+19792195005"}, number_type{"+19792195006"}} };

	/**/
	const std::vector<contact> initial_data
	{
		contact{name_type{"Bjarne", "Stroustrup"}, address_type{"bjarne@stroustrup.com"},
		number_list_type{ number_type{"+19792195004"}, number_type{"+19792195005"}, number_type{"+19792195006"}}},

		contact{name_type{"Inbal", "Levi"}, address_type{"feedback@cppcast.com"},
		number_list_type{number_type{}}},

		contact{name_type{"Nina", "Ranns"}, address_type{},
		number_list_type{number_type{"+12222532851"}}},

		contact{name_type{"Herb", "Sutter"}, address_type{"herb.sutter@gmail.com"},
		number_list_type{number_type{"+14442681754"}}},

		contact{name_type{"Michael", "Wong"}, address_type{},
		number_list_type{number_type{"+12142631853"}, number_type{"+12144531855"}, number_type{"+12142781343"}} },

		contact{ name_type{"Lorenzo", "Montanelli"}, address_type{"lorenzo_montanelli@gmail.com"},
		number_list_type{number_type{"+19257546470"}}}
	};
	/**/
	try
	{
		//phone::phone_book contacts{ pqxx::connection{connection_string} };
		phone::phone_book contacts{ pqxx::connection{connection_string}, initial_data };
		//const phone::contact member{ std::make_pair("Dmitry", "Kostyuchenko"), "dima.kostyuchenko@gmail.com", numbers{"+79257546474"} };
		//contacts.add_contact(member);
		//phone::print(contacts.get());
	}
	catch (const std::exception& err)
	{
		std::cout << err.what();
	}
	
	return 0;
}
