/// log_command.h

#ifndef LOG_COMMAND_H_IN_MY_PROJECT
#define LOG_COMMAND_H_IN_MY_PROJECT

#include "logger.h"
#include <iosfwd>
#include <memory>
#include <string>
#include <type_traits>

namespace pattern
{
	/// Абстрактная команда
	struct log_command
	{
		virtual ~log_command() = default;
		virtual void send(const std::string& message) const = 0;
	}; /// struct log_command

	/// Конкретная команда
	class send_to_console : public log_command
	{
	public:
		send_to_console() = default;
		send_to_console(std::ostream& os, std::unique_ptr<logger> log)
			: _os{ os }, _log{ std::move(log) } {
		}
		send_to_console(const send_to_console&) = delete;
		send_to_console(send_to_console&&) = default;
		virtual ~send_to_console() = default;

		send_to_console& operator=(const send_to_console&) = delete;
		send_to_console& operator=(send_to_console&&) = delete;

		void send(const std::string& message) const override final
		{
			_log->send_to_console(_os, message);
		}
	private:
		std::ostream& _os;
		std::unique_ptr<logger> _log{};
	}; /// class send_to_console

	/// Конкретная команда
	struct send_to_journal : public log_command
	{
	public:
		send_to_journal() = default;
		send_to_journal(std::ofstream& of, std::unique_ptr<logger> log)
			: _of{ of }, _log{ std::move(log) } {
		}
		send_to_journal(const send_to_journal&) = delete;
		send_to_journal(send_to_journal&&) = default;
		virtual ~send_to_journal() = default;

		send_to_journal& operator=(const send_to_journal&) = delete;
		send_to_journal& operator=(send_to_journal&&) = delete;

		void send(const std::string& message) const override final
		{
			_log->send_to_journal(_of, message);
		}
	private:
		std::ofstream& _of;
		std::unique_ptr<logger> _log{};
	}; /// class send_to_journal

} /// namespace pattern

#endif // !LOG_COMMAND_H_IN_MY_PROJECT

