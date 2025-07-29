/// mainwindow.h

#ifndef MAINWINDOW_H_IN_MY_PROJECT
#define MAINWINDOW_H_IN_MY_PROJECT

#include "database.h"
#include "db_connect.h"
#include <memory>
#include <qabstractitemmodel.h>
#include <qcontainerfwd.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qnamespace.h>
#include <qsqlquerymodel.h>
#include <qsqltablemodel.h>
#include <qtconfigmacros.h>
#include <qtmetamacros.h>
#include <qwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	/// <summary>
	/// Метод настраивает начальное состояние элементов управления.
	/// </summary>
	void setupUi();

	/// <summary>
	/// Обработчик сигналов элементов управления.
	/// </summary>
	void setupControl();

public slots:
	/// <summary>
	/// Метод получает указатель на модель с данными, форматирует модель,
	/// и передает ее в представление.
	/// </summary>
	/// <param name="model"></param>
	/// <param name="header"></param>
	void receiveData(QSqlTableModel* table, const QStringList& header);

	/// <summary>
	/// Метод получает указатель на данные, заполняем данными модель,
	/// и передает ее в представление.
	/// </summary>
	/// <param name="query"></param>
	/// <param name="header"></param>
	void receiveData(QSqlQueryModel* query, const QStringList& header);

	/// <summary>
	/// Метод изменяет состояние элементов управления в зависимости от статуса состояния
	/// взаимодействия с базой данных. В случае неудачного открытия - выводит 
	/// информацию об ошибке.
	/// </summary>
	/// <param name="status"></param>
	void receiveStatusConnection(bool status);

private slots:
	/// <summary>
	/// В зависимости от статуса состояния взаимодействия с базой данных, 
	/// метод выполняет подключение, или отключение.
	/// </summary>
	void connectToDatabase();

private:
	/// <summary>
	/// Метод отправляет запрос, в случае неудачного выполнения - 
	/// выводит информацию об ошибке. 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="value"></param>
	template<typename T>
	void queryToDatabase(T&& value)
	{
		if (const auto error = _db->executeQuery(std::forward<T>(value)); error.isValid())
			QMessageBox::critical(this, "Error", error.text());
	}

	void setHeader(QAbstractTableModel* model, const QStringList& header);

private:
	enum class model { table, query };

	std::unique_ptr<Ui::MainWindow> _ui{};
	std::unique_ptr<mydb::Database> _db{};
	std::unique_ptr<mydb::DbConnectParams> _connParamDialog{};
	mydb::ConnectParamFields _connParam{};
	model _cleaner;

}; // class MainWindow
#endif // MAINWINDOW_H_IN_MY_PROJECT