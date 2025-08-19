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
		if (!_db->open()) {
			connectionStatus();
			emit disconnected();
			return;
		};
		emit connected();
	}

	void AirportInspectorDriver::executeQuery(const QString& query, const QStringList& header, 
		QueryEntity entity)
	{
		QSqlQueryModel* model = new QSqlQueryModel(this);
		model->setQuery(query, *_db);
		if (model->lastError().isValid()) {
			connectionStatus();
			return;
		}
		for (std::size_t col{}; col < header.size(); ++col)
			model->setHeaderData(col, Qt::Horizontal, header[col]);
		emit sendData(model, entity);
	}

	void AirportInspectorDriver::setParams(const ConnectionParameters& connectParams)
	{
		_db->setDatabaseName(connectParams.name);
		_db->setHostName(connectParams.host);
		_db->setUserName(connectParams.user);
		_db->setPassword(connectParams.pass);
		_db->setPort(connectParams.port);
	}

	void AirportInspectorDriver::connectionStatus()
	{
		if (const auto err = _db->lastError(); err.isValid())
			emit sendConnectionStatus(QString{ tr("Error connecting to database %1 %2\n%3") }.
				arg(_db->databaseName()).arg(err.databaseText()).arg(err.driverText()));
		emit sendConnectionStatus(QString{ tr("Connected to database %1") }.arg(_db->databaseName()));
	}

} /// namespace air