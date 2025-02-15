// window.h

#ifndef WINDOW_H
#define WINDOW_H

#include "book.h"
#include "command.h"
#include "driver.h"
#include "message.h"
#include "publisher.h"
#include "reserve.h"
#include "sale.h"
#include "store.h"
#include "view.h"
#include "window_base.h"
#include "writer.h"

#include "additions.h"
#include <algorithm>
#include <cstdlib>
#include <iosfwd>
#include <iterator>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <Wt/Dbo/Transaction.h>

namespace orm
{
	namespace win
	{
		class main_window : public window_base
		{
		public:
			using EVENT_MESSAGE = typename window_base::EVENT_MESSAGE;
			using handle = typename window_base::handle;

			main_window() = default;
			main_window(const std::string& connection_string, const std::string& heading) :
				window_base{ connection_string, heading } {
			}
			main_window(const main_window&) = default;
			main_window(main_window&&) = default;
			virtual ~main_window() = default;

			main_window& operator=(const main_window&) = default;
			main_window& operator=(main_window&&) = default;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto show() const
			{
				window_base::show();
				std::cout << std::setiosflags(std::ios::left);
				std::cout << '\n'
					<< "Книги: 1     "
					<< "Авторы: 2     "
					<< "Издательства: 3     "
					<< "Магазины: 4     "
					<< "Зарезервировано: 5     "
					<< "Продажи: 6     "
					<< "Выход: 0"
					<< "\n\n";
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto exec(std::pair<std::string, handle>& hwnd, EVENT_MESSAGE& message,
				const std::string& promt = "Введите ID команды: ")
			{
				show();
				window_base::command<com::command_mwnd>(message);
				if (message == EVENT_MESSAGE::MW_QUIT)
					return;
				hwnd = window_base::get_handle(message);
			}
		};

		class child_window : public window_base
		{
		public:
			using EVENT_MESSAGE = typename window_base::EVENT_MESSAGE;
			using handle = typename window_base::handle;

			child_window() = default;
			child_window(const std::string& connection_string, const std::string& heading) :
				window_base{ connection_string, heading } {
			}
			child_window(const child_window&) = default;
			child_window(child_window&&) = default;
			virtual ~child_window() = default;

			child_window& operator=(const child_window&) = default;
			child_window& operator=(child_window&&) = default;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto show() const
			{
				window_base::show();
				std::cout << std::setiosflags(std::ios::left);
				std::cout << '\n'
					<< "Редактировать: 8     "
					<< "Добавить: 9     "
					<< "Удалить: 10     "
					<< "Выход: 7"
					<< "\n\n";
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto exec(handle hwnd, EVENT_MESSAGE& message,
				const std::string& promt = "Введите ID команды: ")
			{
				for (;;)
				{
					show();
					update(hwnd);
					window_base::command<com::command_cwnd>(message);
					if (message == EVENT_MESSAGE::CW_QUIT)
						return;
					wndproc(hwnd, message);
				}
			}

		private:
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto update(handle hwnd) -> void
			{
				using namespace tps;
				hwnd == book::hash() ? update<book, writer, publisher>() :
					hwnd == writer::hash() ? update<writer>() :
					hwnd == publisher::hash() ? update<publisher>() :
					hwnd == store::hash() ? update<store>() :
					hwnd == reserve::hash() ? update<reserve, book, store>() :
					update<sale, reserve>();
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename Table_type, typename... Tabular_types>
				requires Is_tabulartypes<Table_type> && (Is_tabulartypes<Tabular_types> && ...)
			auto update()
			{
				Wt::Dbo::Transaction work{ *(session()) };
				{
					auto data = get<Table_type, Tabular_types...>();
					using namespace std;
					using Value_type = typename iterator_traits<decay_t<decltype(cbegin(data))>>::value_type;
					for_each(cbegin(data), cend(data),
						[](const Value_type& value) { tps::view(value); });
				}
				work.commit();
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto edit(handle hwnd) 
			{
				std::cout << "WM_MESSAGE::CW_EDIT\n";
				std::system("pause");
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto add(handle hwnd)
			{
				// добавить для типов: writer, publisher, store
				std::cout << "WM_MESSAGE::CW_ADD\n";
				std::system("pause");
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto remove(handle hwnd)
			{
				// добавить для sale
				std::cout << "WM_MESSAGE::CW_REM\n";
				std::system("pause");
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto wndproc(handle hwnd, EVENT_MESSAGE message) -> void
			{
				switch (message)
				{
					case EVENT_MESSAGE::CW_EDIT:
						edit(hwnd);
						break;
					case EVENT_MESSAGE::CW_ADD:
						add(hwnd);
						break;
					case EVENT_MESSAGE::CW_REM:
						remove(hwnd);
						break;
				}
			}
		};
	} // namespace win
} // namespace orm

#endif // !WINDOW_H
