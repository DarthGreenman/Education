/// tcpserver.h

#ifndef TCPSERVER_H_IN_MY_PROJECT
#define TCPSERVER_H_IN_MY_PROJECT

#include "../common/protocol.h"
#include <cstdint>
#include <memory>
#include <qbytearray.h>
#include <qdatastream.h>
#include <qmap.h>
#include <qobject.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <qtimer.h>
#include <qtmetamacros.h>

class TcpServer : public QTcpServer
{
	Q_OBJECT
public:
	TcpServer(QObject* parent = nullptr, uint64_t capacity = 1000, uint16_t port = 12345);
	~TcpServer() = default;

private:
	void processingMessage(QDataStream& data, const protocol::ServiceHeader& header, QTcpSocket* socket);
	auto freeSpace() const { return _capacity - _data->size(); }

private slots:
	/// <summary>
	/// В цикле:
	/// Проверяем есть ли несчитанные данные для этого загаловка. Если есть, то получаем данные заголовка
	/// из очереди заголовков, и переходим к обработке данных. Если данных нет, то это новое сообщение. 
	/// Считываем данные заголовка.
	/// На этапах обработки заголовка и данных проверяем доступные данные для считывания.
	/// Если их недостаточно, то выходим из процедуры readyRead() и ожидаем следующие пакеты.
	/// </summary>
	void readyRead();

private:
	std::unique_ptr<QTcpServer> _server{};
	std::unique_ptr<QByteArray> _data{};
	std::unique_ptr<QTimer> _timer{};
	uint64_t _capacity{};

	QMap<QTcpSocket*, int> _sockets{};
	QMap<uint16_t, protocol::ServiceHeader> _queue{};
	protocol::StatServer _state{};

}; /// class TcpServer

#endif // TCPSERVER_H_IN_MY_PROJECT