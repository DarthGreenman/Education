/// logger.h

#ifndef LOGGER_H_IN_MY_PROJECT
#define LOGGER_H_IN_MY_PROJECT

#include <fstream>
#include <iosfwd>
#include <string>

namespace pattern
{
	/// Ресивер
	struct logger
	{
		void send_to_console(std::ostream& os, const std::string& message) const
		{
			os << __FUNCTION__ << "\tmessage: " << message << '\n';
		}

		void send_to_journal(std::ofstream& of, const std::string& message) const
		{
			of << __FUNCTION__ << "\tmessage: " << message << '\n';
		}
	}; /// struct logger

} /// namespace pattern

#endif // !LOGGER_H_IN_MY_PROJECT

