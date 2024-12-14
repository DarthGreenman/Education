// simple_db_viewer.h

#ifndef  SIMPLE_DB_VIEWER_H
#define SIMPLE_DB_VIEWER_H

#include "my_input.h"
#include "phone_book.h"


#include <iostream>
#include <string>
#include <utility>

namespace phone
{
	class simple_db_viewer
	{
	public:
		enum user_message { 
			UM_QUIT = 0, 
			UM_EDIT = 1, 
			UM_ADD_CONTACT = 2, 
			UM_DEL_CONTACT = 3, 
			UM_ADD_PHONE = 4, 
			UM_DEL_PHONE = 5,
			UM_REPEAT = 99
		};

		simple_db_viewer() = default;
		simple_db_viewer(phone_book&& contacts);
		simple_db_viewer(const simple_db_viewer&) = delete;
		simple_db_viewer(simple_db_viewer&&) = delete;
		~simple_db_viewer() = default;

		simple_db_viewer& operator=(const simple_db_viewer&) = delete;
		simple_db_viewer& operator=(simple_db_viewer&&) = delete;

		void exec();

	private:
		std::pair<bool, user_message> work();
		std::pair<bool, user_message> work(std::size_t person_id);
		void view();
		void view(std::size_t person_id);
		user_message get_message(void(*show_menu)());		
		
		template<typename T>
		bool get_phone_number(const T& value, const std::string& invitation) try
		{
			return contacts_.add_phone(value, my::get_input_value(invitation));
		}
		catch (...)
		{
			std::cout << "Введите одиннадцатизначный номер в формате +ХХХХХХХХХХХ";
			return get_phone_number(value, ": ");
		}

		std::string get_email_address(const std::string& invitation);

	private:
		phone_book contacts_;
	};
}

#endif // ! SIMPLE_DB_VIEWER_H
