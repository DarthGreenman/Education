/// airport_inspector_viewer.cpp

#include "airport_inspector_driver.h"
#include "airport_inspector_viewer.h"
#include "ui_airport_inspector_viewer.h"
#include <chrono>
#include <memory>
#include <qabstractitemview.h>
#include <qlogging.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qoverload.h>
#include <qsqlquerymodel.h>
#include <qstatusbar.h>
#include <qstring.h>
#include <qwidget.h>
#include <thread>

AirportInspectorViewer::AirportInspectorViewer(QWidget* parent, const driver::ConnectionParameters& connectParams)
	: QMainWindow(parent),
	_ui{ std::make_unique<Ui::AirportInspectorViewerClass>() },
	_db{ std::make_unique<driver::AirportInspectorDriver>(nullptr, connectParams) }
{
	_ui->setupUi(this);

	setupEnvironment();
	setupControlView();
	setupControlHandling();
	_db->connect();
}

AirportInspectorViewer::~AirportInspectorViewer() {}

void AirportInspectorViewer::setupEnvironment()
{
	QMainWindow::setCentralWidget(_ui->centralWidget);
	QMainWindow::setMinimumSize(800, 600);

	_ui->airports->setAlternatingRowColors(true);
	_ui->airports->setSelectionBehavior(QAbstractItemView::SelectRows);
	_ui->airports->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	_ui->airports->setEditTriggers(QAbstractItemView::NoEditTriggers);

	_ui->flights->setAlternatingRowColors(true);
	_ui->flights->setSelectionBehavior(QAbstractItemView::SelectRows);
	_ui->flights->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	_ui->flights->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void AirportInspectorViewer::setupControlView()
{
	QObject::connect(_db.get(), &driver::AirportInspectorDriver::connected,
		this, [&] { QMainWindow::statusBar()->showMessage(tr("Подключено")); });

	QObject::connect(_db.get(), &driver::AirportInspectorDriver::connectionError, 
		this, [&] {	QMainWindow::statusBar()->showMessage(tr("Отключено")); });

	QObject::connect(_db.get(), &driver::AirportInspectorDriver::connectionError,
		this, [&](const QString& msg)
		{
			auto msgBox = std::make_unique<QMessageBox>(QMessageBox::Icon::Critical,
				tr("Ошибка подключения к базе данных"), msg, QMessageBox::StandardButton::Ok |
				QMessageBox::StandardButton::Cancel);
			msgBox->setInformativeText("Подключить");

			if (const auto button = msgBox->exec(); button == QMessageBox::StandardButton::Ok) {
				std::this_thread::sleep_for(std::chrono::seconds{ 5 });
				_db->connect();
			};
		});

	QObject::connect(_db.get(), &driver::AirportInspectorDriver::statementError,
		this, [&](QString msg) { qDebug() << msg; });
}

void AirportInspectorViewer::setupControlHandling()
{
	QObject::connect(_db.get(), &driver::AirportInspectorDriver::connected, this, [&]
		{
			_db->executeQuery("SELECT airport_code, airport_name->>'en', city->>'ru' FROM bookings.airports_data "
				"ORDER BY airport_code;",
				driver::AirportInspectorDriver::Airports{ { "Код","Наименование","Расположение" } });
		});
	QObject::connect(_db.get(), &driver::AirportInspectorDriver::connected, this, [&]
		{
			_db->executeQuery("SELECT "
				"flight_no, scheduled_departure, scheduled_arrival, departure_airport, arrival_airport, status, aircraft_code "
				"FROM bookings.flights "
				"ORDER BY scheduled_departure;",
				driver::AirportInspectorDriver::Flights{ { "Код","Наименование","Расположение" } });
		});
	using namespace driver;
	using Airports = AirportInspectorDriver::Airports;
	QObject::connect(_db.get(), qOverload<QSqlQueryModel*, const Airports&>(&AirportInspectorDriver::sendData),
		this, qOverload<QSqlQueryModel*, const Airports&>(&AirportInspectorViewer::receiveData));

	using Flights = AirportInspectorDriver::Flights;
	QObject::connect(_db.get(), qOverload<QSqlQueryModel*, const Flights&>(&AirportInspectorDriver::sendData),
		this, qOverload<QSqlQueryModel*, const Flights&>(&AirportInspectorViewer::receiveData));
}

void AirportInspectorViewer::receiveData(QSqlQueryModel* model, 
	const driver::AirportInspectorDriver::Airports& airports)
{
	_ui->airports->setModel(model);
	_ui->airports->resizeColumnsToContents();
}

void AirportInspectorViewer::receiveData(QSqlQueryModel* model, 
	const driver::AirportInspectorDriver::Flights& flights)
{
	_ui->flights->setModel(model);
	_ui->flights->resizeColumnsToContents();
}

/// flight_no, scheduled_arrival, scheduled_departure, departure_airport, arrival_airport, status, aircraft_code

/*
* By default, the cells in a table do not expand to fill the available space.
* You can make the cells fill the available space by stretching the last header section.
* Access the relevant header using horizontalHeader() or verticalHeader() and set the header's stretchLastSection property.
* To distribute the available space according to the space requirement of each column or row, 
* call the view's resizeColumnsToContents() or resizeRowsToContents() functions.
*/