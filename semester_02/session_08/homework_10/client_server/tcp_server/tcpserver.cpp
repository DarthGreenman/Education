/// tcpserver.cpp

#include "../common/inout.h"
#include "../common/protocol.h"
#include "tcpserver.h"
#include <cstdint>
#include <memory>
#include <qbytearray.h>
#include <qdatastream.h>
#include <qdatetime.h>
#include <qhostaddress.h>
#include <qiodevice.h>
#include <qlogging.h>
#include <qobject.h>
#include <qstring.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <qtimer.h>

TcpServer::TcpServer(QObject* parent, uint64_t capacity, uint16_t port) : _server{ std::make_unique<QTcpServer>() },
_data{ std::make_unique<QByteArray>(capacity,'\0') }, _timer{ std::make_unique<QTimer>() },
_capacity{ capacity }
{
	if (_server->listen(QHostAddress::Any, port)) {
		_timer->start(1000);
		qDebug() << "Server started.";
		qDebug() << "Time: " << QDateTime::currentDateTime();
		qDebug() << "Port: " << port;
	}
	else {
		qDebug() << "Error start server!";
	}

	QObject::connect(_timer.get(), &QTimer::timeout,
		this, [&] { ++_state.serverUpTime; });

	QObject::connect(_server.get(), &QTcpServer::newConnection, this, [&]
		{
			QTcpSocket* newSocket = _server->nextPendingConnection();
			_sockets.insert(newSocket, newSocket->socketDescriptor());
			++_state.connectedClients;
			++_state.packetsReceived;

			QObject::connect(newSocket, &QTcpSocket::readyRead, this, &TcpServer::readyRead);
			QObject::connect(newSocket, &QTcpSocket::disconnected, this, [&]
				{
					QTcpSocket* curSocket = (QTcpSocket*)sender();
					qDebug() << "Socket " << _sockets.find(curSocket).value() << " disconnected!";
					curSocket->deleteLater();
					_sockets.erase(_sockets.find(curSocket));
					--_state.connectedClients;
				});
			qDebug() << "Socket " << newSocket->socketDescriptor() << " connected!";
		});
}

void TcpServer::processingMessage(QDataStream& data, const protocol::ServiceHeader& header, QTcpSocket* socket)
{
	_data->clear();
	QDataStream ou(_data.get(), QIODevice::WriteOnly);

	const auto& [idHead, typeMsg, statusMsg, sizeMsg] = header;
	using namespace protocol;
	ServiceHeader head{ idHead, typeMsg, statusMsg, sizeMsg };
	switch (typeMsg)
	{
	case RequestTypeMessages::GetTime:
		head.sizeMsg = sizeof(QDateTime);
		ou << head;
		ou << QDateTime::currentDateTime();
		break;
	case RequestTypeMessages::GetFreeSpace:
		head.sizeMsg = sizeof(freeSpace());
		ou << head;
		ou << freeSpace();
		break;
	case RequestTypeMessages::GetStat:
		head.sizeMsg = sizeof(_state);
		ou << head;
		ou << _state;
		break;
	case RequestTypeMessages::SendData:
		if (freeSpace() < sizeMsg)
		{
			head.statusMsg = StatusMessages::NoFreeSpace;
			head.sizeMsg = 0;
			ou << head;
		}
		else
		{
			QString str{};
			data >> str;
			ou << head;
			ou << str;
		}
		break;
	case RequestTypeMessages::ClearData:
		_data->clear();
		head.sizeMsg = sizeof(freeSpace());
		ou << head;
		ou << freeSpace();
		break;
	default:
		return;
	}

	socket->write(*_data);
	++_state.packetsTransferred;
	_state.bytesTransferred += sizeof(ServiceHeader) + head.sizeMsg;
}

void TcpServer::readyRead()
{
	QTcpSocket* socket = (QTcpSocket*)sender();
	QDataStream ou(socket);

	if (ou.status() != QDataStream::Ok)
	{
		qDebug() << "Error open stream for client " << socket->socketDescriptor();
		return;
	}
	qDebug() << "Incoming data from client " << socket->socketDescriptor();
	++_state.packetsReceived;

	for (protocol::ServiceHeader head{}; socket->bytesAvailable();)
	{
		if (_queue.find(socket->socketDescriptor()) != _queue.end()) {
			head = _queue.value(socket->socketDescriptor());
		}
		// Обрабатываем заголовок сообщения.
		// QAbstractSocket::bytesAvailable() const возвращает количество байтов, доступных для чтения. 
		else {
			if (const auto ava = socket->bytesAvailable(); ava < sizeof(protocol::ServiceHeader)) {
				return;
			}
			else {
				ou >> head;
				_state.bytesReceived += sizeof(protocol::ServiceHeader);
			}
		}
		// Обрабатываем данные сообщения.
		// QAbstractSocket::bytesAvailable() const возвращает количество байтов, после чтения заголовка. 
		if (const auto ava = socket->bytesAvailable(); ava < head.sizeMsg) {
			if (_queue.find(socket->socketDescriptor()) == _queue.end())
				_queue.insert(socket->socketDescriptor(), head);
			return;
		}
		else {
			processingMessage(ou, head, socket);
		}
	}
}