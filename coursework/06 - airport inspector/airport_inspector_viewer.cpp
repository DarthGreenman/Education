/// airport_inspector_viewer.cpp

#include "airport_inspector_driver.h"
#include "airport_inspector_viewer.h"
#include "ui_airport_inspector_viewer.h"
#include <memory>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qstring.h>
#include <qwidget.h>

AirportInspectorViewer::AirportInspectorViewer(QWidget* parent, const driver::ConnectionParameters& connectParams)
	: QMainWindow(parent), _ui{ std::make_unique<Ui::AirportInspectorViewerClass>() },
	_db{ std::make_unique<driver::AirportInspectorDriver>(nullptr, connectParams) }
{
	_ui->setupUi(this);

	setControlView();
	setControlHandling();
	_db->checkConnectionStatus();
}

AirportInspectorViewer::~AirportInspectorViewer()
{
}

void AirportInspectorViewer::setControlView()
{
	QObject::connect(_db.get(), static_cast<void(driver::AirportInspectorDriver::*)(bool)>(&driver::AirportInspectorDriver::connectionStatus),
		this, [&] (bool value) { value ? _ui->connectStatus->setText(tr("Подключено")) : _ui->connectStatus->setText(tr("Отключено")); });

}

void AirportInspectorViewer::setControlHandling()
{
	QObject::connect(_db.get(), static_cast<void(driver::AirportInspectorDriver::*)(QString)>(&driver::AirportInspectorDriver::connectionStatus),
		this, [&] {
			auto msgBox = std::make_unique<QMessageBox>(QMessageBox::Icon::Warning,
				tr("Ошибка соединения"),
				tr("Не удалось подключиться к базе данных"));
			msgBox->exec();
			QObject::connect(msgBox.get(), &QMessageBox::buttonClicked, this, [&] {; });
		});
}

