/// airport_inspector_driver.cpp

#include "airport_inspector_driver.h"
#include <memory>
#include <qcontainerfwd.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qsqlquerymodel.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <utility>

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
		if (const auto error = _db->lastError(); error.isValid()) {
			errorHandling(error);
			return;
		};
		emit connected();
	}

	void AirportInspectorDriver::setParams(const ConnectionParameters& connectParams)
	{
		_db->setDatabaseName(connectParams.name);
		_db->setHostName(connectParams.host);
		_db->setUserName(connectParams.user);
		_db->setPassword(connectParams.pass);
		_db->setPort(connectParams.port);
	}

	void AirportInspectorDriver::errorHandling(const QSqlError& error)
	{
		const auto errorType = error.type();
		switch (errorType)
		{
		case QSqlError::ErrorType::ConnectionError:
			emit connectionError(QString{ tr("Connection error to database %1 %2\n%3") }.
				arg(_db->databaseName()).arg(error.databaseText()).arg(error.driverText()));
			break;
		case QSqlError::ErrorType::StatementError:
			emit statementError(QString{ tr("SQL statement syntax error %1 %2\n%3") }.
				arg(_db->databaseName()).arg(error.databaseText()).arg(error.driverText()));
			break;
		case QSqlError::ErrorType::TransactionError:
			emit transactionError(QString{ tr("Transaction failed error %1 %2\n%3") }.
				arg(_db->databaseName()).arg(error.databaseText()).arg(error.driverText()));
			break;
		case QSqlError::ErrorType::NoError:
		case QSqlError::ErrorType::UnknownError:
			break;
		}
	}
} /// namespace air