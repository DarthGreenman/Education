/// mainwindow.cpp

#include "../common/protocol.h"
#include "mainwindow.h"
#include "tcpclient.h"
#include "ui_mainwindow.h"
#include <cstdint>
#include <memory>
#include <qabstractsocket.h>
#include <qcombobox.h>
#include <qdatetime.h>
#include <qhostaddress.h>
#include <qmainwindow.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qwidget.h>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent),
	_ui{ std::make_unique<Ui::MainWindow>() },
	_client{ std::make_unique<TcpClient>() }
{
	_ui->setupUi(this);

	setControlView();
	setControlHandling();
}
MainWindow::~MainWindow() {}

void MainWindow::setControlView()
{
	_ui->le_data->setEnabled(false);
	_ui->pb_request->setEnabled(false);
	_ui->lb_connectStatus->setText("Отключено");
	_ui->lb_connectStatus->setStyleSheet("color: red");

	QObject::connect(_client.get(), &TcpClient::connected, this, [&]
		{
			_ui->lb_connectStatus->setText("Подключено");
			_ui->lb_connectStatus->setStyleSheet("color: green");
			_ui->pb_connect->setText("Отключиться");
			_ui->pb_request->setEnabled(true);
			_ui->spB_port->setEnabled(false);
			_ui->spB_ip1->setEnabled(false);
			_ui->spB_ip2->setEnabled(false);
			_ui->spB_ip3->setEnabled(false);
			_ui->spB_ip4->setEnabled(false);

		});

	QObject::connect(_client.get(), &TcpClient::disconnected, this, [&]
		{
			_ui->lb_connectStatus->setText("Отключено");
			_ui->lb_connectStatus->setStyleSheet("color: red");
			_ui->pb_connect->setText("Подключиться");
			_ui->pb_request->setEnabled(false);
			_ui->spB_port->setEnabled(true);
			_ui->spB_ip1->setEnabled(true);
			_ui->spB_ip2->setEnabled(true);
			_ui->spB_ip3->setEnabled(true);
			_ui->spB_ip4->setEnabled(true);
			_ui->le_data->setEnabled(false);
		});

	QObject::connect(_ui->cb_request, &QComboBox::currentIndexChanged, this, [&](int index)
		{
			const auto typeMsg = static_cast<protocol::RequestTypeMessages>(_ui->cb_request->currentIndex());
			typeMsg == protocol::RequestTypeMessages::SendData ? _ui->le_data->setEnabled(true) :
				_ui->le_data->setEnabled(false);
		});

	QObject::connect(_client.get(), &TcpClient::sendData, this, [&] {_ui->le_data->clear(); });
}

void MainWindow::setControlHandling()
{
	QObject::connect(_ui->pb_connect, &QPushButton::clicked, this, &MainWindow::connect);
	QObject::connect(_ui->pb_request, &QPushButton::clicked, this, &MainWindow::request);

	QObject::connect(_client.get(), &TcpClient::sendTime, this, &MainWindow::viewTime);
	QObject::connect(_client.get(), &TcpClient::sendFreeSpace, this, &MainWindow::viewFreeSpace);
	QObject::connect(_client.get(), &TcpClient::sendStat, this, &MainWindow::viewStat);
	QObject::connect(_client.get(), &TcpClient::sendData, this, &MainWindow::viewData);
	QObject::connect(_client.get(), &TcpClient::clearData, this, &MainWindow::viewFreeSpace);
	QObject::connect(_client.get(), &TcpClient::connectStatus,
		this, &MainWindow::viewInfoStatus);
}

void MainWindow::connect()
{
	if (_client->state() != QAbstractSocket::ConnectedState) {
		const QString ip{ _ui->spB_ip4->text() + "." +
			_ui->spB_ip3->text() + "." +
			_ui->spB_ip2->text() + "." +
			_ui->spB_ip1->text() };
		const auto port = _ui->spB_port->value();
		_client->connectToHost(QHostAddress{ ip }, port);
	}
	else {
		_client->disconnectFromHost();
	}
}

void MainWindow::request()
{
	using namespace protocol;
	ServiceHeader head{};
	head.typeMsg = static_cast<RequestTypeMessages>(_ui->cb_request->currentIndex());

	if (const auto data = _ui->le_data->text(); data.isEmpty())
		_client->sendRequest(head);
	else
		_client->sendRequest(head, data);
}

void MainWindow::viewTime(QDateTime time) const {
	_ui->tb_result->append("\nТекущие время и дата на сервере:\n" + time.toString());
}

void MainWindow::viewFreeSpace(uint64_t capacity) const {
	_ui->tb_result->append("\nСвободное пространство на сервере:\n" + QString::number(capacity) + " байт");
}

void MainWindow::viewStat(protocol::StatServer stat) const
{
	_ui->tb_result->append("\nПринято байт: " + QString::number(stat.bytesReceived));
	_ui->tb_result->append("Передано байт: " + QString::number(stat.bytesTransferred));
	_ui->tb_result->append("Принято пакетов: " + QString::number(stat.packetsReceived));
	_ui->tb_result->append("Передано пакетов: " + QString::number(stat.packetsTransferred));
	_ui->tb_result->append("Время работы сервера: " + QString::number(stat.serverUpTime) + QString{ " секунд" });
	_ui->tb_result->append("Количество подключенных клиентов: " + QString::number(stat.connectedClients));
}

void MainWindow::viewData(QString data, protocol::StatusMessages status) const
{
	if (status != protocol::StatusMessages::Succes)
		viewInfoStatus(status);
	_ui->tb_result->append("\nПереданные данные: " + data);
}

void MainWindow::viewInfoStatus(protocol::StatusMessages status) const
{
	using namespace protocol;
	switch (status)
	{
	case StatusMessages::Succes:
		_ui->tb_result->append("Соединение с портом: "
			+ QString::number(_ui->spB_port->value()) + " установлено.");
		break;
	case StatusMessages::NoFreeSpace:
		_ui->tb_result->append("На сервере недостаточно свободного места.");
		break;
	case StatusMessages::NoConnectToHost:
		_ui->tb_result->append("Ошибка подключения к порту: "
			+ QString::number(_ui->spB_port->value()));
		break;
	case StatusMessages::NoImplFunc:
		_ui->tb_result->append("Не реализован функционал.");
		break;
	}
}