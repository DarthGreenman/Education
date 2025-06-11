/// logger.h

#ifndef LOGGER_H_IN_MY_PROJECT
#define LOGGER_H_IN_MY_PROJECT

#include <iterator>
#include <string>
#include <type_traits>

namespace pattern
{
	namespace chain_of_responsibility
	{
		class logger
		{
		public:
			enum class level  {
				unknown = 0, warning, error, fatal
			};

			logger(const logger&) = delete;
			logger& operator=(const logger&) = delete;

			static logger& get_instance()
			{
				static logger instance;
				return instance;
			}
			logger& send(level lev, const std::string& msg)
			{
				construct(lev, msg);
				return *this;
			}
			logger& send(level lev, std::string&& msg)
			{
				construct(lev, std::move(msg));
				return *this;
			}

			level type() const { return _level; }
			const std::string& message() const { return _message; }

		private:
			logger() = default;
			void construct(level lev, const std::string& msg)
			{
				_message.clear();
				std::copy(std::cbegin(msg), std::cend(msg), 
					std::back_inserter(_message));
				_level = lev;
			}
			void construct(level lev, std::string&& msg)
			{
				_message = std::move(msg);
				_level = lev;
			}

			std::string _message{};
			level _level{ level::unknown };

		}; /// class logger 
	} /// namespace chain_of_responsibility
} /// namespace pattern

#endif // !LOGGER_H_IN_MY_PROJECT
