// window_base.h

#ifndef WINDOW_BASE_H
#define WINDOW_BASE_H

#include "book.h"
#include "command.h"
#include "driver.h"
#include "message.h"
#include "publisher.h"
#include "reserve.h"
#include "sale.h"
#include "store.h"
#include "writer.h"

#include <cstdlib>
#include <string>
#include <unordered_map>
#include <utility>

namespace orm
{
	namespace win
	{
		class window_base : public drv::driver
		{
		public:
			using EVENT_MESSAGE = typename com::EVENT_MESSAGE;
			using handle = typename std::size_t;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			window_base() = default;
			window_base(const std::string& connection_string, const std::string& heading) :
				drv::driver{ connection_string }, _heading{ heading } {
			}
			window_base(const window_base&) = default;
			window_base(window_base&&) = default;
			virtual ~window_base() = default;

			window_base& operator=(const window_base&) = default;
			window_base& operator=(window_base&&) = default;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto show() const
			{
				std::system("cls");
				std::cout << _heading;
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename Command>
				requires Is_command<Command>
			auto command(EVENT_MESSAGE& message, const std::string& promt = "¬‚Â‰ËÚÂ ID ÍÓÏ‡Ì‰˚: ") const
			{
				for (; std::cout << promt, std::cin >> message;)
					if (Command::is_correct(message))
						return;
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto get_handle(EVENT_MESSAGE message) const { return _wnds.at(message); }

		private:
			std::string _heading{};
			static inline std::unordered_map<
				EVENT_MESSAGE, std::pair<decltype(_heading), handle>> _wnds
			{
				{ EVENT_MESSAGE::MW_BOOKS,      std::make_pair(" Õ»√»",           tps::book::hash()) },
				{ EVENT_MESSAGE::MW_WRITERS,    std::make_pair("œ»—¿“≈À»",        tps::writer::hash()) },
				{ EVENT_MESSAGE::MW_PUBLISHERS, std::make_pair("»«ƒ¿“≈À‹—“¬¿",    tps::publisher::hash()) },
				{ EVENT_MESSAGE::MW_STORES,     std::make_pair("Ã¿√¿«»Õ€",        tps::store::hash()) },
				{ EVENT_MESSAGE::MW_RESERVES,   std::make_pair("«¿–≈«≈–¬»–Œ¬¿ÕŒ", tps::reserve::hash()) },
				{ EVENT_MESSAGE::MW_SALES,      std::make_pair("œ–Œƒ¿∆»",         tps::sale::hash()) }
			};
		};
	}// namespace win

} // namespace orm

#endif // !WINDOW_BASE_H
