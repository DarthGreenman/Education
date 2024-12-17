// advanced_data_extraction.cpp: определяет точку входа для приложения.
//

#include "includes/contact.h"
#include "includes/my_localisation.h"
#include "includes/phone_book.h"
#include "includes/simple_db_viewer.h"

#include <exception>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#define NDEMO

int main()
{
	const my::console_localisation<my::code_pages::rus> loc{};

	using contact = typename phone::contact;
	using name_type = typename phone::contact::name_type;
	using address_type = typename phone::contact::email_address_type;
	using email_list_type = typename phone::contact::email_list_type;
	using number_type = typename phone::contact::phone_number_type;
	using number_list_type = typename phone::contact::number_list_type;
	
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

		phone::phone_book contacts{ connection_string };
#ifdef DEMO
		const std::vector<contact> demo_data
		{
			contact{name_type{"Bjarne", "Stroustrup"},
			email_list_type{address_type{"bjarne@stroustrup.com"}, address_type{"feedback@cppcast.com"}},
			number_list_type{number_type{"+19792195004"}, number_type{"+19792195005"}, number_type{"+19792195006"}}},

			contact{name_type{"Inbal", "Levi"},
			email_list_type{address_type{"inbal.levi@cppcast.com"}, address_type{"feedback@cppcast.com"}}},

			contact{name_type{"Nina", "Ranns"},
			email_list_type{address_type{}},
			number_list_type{number_type{"+12222532851"}}},

			contact{name_type{"Herb", "Sutter"},
			email_list_type{address_type{"herb.sutter@gmail.com"}},
			number_list_type{number_type{"+14442681754"}}},

			contact{name_type{"Michael", "Wong"},
			email_list_type{address_type{}},
			number_list_type{number_type{"+12142631853"}, number_type{"+12144531855"}, number_type{"+12142781343"}} },

			contact{ name_type{"Lorenzo", "Montanelli"},
			email_list_type{address_type{"lorenzo_montanelli@gmail.com"}, address_type{"dima.kostyuchenko@gmail.com"}},
			number_list_type{number_type{"+19257546470"}}}
		};
		contacts.loading_data(demo_data);
#endif // !DEMO
		phone::simple_db_viewer notebook{ std::move(contacts) };
		notebook.exec();
	}
	catch (const std::exception& err)
	{
		std::cout << err.what();
	}
	
	return 0;
}
