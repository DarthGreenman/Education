/// airport_inspector_driver.cpp

#include "airport_inspector_driver.h"
#include <memory>
#include <qobject.h>
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qstring.h>
#include <qtmetamacros.h>

namespace driver
{
	AirportInspectorDriver::AirportInspectorDriver(QObject* parent, const ConnectionParameters& connectParams)
		: QObject(parent), _db{ std::make_unique<QSqlDatabase>() }
	{
		*_db = QSqlDatabase::addDatabase("QPSQL", connectParams.name);
		setParams(connectParams);
	}

	AirportInspectorDriver::~AirportInspectorDriver() { _db->close(); }

	void AirportInspectorDriver::connect() 
	{ 
		_db->open();
		checkConnectionStatus();
	}

	void AirportInspectorDriver::checkConnectionStatus()
	{
		if (const auto err = _db->lastError(); err.isValid())
		{
			emit connectionStatus(QString{ tr("Error connecting to database %1\n%2").
				arg(err.databaseText()).arg(err.driverText()) });
			emit connectionStatus(ConnectionStatus::disconnected);
		}
		else
			emit connectionStatus(ConnectionStatus::connected);
	}

	void AirportInspectorDriver::setParams(const ConnectionParameters& connectParams)
	{
		_db->setDatabaseName(connectParams.name);
		_db->setHostName(connectParams.host);
		_db->setUserName(connectParams.user);
		_db->setPassword(connectParams.pass);
		_db->setPort(connectParams.port);
	}

} /// namespace air