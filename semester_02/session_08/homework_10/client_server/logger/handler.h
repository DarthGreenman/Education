/// handler.h

#ifndef HANDLER_H_IN_MY_PROJECT
#define HANDLER_H_IN_MY_PROJECT

#include "../common/protocol.h"
#include "logger.h"

#include <memory>
#include <stdexcept>
#include <string>

namespace event
{
	class LogHandler
	{
	public:
		void next(std::shared_ptr<LogHandler> next) { _next = next; }
		void receiver(const Logger& log)
		{
			if (const auto type = log.type(); canHandler() == type) {
				handle(log.socketDescriptor(), type, log.timePoint());
				return;
			}
			_next ? _next->receiver(log) : throw std::runtime_error{
					"\nRuntime error, no handler found for this type of message: " + std::string{__FUNCTION__} +
					"\nfile: " + std::string{ __FILE__ } + "\nline: " + std::to_string(__LINE__)
			};
		}
		virtual ~LogHandler() = default;

	protected:
		virtual protocol::RequestTypeMessages canHandler() const = 0;
		virtual void handle(qintptr socketDescriptor, protocol::RequestTypeMessages typeMsg, 
			const QDateTime timePoint) = 0;

	private:
		std::shared_ptr<LogHandler> _next{};

	}; /// class LogHandler
} /// namespace event

#endif // !HANDLER_H_IN_MY_PROJECT
