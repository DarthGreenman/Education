// simple_db_viewer.h

#ifndef  SIMPLE_DB_VIEWER_H
#define SIMPLE_DB_VIEWER_H

#include "phone_book.h"

#include <string>
#include <utility>

namespace phone
{
	class simple_db_viewer
	{
	public:
		using size_t = typename std::size_t;
		using string = typename std::string;
		using phone_number_type = typename phone_book::phone_number_type;
		using email_address_type = typename phone_book::email_address_type;

		enum user_message { 
			UM_QUIT = 0, 
			UM_EDIT = 1, 
			UM_ADD_CONTACT = 2, 
			UM_DEL_CONTACT = 3, 
			UM_ADD_PHONE = 4, 
			UM_DEL_PHONE = 5,
			UM_ADD_EMAIL = 6,
			UM_DEL_EMAIL = 7,
			UM_MOD_FORENAME = 8,
			UM_MOD_SURNAME = 9,
			UM_MOD_PHONE = 10,
			UM_MOD_EMAIL = 11,
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
		void view(size_t person_id);
		void view(const pqxx::internal::result_iteration<size_t, size_t, string>& recordset);
		
		user_message get_message(void(*show_menu)());		
		
		phone_number_type get_phone(const string& invitation) const;
		email_address_type get_email(const string& invitation) const;

	private:
		phone_book contacts_;
	};
}

#endif // ! SIMPLE_DB_VIEWER_H
