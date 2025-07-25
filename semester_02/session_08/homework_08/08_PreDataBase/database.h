/// database.h

#ifndef DATABASE_H_IN_MY_PROJECT
#define DATABASE_H_IN_MY_PROJECT

#include "db_connect.h"

#include <memory>
#include <qcontainerfwd.h>
#include <qobject.h>
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qsqlquery.h>
#include <qsqltablemodel.h>
#include <qstring.h>
#include <qtmetamacros.h>

namespace mydb
{
	constexpr auto dbDriver() { return "QPSQL"; }
	constexpr auto dbName() { return "MyDB"; }
		
	enum class movieGenre // Типы запросов.
	{
		all = 0,
		comedy,
		horrors
	};

	class Database : public QObject
	{
		Q_OBJECT
	public:
		explicit Database(const char* dbDriver, const char* dbName, 
			QObject* parent = nullptr);
		~Database() = default;

		/// <summary>
		/// Подключает приложение, и передает данные для слота
		/// MainWindow::receiveStatusConnection, который обновляет 
		/// состояние элементов управления.
		/// </summary>
		/// <param name="data_for_conn"></param>
		void connectToDatabase(const ConnectParamFields& connParam);

		/// <summary>
		/// Производит отключение с именем dbName.
		/// </summary>
		/// <param name="nameDb"></param>
		void disconnectFromDatabase(const char* dbName);

	private:
		/// <summary>
		/// Создает SQL предложение с параметрами.
		/// </summary>
		/// <param name="query"></param>
		/// <param name="genre"></param>
		void prepareQuery(movieGenre genre);
	
	public:
		/// <summary>
		/// Выполняет запрос SQL.
		/// </summary>
		/// <param name="query"></param>
		/// <returns></returns>
		QSqlError executeQuery(movieGenre genre);

		/// <summary>
		/// Выполняет табличный запрос.
		/// </summary>
		/// <returns></returns>
		QSqlError executeQuery(const char* tableName);
	
		/// <summary>
		/// Возвращает последнюю ошибку при неудачном обращении к базе данных.
		/// </summary>
		/// <returns></returns>
		auto lastError() const { return _db->lastError(); }

		/// <summary>
		/// Возвращает true, если соединение с базой данных в данный момент открыто,
		/// в противном случае возвращает false.
		/// </summary>
		/// <returns></returns>
		auto isOpen() const { return _db->isOpen(); }

	signals:
		void sendStatusConnection(bool status);
		void sendTableData(QSqlTableModel* model, const QStringList& header);
		void sendSqlData(QSqlQuery* query, const QStringList& header);

	private:
		std::unique_ptr<QSqlDatabase> _db{};
		std::unique_ptr<QSqlQuery> _query{};
		std::unique_ptr<QSqlTableModel> _table{};
		const QStringList _header{ QStringList() << "Название" << "Описание" };

	}; // class Database

} // namespace mydb

#endif // !DATABASE_H_IN_MY_PROJECT