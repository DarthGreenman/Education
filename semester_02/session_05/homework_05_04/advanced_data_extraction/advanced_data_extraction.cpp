// advanced_data_extraction.cpp: определяет точку входа для приложения.
//

#include "includes/my_localisation.h"
#include "includes/phone_book.h"
#include "includes/contact.h"

#include <exception>
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <utility>
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
	
	using numbers = std::vector<std::string>;
	using contact = phone::contact;

	const std::vector<contact> initial_data{
		contact{std::make_pair("Bjarne", "Stroustrup"), "bjarne@stroustrup.com", numbers{"+19792195004", "+19792195005", "+19792195006"}},
		contact{std::make_pair("Inbal", "Levi"), "feedback@cppcast.com", numbers{}},
		contact{std::make_pair("Nina", "Ranns"), "", numbers{"+12222532851"}},
		contact{std::make_pair("Herb", "Sutter"), "herb.sutter@gmail.com", numbers{"+14442681754"}},
		contact{std::make_pair("Michael", "Wong"), "", numbers{"+12142631853", "+12144531855", "+12142781343"}},
		contact{std::make_pair("Lorenzo", "Montanelli"), "lorenzo_montanelli@gmail.com", numbers{"+79257546470"}}
	};

	try
	{
		//phone::phone_book contacts{ pqxx::connection{connection_string} };
		phone::phone_book contacts{ pqxx::connection{connection_string}, initial_data };
		const phone::contact member{ std::make_pair("Dmitry", "Kostyuchenko"), "dima.kostyuchenko@gmail.com", numbers{"+79257546474"} };
		contacts.add_contact(member);
		phone::print(contacts.get());
	}
	catch (const std::exception& err)
	{
		std::cout << err.what();
	}
	return 0;
}
