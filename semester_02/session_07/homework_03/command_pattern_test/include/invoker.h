/// invoker.h

#ifndef INVOKER_H_IN_MY_PROJECT
#define INVOKER_H_IN_MY_PROJECT

#include "log_command.h"
#include <memory>
#include <string>
#include <type_traits>

namespace pattern
{
	/// Инициатор
	class invoker {
	public:
		invoker(std::unique_ptr<log_command> command) : _command{ std::move(command) } {}

		void send(const std::string& message) const
		{
			if (_command) 
				_command->send(message);
		}

	private:
		std::unique_ptr<log_command> _command{};
	};

} /// namespace pattern

#endif // !INVOKER_H_IN_MY_PROJECT
