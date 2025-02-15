// app.h

#ifndef APP_H
#define APP_H

#include "message.h"
#include "window.h"
#include "window_base.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

namespace orm
{
	class app
	{
	public:
		using EVENT_MESSAGE = typename win::window_base::EVENT_MESSAGE;
		using handle = typename win::window_base::handle;

		app() = default;
		app(const std::string& connection_string, const std::string& heading) : 
			_connection_string{ connection_string },
			_pmwnd{ std::make_unique<win::main_window>(connection_string, heading) } {
		}
		app(const app&) = default;
		app(app&&) = default;
		~app() = default;

		app& operator=(const app&) = default;
		app& operator=(app&&) = default;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto exec()
		{
			for (EVENT_MESSAGE message{ EVENT_MESSAGE::MW_REPEAT };	; ) 
			{
				auto hwnd = std::make_pair(std::string{}, handle{});
				_pmwnd->exec(hwnd, message);

				if (message == EVENT_MESSAGE::MW_QUIT)
					return;

				if (!is_window_registered(hwnd.second))
					create_window(hwnd.first, hwnd.second);

				const auto& pcwnd = _hcwnds.at(hwnd.second);
				pcwnd->exec(hwnd.second, message);
			}
		}

	private:
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto is_window_registered(handle hndl) -> bool
		{
			return _hcwnds.find(hndl) != _hcwnds.cend();
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto create_window(const std::string& heading, handle hndl) -> void
		{
			_hcwnds.emplace(hndl, std::make_unique<win::child_window>(_connection_string, heading));
		}
	
	private:
		std::string _connection_string{};
		std::unique_ptr<win::main_window> _pmwnd{};
		std::unordered_map<
			handle,
			std::unique_ptr<win::child_window>
		> _hcwnds{};
	};

} // namespace orm

#endif // !APP_H