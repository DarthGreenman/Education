/// airport_inspector_driver.cpp

#include "airport_inspector_driver.h"
#include <memory>
#include <qobject.h>
#include <qsqldatabase.h>
#include <qtmetamacros.h>
#include <qsqlerror.h>

namespace driver
{
	AirportInspectorDriver::AirportInspectorDriver(QObject* parent, const ConnectionParameters& connectParams)
		: QObject(parent), _db{ std::make_unique<QSqlDatabase>() }
	{
		*_db = QSqlDatabase::addDatabase("QPSQL", connectParams.name);
		setParams(connectParams);
		_db->open();
	}

	AirportInspectorDriver::~AirportInspectorDriver() { _db->close(); }

	void AirportInspectorDriver::checkConnectionStatus()
	{
		if (const auto err = _db->lastError().isValid(); err)
			emit connectionStatus(false);
		else
			emit connectionStatus(true);
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