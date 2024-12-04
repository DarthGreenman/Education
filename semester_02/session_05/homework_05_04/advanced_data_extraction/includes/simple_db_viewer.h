// simple_db_viewer.h

#ifndef  SIMPLE_DB_VIEWER_H
#define SIMPLE_DB_VIEWER_H

#include "phone_book.h"

namespace phone
{
	class simple_db_viewer
	{
	public:
		simple_db_viewer() = default;
		simple_db_viewer(phone_book&& contacts);
		simple_db_viewer(const simple_db_viewer&) = delete;
		simple_db_viewer(simple_db_viewer&&) = delete;
		~simple_db_viewer() = default;

		simple_db_viewer& operator=(const simple_db_viewer&) = delete;
		simple_db_viewer& operator=(simple_db_viewer&&) = delete;

		void action();

	private:
		void view();
		void view(std::size_t person_id);

	private:
		phone_book contacts_;
	};
}

#endif // ! SIMPLE_DB_VIEWER_H
