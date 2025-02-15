// command.h

#ifndef COMMAND_H
#define COMMAND_H

#include "book.h"
#include "message.h"
#include "publisher.h"
#include "reserve.h"
#include "sale.h"
#include "store.h"
#include "writer.h"
#include "app.h"

#include <concepts>
#include <cstdlib>
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

/*
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
MW_REPEAT = 99, // 99
*/

namespace orm
{
	namespace com
	{
		namespace helper
		{
			auto is_correct(EVENT_MESSAGE beg, EVENT_MESSAGE end, EVENT_MESSAGE message)
			{
				for (auto ms = beg; ms < end; ++ms)
					if (message == ms)
						return true;
				return false;
			}
		} // namespace helper

		struct command_mwnd
		{
			const static auto beg() { return EVENT_MESSAGE::MW_BEG_MESSAGE; }
			const static auto end() 
			{ 
				EVENT_MESSAGE last = EVENT_MESSAGE::MW_END_MESSAGE;
				return ++last;
			}
			const static auto is_correct(com::EVENT_MESSAGE message)
			{
				return helper::is_correct(beg(), end(), message);
			}
		};
		struct command_cwnd
		{		
			const static auto beg() { return EVENT_MESSAGE::CW_BEG_MESSAGE; }
			const static auto end() 
			{ 
				EVENT_MESSAGE last = EVENT_MESSAGE::CW_END_MESSAGE;
				return ++last;
			}
			const static auto is_correct(com::EVENT_MESSAGE message)
			{
				return helper::is_correct(beg(), end(), message);
			}
		};
	} // namespace win

	template<typename M>
	concept Is_command = std::is_same_v<M, com::command_mwnd> || std::is_same_v<M, com::command_cwnd>;

} // namespace orm

#endif // !COMMAND_H
