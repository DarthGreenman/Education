/// airport_inspector_viewer.cpp

#include "airport_inspector_driver.h"
#include "airport_inspector_viewer.h"
#include "ui_airport_inspector_viewer.h"
#include <memory>
#include <qdialogbuttonbox.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qstatusbar.h>
#include <qstring.h>
#include <qwidget.h>

AirportInspectorViewer::AirportInspectorViewer(QWidget* parent, const driver::ConnectionParameters& connectParams)
	: QMainWindow(parent),
	_ui{ std::make_unique<Ui::AirportInspectorViewerClass>() },
	_db{ std::make_unique<driver::AirportInspectorDriver>(nullptr, connectParams) }
{
	_ui->setupUi(this);

	setControlView();
	setControlHandling();
	_db->connect();
}

AirportInspectorViewer::~AirportInspectorViewer()
{
}

void AirportInspectorViewer::setControlView()
{
	using namespace driver;
	using ConnectionStatus = AirportInspectorDriver::ConnectionStatus;
	QObject::connect(_db.get(), static_cast<void(AirportInspectorDriver::*)(ConnectionStatus)>(&AirportInspectorDriver::connectionStatus),
		this, [&](ConnectionStatus status) {
			status == ConnectionStatus::connected ? QMainWindow::statusBar()->showMessage(tr("Подключено")) :
				QMainWindow::statusBar()->showMessage(tr("Отключено"));
		});
}

void AirportInspectorViewer::setControlHandling()
{
	using namespace driver;
	QObject::connect(_db.get(), static_cast<void(AirportInspectorDriver::*)(QString)>(&AirportInspectorDriver::connectionStatus),
		this, [&](QString msg)
		{
			auto msgBox = std::make_unique<QMessageBox>(QMessageBox::Icon::Warning, 
				tr("Ошибка подключения к базе данных"), msg,
				QMessageBox::StandardButton::Ok | QMessageBox::StandardButton::Cancel);
			msgBox->setInformativeText("Повторное подключение к базе данных произойдет через 5 секунд после закрытия окна");
			if (const auto button = msgBox->exec(); button == QMessageBox::StandardButton::Ok)
				_db->connect();
		});
}