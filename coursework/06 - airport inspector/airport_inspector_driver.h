/// airport_inspector_driver.h

#ifndef AIRPORT_INSPECTOR_DRIVER_IN_PROJECT_AIRPORT_INSPECTOR
#define AIRPORT_INSPECTOR_DRIVER_IN_PROJECT_AIRPORT_INSPECTOR

#include <memory>
#include <qcontainerfwd.h>
#include <qobject.h>
#include <qsqldatabase.h>
#include <qsqlquerymodel.h>
#include <qstring.h>
#include <qtmetamacros.h>

namespace driver
{
	struct ConnectionParameters
	{
		const char* name{};
		const char* host{};
		const char* user{};
		const char* pass{};
		int port{};
	}; /// ConnectionParameters

	class AirportInspectorDriver : public QObject
	{
		Q_OBJECT
	public:
		enum class QueryEntity { Airports, Flights };
		AirportInspectorDriver(QObject* parent, const ConnectionParameters& connectParams);
		~AirportInspectorDriver();

		void connect();
		void executeQuery(const QString& query, const QStringList& header, QueryEntity entity);

	private:
		void setParams(const ConnectionParameters& connectParams);
		void connectionStatus();

	private:
		std::unique_ptr<QSqlDatabase> _db{};

	signals:
		void disconnected();
		void connected();
		void sendConnectionStatus(QString);
		void sendData(QSqlQueryModel*, QueryEntity);

	}; /// class AirportInspectorDriver
} /// namespace driver

#endif /// !AIRPORT_INSPECTOR_DRIVER_IN_PROJECT_AIRPORT_INSPECTOR
