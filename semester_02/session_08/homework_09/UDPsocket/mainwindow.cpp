/// mainwindow.cpp

#include "mainwindow.h"
#include "udpworker.h"
#include "ui_mainwindow.h"
#include <chrono>
#include <memory>
#include <qabstractsocket.h>
#include <qbytearray.h>
#include <qdatastream.h>
#include <qdatetime.h>
#include <qhostaddress.h>
#include <qiodevice.h>
#include <qmainwindow.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qtextedit.h>
#include <qtimer.h>
#include <qtypes.h>
#include <qwidget.h>

MainWindow::MainWindow(QWidget* parent, const QHostAddress& host, unsigned short port,
	std::chrono::milliseconds timeout)
	: QMainWindow(parent),
	_ui{ std::make_unique<Ui::MainWindow>() },
	_udp{ std::make_unique<udp::UdpWorker>(nullptr, host, port) },
	_timer{ std::make_unique<QTimer>(nullptr) }
{
	_ui->setupUi(this);
	_timer->setInterval(timeout);

	setControlView();
	setControlHandling();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setControlView()
{
	_ui->pb_send->setDisabled(true);
	_ui->pb_stop->setDisabled(true);

	QObject::connect(_ui->te_msg, &QTextEdit::textChanged,
		this, [&] {
			_ui->pb_send->setEnabled(true);
			_ui->pb_start->setDisabled(true);
			_ui->pb_stop->setDisabled(true);
		});

	QObject::connect(_ui->pb_send, &QPushButton::clicked,
		this, [&] {
			_ui->pb_send->setDisabled(true);
			_ui->pb_start->setEnabled(true);
		});

	QObject::connect(_ui->pb_start, &QPushButton::clicked,
		this, [&] {
			_ui->pb_stop->setEnabled(true);
		});

	QObject::connect(_ui->pb_stop, &QPushButton::clicked,
		this, [&] {
			_ui->pb_start->setEnabled(true);
			_ui->pb_stop->setDisabled(true);
		});

	QObject::connect(_timer.get(), &QTimer::timeout,
		this, [&] {
			_ui->pb_start->setDisabled(true);
		});
}

void MainWindow::setControlHandling()
{
	QObject::connect(_ui->pb_send, &QPushButton::clicked,
		this, [&] {
			sendDatagram(_ui->te_msg->toPlainText());
			_typeMsg = MessageType::text;
		});

	// Получаем данные. 
	QObject::connect(_udp.get(), &udp::UdpWorker::sendData,
		this, &MainWindow::viewData);

	// Запускаем таймер.
	QObject::connect(_ui->pb_start, &QPushButton::clicked,
		this, [&] { _timer->start(); });

	// Через заданный timeout передаем датаграмму (текущее время) в экземпляр udp::UDPworker.
	QObject::connect(_timer.get(), &QTimer::timeout,
		this, [&] {
			sendDatagram(QTime::currentTime().toString());
			_typeMsg = MessageType::time;
		});

	// Останавливаем таймер.
	QObject::connect(_ui->pb_stop, &QPushButton::clicked,
		this, [&] { _timer->stop(); });
}

void MainWindow::sendDatagram(const QString& msg)
{
	QByteArray data{};
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << msg; // Сериализация текущих данных.
	if (!_udp->writeDatagram(data)) {
		handleError(_udp->error());
	}
}

void MainWindow::handleError(QAbstractSocket::SocketError error) const
{
	; // Код обработки ошибки.
}

void MainWindow::viewData(QString msg, QHostAddress senderAddress, qsizetype size)
{
	switch (_typeMsg)
	{
	case MainWindow::MessageType::text:
		_ui->te_result->append("Сообщение:\n" + msg
			+ "\nпринято от: " + senderAddress.toString()
			+ ", размер сообщения: " + QString::number(size));
		break;
	case MainWindow::MessageType::time:
		static qsizetype counter{};
		_ui->te_result->append("Текущее время: " + msg + ". "
			"Принято пакетов " + QString::number(++counter));
		break;
	}
}