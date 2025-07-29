/// mainwindow.cpp

#include "database.h"
#include "db_connect.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <memory>
#include <qabstractitemmodel.h>
#include <qabstractitemview.h>
#include <qaction.h>
#include <qcontainerfwd.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qoverload.h>
#include <qpushbutton.h>
#include <qsqlquerymodel.h>
#include <qsqltablemodel.h>
#include <qtableview.h>
#include <qtconcurrentrun.h>
#include <qwidget.h>
#include <stdexcept>
#include <string>


MainWindow::MainWindow(QWidget* parent)
try : QMainWindow(parent), _ui{ std::make_unique<Ui::MainWindow>() },
_db{ std::make_unique<mydb::Database>(mydb::dbDriver(), mydb::dbName()) },
_connParamDialog{ std::make_unique<mydb::DbConnectParams>() }
{
	setupUi(); // Настраиваем начальное состояние элементов управления.
	setupControl(); // Настраиваем события: сигналы и слоты. 
}
catch (...)
{
	throw std::runtime_error{ "\nRuntime error: " + std::string{__FUNCTION__} +
		"\nfile: " + std::string{ __FILE__ } + "\nline: " + std::to_string(__LINE__) };
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
	_ui->setupUi(this);
	_ui->connect->setEnabled(false);
	_ui->clear->setEnabled(false);
	_ui->query->setEnabled(false);

	_ui->view->resizeColumnsToContents();
	_ui->view->setAlternatingRowColors(true);
	_ui->view->setSelectionBehavior(QAbstractItemView::SelectRows);
	_ui->view->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	_ui->view->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::setupControl()
{
	/// Отобразим диалоговое окно "Параметры подключения к базе данных".
	QObject::connect(_ui->setConnParam, &QAction::triggered, this, [&] { _connParamDialog->show(); });

	/*********************************************************************************************/
	/// Получаем данные для подключения из диалогового окна через параметр лямбды и устанавливаем их.
	QObject::connect(_connParamDialog.get(), &mydb::DbConnectParams::sigSendParam, this,
		[&](const mydb::ConnectParamFields& conn_param) { _connParam = conn_param; });
	/// Обновляем интерфейс.
	QObject::connect(_connParamDialog.get(), &mydb::DbConnectParams::sigSendParam, this, [&]
		{
			_connParamDialog->close();
			_ui->connect->setEnabled(true);
		});

	/*********************************************************************************************/
	/// Связываем стандартный сигнал элемента управления QMenuBar-QMenu-QAction с слотом данного 
	/// класса. В момент события, испускания сигнала  QAction::triggered, приложение подключается.
	QObject::connect(_ui->connect, &QAction::triggered, this, &MainWindow::connectToDatabase);

	/*********************************************************************************************/
	/// Получаем статус подключения.
	QObject::connect(_db.get(), &mydb::Database::sendStatusConnection, this,
		&MainWindow::receiveStatusConnection);

	/*********************************************************************************************/
	/// Связываем стандартный сигнал элемента управления QPushButton с слотом данного класса.
	/// В момент события, испускания сигнала  QPushButton::clicked, приложение подключается, или 
	/// отключается.
	QObject::connect(_ui->query, &QPushButton::clicked, this, [&]
		{
			const auto genre = static_cast<mydb::movieGenre>(_ui->category->currentIndex());
			genre == mydb::movieGenre::all ? queryToDatabase("film") :
				queryToDatabase(genre);
		});

	/*********************************************************************************************/
	/// Получаем данные методами класса QSqlTableModel.
	QObject::connect(_db.get(), qOverload<QSqlTableModel*, const QStringList&>(&mydb::Database::sendData),
		this, qOverload<QSqlTableModel*, const QStringList&>(&MainWindow::receiveData));
	/// Обновляем интерфейс.
	QObject::connect(_db.get(), qOverload<QSqlTableModel*, const QStringList&>(&mydb::Database::sendData),
		this, [&]
		{
			_cleaner = model::table;
			_ui->clear->setEnabled(true);
		});

	/// Получаем данные методами класса QSqlQueryModel.
	QObject::connect(_db.get(), qOverload<QSqlQueryModel*, const QStringList&>(&mydb::Database::sendData),
		this, qOverload<QSqlQueryModel*, const QStringList&>(&MainWindow::receiveData));
	/// Обновляем интерфейс.
	QObject::connect(_db.get(), qOverload<QSqlQueryModel*, const QStringList&>(&mydb::Database::sendData),
		this, [&]
		{
			_cleaner = model::query;
			_ui->clear->setEnabled(true);
		});

	/*********************************************************************************************/
	/// Очищаем представление.
	QObject::connect(_ui->clear, &QPushButton::clicked, this, [&]
		{
			auto model = _ui->view->model();
			_cleaner == model::table ? dynamic_cast<QSqlTableModel*>(model)->clear() :
				dynamic_cast<QSqlQueryModel*>(model)->clear();
			_ui->clear->setEnabled(false);
		});
}

void MainWindow::receiveData(QSqlTableModel* table, const QStringList& header)
{
	setHeader(table, header);
	_ui->view->setModel(table); // Передаем модель в представление.
}

void MainWindow::receiveData(QSqlQueryModel* query, const QStringList& header)
{
	setHeader(query, header);
	_ui->view->setModel(query); // Передаем модель в представление.
}

void MainWindow::receiveStatusConnection(bool status)
{
	if (status)
	{
		_ui->connect->setText("Отключиться");
		_ui->statusConnect->setText("Подключено к БД");
		_ui->statusConnect->setStyleSheet("color:green");
		_ui->query->setEnabled(true);
	}
	else
	{
		_db->disconnectFromDatabase(mydb::dbName());
		_ui->statusConnect->setText("Отключено");
		QMessageBox::critical(this, "Error", _db->lastError().text());
	}
}

void MainWindow::connectToDatabase()
{
	if (!_db->isOpen())
	{
		_ui->statusConnect->setText("Подключение");

		auto conn = [&] {_db->connectToDatabase(_connParam); };
		QtConcurrent::run(conn);
	}
	else
	{
		_db->disconnectFromDatabase(mydb::dbName());
		_ui->statusConnect->setText("Отключено");
		_ui->statusConnect->setStyleSheet("color:black");
		_ui->connect->setText("Подключиться");
		_ui->query->setEnabled(false);
	}
}

void MainWindow::setHeader(QAbstractTableModel* model, const QStringList& header)
{
	// Устанавливаем наименование столбцов.
	for (std::size_t col{}; col < header.size(); ++col) {
		model->setHeaderData(col, Qt::Horizontal, header[col]);
	}
}
