/// airport_inspector_viewer.cpp

#include "airport_inspector_driver.h"
#include "airport_inspector_viewer.h"
#include "ui_airport_inspector_viewer.h"
#include <chrono>
#include <memory>
#include <qabstractitemview.h>
#include <qcontainerfwd.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qsqlquery.h>
#include <qsqlquerymodel.h>
#include <qstatusbar.h>
#include <qstring.h>
#include <qtmetamacros.h>
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

	_ui->airports->resizeColumnsToContents();
	_ui->airports->setAlternatingRowColors(true);
	_ui->airports->setSelectionBehavior(QAbstractItemView::SelectRows);
	_ui->airports->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	_ui->airports->setEditTriggers(QAbstractItemView::NoEditTriggers);

	_ui->flights->resizeColumnsToContents();
	_ui->flights->setAlternatingRowColors(true);
	_ui->flights->setSelectionBehavior(QAbstractItemView::SelectRows);
	_ui->flights->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	_ui->flights->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void AirportInspectorViewer::setupControlView()
{
	QObject::connect(_db.get(), &driver::AirportInspectorDriver::disconnected, this, [&] { QMainWindow::statusBar()->showMessage(tr("Отключено")); });
	QObject::connect(_db.get(), &driver::AirportInspectorDriver::connected, this, [&] { QMainWindow::statusBar()->showMessage(tr("Подключено")); });
	QObject::connect(_db.get(), &driver::AirportInspectorDriver::sendConnectionStatus, this, [&](QString msg)
		{
			auto msgBox = std::make_unique<QMessageBox>(QMessageBox::Icon::Warning,
				tr("Ошибка подключения к базе данных"), msg, QMessageBox::StandardButton::Ok | QMessageBox::StandardButton::Cancel);
			msgBox->setInformativeText("Повторное подключение к базе данных произойдет после нажатия Ok");
			if (const auto button = msgBox->exec(); button == QMessageBox::StandardButton::Ok)
			{
				std::this_thread::sleep_for(std::chrono::seconds{ 5 });
				_db->connect();
			};
		});
}

void AirportInspectorViewer::setupControlHandling()
{
	QObject::connect(_db.get(), &driver::AirportInspectorDriver::connected, this, [&]
		{
			_db->executeQuery("SELECT airport_code, airport_name->>'en', city->>'ru' FROM bookings.airports_data "
				"ORDER BY airport_code;", QStringList{ "Код","Наименование","Расположение" },
				driver::AirportInspectorDriver::QueryEntity::Airports);
		});
	QObject::connect(_db.get(), &driver::AirportInspectorDriver::connected, this, [&]
		{
			_db->executeQuery("SELECT "
				"flight_no, scheduled_departure, scheduled_arrival, departure_airport, arrival_airport, status, aircraft_code "
				"FROM bookings.flights "
				"ORDER BY scheduled_departure;", QStringList{},
				driver::AirportInspectorDriver::QueryEntity::Flights);
		});
	QObject::connect(_db.get(), &driver::AirportInspectorDriver::sendData, this, &AirportInspectorViewer::receiveData);
}

void AirportInspectorViewer::receiveData(QSqlQueryModel* model, driver::AirportInspectorDriver::QueryEntity entity)
{
	entity == driver::AirportInspectorDriver::QueryEntity::Airports ? _ui->airports->setModel(model) :
		_ui->flights->setModel(model);
}

/// flight_no, scheduled_arrival, scheduled_departure, departure_airport, arrival_airport, status, aircraft_code