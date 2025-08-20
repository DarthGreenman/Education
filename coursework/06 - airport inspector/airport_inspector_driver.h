/// airport_inspector_driver.h

#ifndef AIRPORT_INSPECTOR_DRIVER_IN_PROJECT_AIRPORT_INSPECTOR
#define AIRPORT_INSPECTOR_DRIVER_IN_PROJECT_AIRPORT_INSPECTOR

#include <memory>
#include <qcontainerfwd.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qsqldatabase.h>
#include <qsqlerror.h>
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
		struct Airports { QStringList header{}; };
		struct Flights { QStringList header{}; };

		AirportInspectorDriver(QObject* parent, const ConnectionParameters& connectParams);
		~AirportInspectorDriver();

		void connect();
		template<typename T>
		void executeQuery(const QString& query, T&& value);

	private:
		void setParams(const ConnectionParameters& connectParams);
		void errorHandling(const QSqlError& error);

	private:
		std::unique_ptr<QSqlDatabase> _db{};

	signals:
		void connected();

		void connectionError(const QString&);
		void statementError(const QString&);
		void transactionError(const QString&);
		
		void sendData(QSqlQueryModel*, const Airports&);
		void sendData(QSqlQueryModel*, const Flights&);

	};

	template<typename T>
	inline void AirportInspectorDriver::executeQuery(const QString& query, T&& value)
	{
		QSqlQueryModel* model = new QSqlQueryModel(this);
		model->setQuery(query, *_db);
		if (const auto error = model->lastError(); error.isValid()) {
			errorHandling(error);
			return;
		}
		for (std::size_t col{}; col < value.header.size(); ++col)
			model->setHeaderData(col, Qt::Horizontal, value.header[col]);
		emit sendData(model, std::forward<T>(value));
	}

	/// class AirportInspectorDriver
} /// namespace driver

#endif /// !AIRPORT_INSPECTOR_DRIVER_IN_PROJECT_AIRPORT_INSPECTOR
