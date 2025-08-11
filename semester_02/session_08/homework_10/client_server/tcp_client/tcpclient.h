/// tcpclient.h

#ifndef TCPCLIENT_H_IN_MY_PROJECT
#define TCPCLIENT_H_IN_MY_PROJECT

#include "../common/protocol.h"
#include <cstdint>
#include <memory>
#include <qabstractsocket.h>
#include <qdatastream.h>
#include <qdatetime.h>
#include <qhostaddress.h>
#include <qobject.h>
#include <qstring.h>
#include <qtcpsocket.h>
#include <qtmetamacros.h>
#include <qtypes.h>

/// <summary>
/// Класс TcpClient реализует TCP-клиента для обмена данными по протоколу, определённому в protocol.h.
/// Использует QTcpSocket для асинхронного взаимодействия с сервером.
/// Поддерживает отправку запросов, обработку входящих пакетов, 
/// а также сигналы для передачи данных и статусов соединения.
/// </summary>
class TcpClient : public QObject
{
	Q_OBJECT
public:
	/// <summary>
	/// Конструктор класса TcpClient.
	/// Инициализирует объект TCP-клиента и настраивает внутренний QTcpSocket.
	/// </summary>
	/// <param name="parent">Родительский объект QObject.</param>
	explicit TcpClient(QObject* parent = nullptr);

	/// <summary>
	/// Деструктор класса TcpClient.
	/// Освобождает ресурсы, связанные с TCP-клиентом.
	/// </summary>
	~TcpClient() = default;

	/// <summary>
	/// Отправляет запрос с указанным заголовком.
	/// Используется для передачи команд или запросов серверу без дополнительных данных.
	/// </summary>
	/// <param name="header">Заголовок служебного пакета.</param>
	void sendRequest(const protocol::ServiceHeader& header);

	/// <summary>
	/// Отправляет запрос с заголовком и дополнительными данными.
	/// Используется для передачи команд или запросов серверу с дополнительной строкой данных.
	/// </summary>
	/// <param name="header">Заголовок служебного пакета.</param>
	/// <param name="data">Данные для отправки.</param>
	void sendRequest(protocol::ServiceHeader& header, const QString& data);

	/// <summary>
	/// Устанавливает соединение с сервером по адресу и порту.
	/// </summary>
	/// <param name="host">Адрес сервера.</param>
	/// <param name="port">Порт сервера.</param>
	void connectToHost(const QHostAddress& host, quint16 port) {
		_socket->connectToHost(host, port);
	}

	/// <summary>
	/// Отключается от сервера.
	/// </summary>
	void disconnectFromHost() { _socket->disconnectFromHost(); }

	/// <summary>
	/// Возвращает текущее состояние сокета.
	/// </summary>
	/// <returns>Состояние сокета QAbstractSocket::SocketState.</returns>
	auto state() const { return _socket->state(); }

private slots:
	/// <summary>
	/// Слот, вызываемый при появлении новых данных в сокете. Обработчик сигнала готовности
	/// к чтению данных из сокета.
	/// Читает и обрабатывает входящие пакеты данных, проверяет их корректность и завершённость.
    /// </summary>
	void readyRead();

	/// <summary>
	/// Слот для обработки данных из потока.
	/// Выполняет обработку данных в зависимости от типа сообщения.
	/// </summary>
	/// <param name="stream">Поток данных для обработки.</param>
	void processingData(QDataStream& ou, const protocol::ServiceHeader& header);

private:
	/// <summary>
	/// Указатель на объект QTcpSocket для сетевого взаимодействия.
	/// </summary>
	std::unique_ptr<QTcpSocket> _socket{};

signals:
	/// <summary>
	/// Сигнал передачи времени.
	/// Вызывается при получении серверного времени от сервера.
	/// </summary>
	/// <param name="time">Объект QDateTime, содержащий полученное время.</param>
	void sendTime(QDateTime);
	
	/// <summary>
	/// Сигнал передачи свободного места на сервере.
	/// Вызывается при получении данных с сервера.
	/// </summary>
	/// <param name=""></param>
	void sendFreeSpace(uint64_t);

	/// <summary>
	/// Сигнал передачи статистики сервера.
	/// Вызывается при получении статистических данных от сервера.
	/// </summary>
	/// <param name="stat">Структура StatServer с данными о состоянии сервера.</param>
	void sendStat(protocol::StatServer);

	/// <summary>
	/// Сигнал передачи пользовательских данных.
	/// Вызывается при получении пользовательских данных от сервера.
	/// </summary>
	void sendData(QString, protocol::StatusMessages);

	/// <summary>
	/// Сигнал очистки сервера, возвращает значение свободного промтранства.
	/// Вызывается при получении значение свободного промтранства. 
	/// </summary>
	/// <param name=""></param>
	void clearData(uint64_t);

	/// <summary>
	/// Сигнал изменения статуса соединения.
	/// Вызывается при изменении состояния соединения с сервером (успех, ошибка и т.д.).
	/// </summary>
	/// <param name="status">Статус соединения из перечисления StatusMessages.</param>
	void connectStatus(protocol::StatusMessages);

	/// <summary>
	/// Сигнал успешного подключения к серверу.
	/// Вызывается при установлении соединения с сервером.
	/// </summary>
	void connected();

	/// <summary>
	/// Сигнал отключения от сервера.
	/// Вызывается при разрыве соединения с сервером.
	/// </summary>
	void disconnected();

	void sendInfoStatus(protocol::StatusMessages);
};

#endif // TCPCLIENT_H_IN_MY_PROJECT