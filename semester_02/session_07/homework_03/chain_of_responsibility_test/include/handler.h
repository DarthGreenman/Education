/// handler.h

#ifndef HANDLER_H_IN_MY_PROJECT
#define HANDLER_H_IN_MY_PROJECT

#include "logger.h"
#include <memory>
#include <stdexcept>
#include <string>

namespace pattern
{	
	namespace chain_of_responsibility
	{
		class log_handler
		{
		public:
			void next(std::shared_ptr<log_handler> next) { _next = next; }
			void receiver(const logger& log)
			{
				if (can_handler() == log.type())
				{
					handle(log.message());
					return;
				}
				_next ? _next->receiver(log) : throw std::runtime_error{
						"\nRuntime error, no handler found for this type of message: " + std::string{__FUNCTION__} +
						"\nfile: " + std::string{ __FILE__ } + "\nline: " + std::to_string(__LINE__)
					};
			}
			virtual ~log_handler() = default;

		protected:
			virtual logger::level can_handler() const = 0;
			virtual void handle(const std::string& msg) = 0;

		private:
			std::shared_ptr<log_handler> _next{};

		}; /// class log_handler
	} /// namespace chain_of_responsibility
} /// namespace pattern

#endif // !HANDLER_H_IN_MY_PROJECT
