/// mainwindow.h

#ifndef MAINWINDOW_H_IN_MY_PROJECT
#define MAINWINDOW_H_IN_MY_PROJECT

#include "UdpWorker.h"
#include <chrono>
#include <memory>
#include <qabstractsocket.h>
#include <qhostaddress.h>
#include <qmainwindow.h>
#include <qstring.h>
#include <qtconfigmacros.h>
#include <qtimer.h>
#include <qtmetamacros.h>
#include <qtypes.h>
#include <qwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	enum class MessageType { text, time };

	MainWindow(QWidget* parent, const QHostAddress& host, unsigned short port,
		std::chrono::milliseconds timeout);
	~MainWindow();

private:
	void setControlView();
	void setControlHandling();
	void sendDatagram(const QString& msg);
	void handleError(QAbstractSocket::SocketError error) const;

private slots:
	void viewData(QString msg, QHostAddress senderAddress, qsizetype size);

private:
	std::unique_ptr<Ui::MainWindow> _ui{};
	std::unique_ptr<udp::UdpWorker> _udp{};
	std::unique_ptr<QTimer> _timer{};
	MessageType _typeMsg{ MessageType::text };

}; /// class MainWindow
#endif /// MAINWINDOW_H_IN_MY_PROJECT
