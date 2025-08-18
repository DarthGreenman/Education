/// airport_inspector_driver.h

#ifndef AIRPORT_INSPECTOR_DRIVER_IN_PROJECT_AIRPORT_INSPECTOR
#define AIRPORT_INSPECTOR_DRIVER_IN_PROJECT_AIRPORT_INSPECTOR

#include <memory>
#include <qobject.h>
#include <qsqldatabase.h>
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
		enum class ConnectionStatus { connected, disconnected };
		AirportInspectorDriver(QObject* parent, const ConnectionParameters& connectParams);
		~AirportInspectorDriver();

		void connect();
		void checkConnectionStatus();
	private:
		void setParams(const ConnectionParameters& connectParams);

	private:
		std::unique_ptr<QSqlDatabase> _db{};

	signals:
		void connectionStatus(ConnectionStatus);
		void connectionStatus(QString);

	}; /// class AirportInspectorDriver
} /// namespace driver

#endif /// !AIRPORT_INSPECTOR_DRIVER_IN_PROJECT_AIRPORT_INSPECTOR
