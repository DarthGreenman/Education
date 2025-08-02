/// udpworker.cpp

#include "udpworker.h"
#include <memory>
#include <qabstractsocket.h>
#include <qbytearray.h>
#include <qdatastream.h>
#include <qhostaddress.h>
#include <qiodevice.h>
#include <qnetworkdatagram.h>
#include <qobject.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qudpsocket.h>

namespace udp
{
	UdpWorker::UdpWorker(QObject* parent, const QHostAddress& host, unsigned short port)
		: QObject(parent), _socket{ std::make_unique<QUdpSocket>() }, _host{ host }, _port{ port }
	{
		// Соединяем, присваиваем адрес и порт серверу.
		_socket->bind(_host, _port);
		// Соединяем функцию обработчик принятых пакетов с сокетом.
		QObject::connect(_socket.get(), &QUdpSocket::readyRead,
			this, &UdpWorker::readDatagram);
	}

	// Метод осуществляет передачу датаграммы.
	bool UdpWorker::writeDatagram(QByteArray data)
	{
		// Записываем байты в сокет (QUdpSocket)
		if (_socket->writeDatagram(data, _host, _port) == -1)
			return false;
		return true;
	}

	// Метод осуществляет обработку принятой датаграммы.
	void UdpWorker::processingDatagram(const QNetworkDatagram& datagram)
	{
		const auto data = datagram.data();
		QDataStream stream{ data }; // Создаем поток данных, доступный только для чтения.
		QString msg{};
		stream >> msg;
		emit sendData(msg, datagram.senderAddress(), data.size());
	}

	// Метод осуществляет чтение датаграм из сокета.
	void UdpWorker::readDatagram()
	{
		while (_socket->hasPendingDatagrams()) {
			const auto datagram = _socket->receiveDatagram(); // Получаем датаграмму.
			processingDatagram(datagram); // Передаем на обработку.
		}
	}
} /// namespace udp