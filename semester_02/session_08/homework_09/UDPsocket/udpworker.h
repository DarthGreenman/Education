/// udpworker.h

#ifndef UDPWORKER_H_IN_MY_PROJECT
#define UDPWORKER_H_IN_MY_PROJECT

#include <memory>
#include <qabstractsocket.h>
#include <qbytearray.h>
#include <qhostaddress.h>
#include <qnetworkdatagram.h>
#include <qobject.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qtypes.h>
#include <qudpsocket.h>

namespace udp
{
	class UdpWorker : public QObject
	{
		Q_OBJECT
	public:
		explicit UdpWorker(QObject* parent, const QHostAddress& host, unsigned short port);
		~UdpWorker() = default;

		bool writeDatagram(QByteArray data);
		auto error() const { return _socket->error(); }

	private:
		void processingDatagram(const QNetworkDatagram& datagram);

	private slots:
		void readDatagram();

	private:
		std::unique_ptr<QUdpSocket> _socket{};
		QHostAddress _host{};
		unsigned short _port{};

	signals:
		void sendData(QString msg, QHostAddress senderAddress, qsizetype size);

	}; /// class UdpWorker
} /// namespace udp
#endif /// UDPWORKER_H_IN_MY_PROJECT
