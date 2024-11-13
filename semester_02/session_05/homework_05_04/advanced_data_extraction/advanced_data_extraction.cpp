// advanced_data_extraction.cpp: определяет точку входа для приложения.
//

#include "includes/my_localisation.h"
#include "includes/phone_book.h"

#include <exception>
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <utility>
#include <vector>

int main()
{
	const my::console_localisation<my::code_pages::rus> loc{};
	const char* connection_string
	{
		"host=localhost "
		"port=5432 "
		"dbname=telephone_book "
		"user=postgres "
		"password=DarthGreenman1969"
	};

	using numbers = std::vector<std::string>;

	const std::vector<phone::contact> initial_data{
		phone::contact{std::make_pair("Bjarne", "Stroustrup"), "bjarne@stroustrup.com", numbers{"+19792195004", "+19792195005", "+19792195006"}},
		phone::contact{std::make_pair("Inbal", "Levi"), "feedback@cppcast.com", numbers{}},
		phone::contact{std::make_pair("Nina", "Ranns"), "", numbers{}},
		phone::contact{std::make_pair("Herb", "Sutter"), "herb.sutter@gmail.com", numbers{}},
		phone::contact{std::make_pair("Michael", "Wong"), "", numbers{"+12142631853"}} 
	};
	
	try
	{
		//phone::phone_book customer{ pqxx::connection{connection_string} };
		phone::phone_book customer{ pqxx::connection{connection_string}, initial_data };
		/*
		auto result = customer.add_contact(phone::contact{std::make_pair("Bjarne", "Stroustrup"),
			"bjarne@stroustrup.com" });
		result = customer.add_number(phone::contact{ std::make_pair("Bjarne", "Stroustrup"), "bjarne@stroustrup.com",
			std::vector<std::string>{"+19792195004", "+19792195005"} } );
		*/
	}
	catch (const std::exception& err)
	{
		std::cout << err.what();
	}
	return 0;
}
