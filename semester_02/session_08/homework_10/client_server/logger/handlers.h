/// handlers.h

#ifndef HANDLERS_H_IN_MY_PROJECT
#define HANDLERS_H_IN_MY_PROJECT

#include "../common/protocol.h"
#include "handler.h"
#include "logger.h"

#include <QtLogging>

namespace event
{
	class GetTime : public LogHandler
	{
	public:
		GetTime() = default;
		virtual ~GetTime() = default;

	private:
		protocol::RequestTypeMessages canHandler() const override final {
			return protocol::RequestTypeMessages::GetTime;
		}
		void handle(qintptr socketDescriptor, protocol::RequestTypeMessages typeMsg, 
			const QDateTime timePoint) override final 
		{
			qDebug() << timePoint.toString() 
				<< "server current time request completed for socket: " << socketDescriptor;
		}

	}; /// class GetTime

	class GetFreeSpace : public LogHandler
	{
	public:
		GetFreeSpace() = default;
		virtual ~GetFreeSpace() = default;

	private:
		protocol::RequestTypeMessages canHandler() const override final {
			return protocol::RequestTypeMessages::GetFreeSpace;
		}
		void handle(qintptr socketDescriptor, protocol::RequestTypeMessages typeMsg, 
			const QDateTime timePoint) override final
		{
			qDebug() << timePoint.toString() 
				<< "server free space request completed for socket: " << socketDescriptor;
		}

	}; /// class GetFreeSpace

	class GetStat : public LogHandler
	{
	public:
		GetStat() = default;
		virtual ~GetStat() = default;

	private:
		protocol::RequestTypeMessages canHandler() const override final {
			return protocol::RequestTypeMessages::GetStat;
		}
		void handle(qintptr socketDescriptor, protocol::RequestTypeMessages typeMsg,
			const QDateTime timePoint) override final
		{
			qDebug() << timePoint.toString() 
				<< "server statistics query completed for socket: " << socketDescriptor;
		}

	}; /// class GetStat

	class SendData : public LogHandler
	{
	public:
		SendData() = default;
		virtual ~SendData() = default;

	private:
		protocol::RequestTypeMessages canHandler() const override final {
			return protocol::RequestTypeMessages::SendData;
		}
		void handle(qintptr socketDescriptor, protocol::RequestTypeMessages typeMsg,
			const QDateTime timePoint) override final
		{
			qDebug() << timePoint.toString()
				<< "received data from socket: " << socketDescriptor << "processed";
		}

	}; /// class SendData


	class ClearData : public LogHandler
	{
	public:
		ClearData() = default;
		virtual ~ClearData() = default;

	private:
		protocol::RequestTypeMessages canHandler() const override final {
			return protocol::RequestTypeMessages::ClearData;
		}
		void handle(qintptr socketDescriptor, protocol::RequestTypeMessages typeMsg, 
			const QDateTime timePoint) override final
		{
			qDebug() << timePoint.toString() 
				<< "request from socket: " << socketDescriptor <<  "for data cleaning is performed";
		}

	}; /// class ClearData

} /// namespace event

#endif // !HANDLERS_H_IN_MY_PROJECT

