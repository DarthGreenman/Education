/// mainwindow.h

#ifndef MAINWINDOW_H_IN_MY_PROJECT
#define MAINWINDOW_H_IN_MY_PROJECT

#include "../common/protocol.h"
#include "tcpclient.h"
#include <cstdint>
#include <memory>
#include <qdatetime.h>
#include <qmainwindow.h>
#include <qstring.h>
#include <qtconfigmacros.h>
#include <qtmetamacros.h>
#include <qwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/// <summary>
/// Класс MainWindow реализует главное окно приложения.
/// Обеспечивает взаимодействие пользователя с сервером через TCP-клиента.
/// Управляет элементами интерфейса, отправкой запросов и отображением полученных данных.
/// </summary>
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	/// <summary>
	/// Конструктор главного окна приложения.
	/// Инициализирует пользовательский интерфейс, клиент TCP и настраивает обработку элементов управления.
	/// </summary>
	/// <param name="parent">Родительский виджет.</param>
	explicit MainWindow(QWidget* parent = nullptr);
	
	/// <summary>
	/// Деструктор главного окна.
	/// </summary>
	~MainWindow();

private:
	/// <summary>
	/// Настраивает начальное состояние элементов управления интерфейса
	/// и подключает обработчики событий для отображения статуса соединения.
	/// </summary>
	void setControlView();
	
	/// <summary>
	/// Устанавливает обработчики событий для элементов управления.
	/// </summary>
	void setControlHandling();

private slots:
	/// <summary>
	/// Слот для подключения или отключения от сервера.
	/// </summary>
	void connect();
	
	/// <summary>
	/// Слот для формирования и отправки запроса серверу.
	/// </summary>
	void request();
	
	/// <summary>
	/// Слот для отображения полученного времени от сервера.
	/// </summary>
	/// <param name="time">Время, полученное от сервера.</param>
	void viewTime(QDateTime time) const;
	
	/// <summary>
	/// Слот для отображения свободного места на сервере.
	/// </summary>
	/// <param name="capacity"></param>
	void viewFreeSpace(uint64_t capacity) const;
	
	/// <summary>
	/// Слот для отображения статистики сервера.
	/// </summary>
	/// <param name="stat">Статистика сервера.</param>
	void viewStat(protocol::StatServer stat) const;
	
	/// <summary>
	/// Слот для отображения переданных данных на сервер.
	/// </summary>
	/// <param name="data"></param>
	/// <param name="status"></param>
	void viewData(QString data, protocol::StatusMessages status) const;
	
	/// <summary>
	/// Слот для отображения статуса соединения или служебных сообщений.
	/// </summary>
	/// <param name="status">Статус сообщения.</param>
	void viewInfoStatus(protocol::StatusMessages status) const;

private:
	/// <summary>
	/// Указатель на интерфейс главного окна.
	/// </summary>
	std::unique_ptr<Ui::MainWindow> _ui{};
	/// <summary>
	/// Указатель на TCP-клиента для взаимодействия с сервером.
	/// </summary>
	std::unique_ptr<TcpClient> _client{};
};
#endif // MAINWINDOW_H_IN_MY_PROJECT
