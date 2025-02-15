// message.h

#ifndef MESSAGE_H
#define MESSAGE_H


namespace orm
{
	namespace com
	{
		enum class EVENT_MESSAGE : unsigned {
			MW_QUIT = 0,    // 0
			MW_BOOKS = 1,   // 1
			MW_WRITERS,     // 2
			MW_PUBLISHERS,  // 3
			MW_STORES,      // 4
			MW_RESERVES,    // 5
			MW_SALES,       // 6
			CW_QUIT = 7,    // 7
			CW_EDIT,        // 8
			CW_ADD,         // 9
			CW_REM,         // 10
			MW_REPEAT = 99,    // 99
			// Начало оконных сообщений
			BEG_MESSAGE    = EVENT_MESSAGE::MW_QUIT,
			// Сообщения главного окна
			MW_BEG_MESSAGE = EVENT_MESSAGE::MW_QUIT,
			MW_END_MESSAGE = EVENT_MESSAGE::MW_SALES,
			// Сообщения окна
			CW_BEG_MESSAGE = EVENT_MESSAGE::CW_QUIT,
			CW_END_MESSAGE = EVENT_MESSAGE::CW_REM,
			// Конец оконных сообщений
			END_MESSAGE    = EVENT_MESSAGE::MW_REPEAT
		};

		EVENT_MESSAGE& operator++(EVENT_MESSAGE& um)
		{
			um = static_cast<EVENT_MESSAGE>(static_cast<unsigned>(um) + 1);
			return um;
		}

		EVENT_MESSAGE operator++(EVENT_MESSAGE& um, int)
		{
			EVENT_MESSAGE prev{ um };
			um = static_cast<EVENT_MESSAGE>(static_cast<unsigned>(um) + 1);
			return prev;
		}

		EVENT_MESSAGE beg()
		{
			return EVENT_MESSAGE::BEG_MESSAGE;
		}

		EVENT_MESSAGE end()
		{
			EVENT_MESSAGE last = EVENT_MESSAGE::END_MESSAGE;
			return ++last;
		}

		auto is_mwnd_command(EVENT_MESSAGE message)
		{
			return message != EVENT_MESSAGE::CW_EDIT && message != EVENT_MESSAGE::CW_ADD && 
				message != EVENT_MESSAGE::CW_REM;
		}

	} // namespace com
} // namespace orm

#include <istream>
#include <iosfwd>

std::istream& operator>> (std::istream& in, orm::com::EVENT_MESSAGE& message)
{
	unsigned val{};
	in >> val;
	message = static_cast<orm::com::EVENT_MESSAGE>(val);
	return in;
}


#endif // !MESSAGE_H
