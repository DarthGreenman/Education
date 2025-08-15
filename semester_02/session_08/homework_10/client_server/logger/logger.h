/// logger.h

#ifndef LOGGER_H_IN_MY_PROJECT
#define LOGGER_H_IN_MY_PROJECT

#include "../common/protocol.h"
#include <qdatetime.h>

namespace event
{
	class Logger
	{
	public:
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		static Logger& getInstance()
		{
			static Logger instance;
			return instance;
		}
		Logger& send(qintptr socketDescriptor, protocol::RequestTypeMessages typeMsg, const QDateTime& timePoint)
		{
			_socketDescriptor = socketDescriptor;
			_typeMsg = typeMsg;
			_timePoint = timePoint;
			return *this;
		}
		auto type() const { return _typeMsg; }
		auto timePoint() const { return _timePoint; }
		auto socketDescriptor() const { return _socketDescriptor; }

	private:
		Logger() = default;
		qintptr _socketDescriptor{};
		protocol::RequestTypeMessages _typeMsg{ protocol::RequestTypeMessages::Unknown };
		QDateTime _timePoint{ QDateTime::currentDateTime() };
	
	}; /// class logger 
} /// namespace pattern

#endif // !LOGGER_H_IN_MY_PROJECT
