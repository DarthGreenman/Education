/// tcpclient.cpp

#include "../common/inout.h"
#include "../common/protocol.h"
#include "tcpclient.h"
#include <cstdint>
#include <memory>
#include <qbytearray.h>
#include <qdatastream.h>
#include <qdatetime.h>
#include <qiodevice.h>
#include <qlogging.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qstring.h>
#include <qtcpsocket.h>
#include <qtmetamacros.h>

TcpClient::TcpClient(QObject* parent) : QObject(parent), _socket{ std::make_unique<QTcpSocket>() }
{
	QObject::connect(_socket.get(), &QTcpSocket::connected, this, [&] { emit connected(); });
	QObject::connect(_socket.get(), &QTcpSocket::connected, this, [&]
		{
			emit connectStatus(protocol::StatusMessages::Succes);
		});
	QObject::connect(_socket.get(), &QTcpSocket::errorOccurred, this, [&]
		{
			emit connectStatus(protocol::StatusMessages::NoConnectToHost);
		});
	QObject::connect(_socket.get(), &QTcpSocket::disconnected, this, [&] { emit disconnected(); });

	// Этот сигнал излучается один раз каждый раз, когда доступны новые данные для считывания 
	// из текущего канала считывания устройства. Он будет выдан снова только после того, 
	// как будут доступны новые данные, например, когда новая полезная нагрузка сетевых данных 
	// поступила в ваш сетевой сокет или когда к вашему устройству был добавлен новый блок данных.
	QObject::connect(_socket.get(), &QTcpSocket::readyRead,
		this, &TcpClient::readyRead);
}

void TcpClient::sendRequest(const protocol::ServiceHeader& header)
{
	QByteArray msg{};
	QDataStream in{ &msg, QIODevice::WriteOnly };
	
	in << header;
	if (_socket->write(msg) == -1)
		qDebug() << "QAbstractSocket::SocketError::" << _socket->error();
}

void TcpClient::sendRequest(protocol::ServiceHeader& header, const QString& data)
{
	QByteArray msg{};
	QDataStream in{ &msg, QIODevice::WriteOnly };

	header.sizeMsg = data.size();
	in << header;
	in << data;
	if (_socket->write(msg) == -1)
		qDebug() << "QAbstractSocket::SocketError::" << _socket->error();
}

void TcpClient::readyRead()
{
	QDataStream ou(_socket.get());

	if (ou.status() != QDataStream::Ok) {
		QMessageBox msg{ QMessageBox::Warning, "Внимание",
			"Ошибка открытия входящего потока для чтения данных!" };
		msg.exec();
	}
	// QDataStream::atEnd() возвращает значение true, если устройство ввода-вывода 
	// достигло конечного положения(конец потока или файла) или не установлено;
	// в противном случае возвращает значение false.
	for (protocol::ServiceHeader head{}; !ou.atEnd();)
	{
		// Обрабатываем заголовок сообщения.
		// QAbstractSocket::bytesAvailable() const возвращает количество байтов, доступных для чтения. 
		if (const auto ava = _socket->bytesAvailable(); ava < sizeof(protocol::ServiceHeader))
			return;
		else
			ou >> head;
		// Обрабатываем данные сообщения.
		// QAbstractSocket::bytesAvailable() const возвращает количество байтов, после чтения заголовка. 
		if (const auto ava = _socket->bytesAvailable(); ava < head.sizeMsg)
			return;
		else
			processingData(ou, head);
	}
}

void TcpClient::processingData(QDataStream& ou, const protocol::ServiceHeader& header)
{
	switch (header.typeMsg)
	{
	case protocol::RequestTypeMessages::GetTime:
	{
		QDateTime time{};
		ou >> time;
		emit sendTime(time);
		break;
	}
	case protocol::RequestTypeMessages::GetFreeSpace:
	{
		uint64_t capacity{};
		ou >> capacity;
		emit sendFreeSpace(capacity);
		break;
	}
	case protocol::RequestTypeMessages::GetStat:
	{
		protocol::StatServer stat{};
		ou >> stat;
		emit sendStat(stat);
		break;
	}
	case protocol::RequestTypeMessages::SendData:
	{
		QString data{};
		ou >> data;
		emit sendData(data, header.statusMsg);
		break;
	}
	case protocol::RequestTypeMessages::ClearData:
	{
		uint64_t capacity{};
		ou >> capacity;
		emit clearData(capacity);
		break;
	}
	default:
		return;
	}
}
