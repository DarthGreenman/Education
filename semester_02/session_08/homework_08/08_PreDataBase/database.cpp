/// database.cpp

#include "database.h"
#include "db_connect.h"

#include <memory>
#include <qobject.h>
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qsqlquery.h>
#include <qsqlquerymodel.h>
#include <qsqltablemodel.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <stdexcept>
#include <string>

namespace mydb
{
	Database::Database(const char* dbDriver, const char* dbName, QObject* parent)
		try : QObject{ parent }, _db{ std::make_unique<QSqlDatabase>() }
	{
		*_db = QSqlDatabase::addDatabase(dbDriver, dbName);
	}
	catch (...)
	{
		throw std::runtime_error{ "\nRuntime error: " + std::string{__FUNCTION__} +
			"\nfile: " + std::string{ __FILE__ } + "\nline: " + std::to_string(__LINE__) };
	}

	void Database::connectToDatabase(const ConnectParamFields& connParam)
	{
		_db->setHostName(connParam.host);
		_db->setDatabaseName(connParam.dbName);
		_db->setUserName(connParam.login);
		_db->setPassword(connParam.pass);
		_db->setPort(connParam.port);

		emit sendStatusConnection(_db->open());
	}

	void Database::disconnectFromDatabase(const char* dbName)
	{
		*_db = QSqlDatabase::database(dbName);
		_db->close();
	}

	void Database::prepareQuery(QSqlQuery* query, movieGenre genre)
	{
		static QString text{ "SELECT title, description FROM film "
			"JOIN film_category ON film.film_id = film_category.film_id "
			"JOIN category ON category.category_id = film_category.category_id "
			"WHERE category.name = :genre" };

		query->prepare(text);
		genre == movieGenre::comedy ? query->bindValue(":genre", "Comedy") :
			query->bindValue(":genre", "Horror");
	}

	QSqlError Database::executeQuery(movieGenre genre)
	{
		QSqlQuery query{ *_db };
		prepareQuery(&query, genre);
		if (query.exec())
		{
			_query = std::make_unique<QSqlQueryModel>(nullptr);
			_query->setQuery(query);
			emit sendSqlData(_query.get(), _header);
			return QSqlError{};
		}
		return _query->lastError();
	}

	QSqlError Database::executeQuery(const char* tableName)
	{
		_table = std::make_unique<QSqlTableModel>(nullptr, *_db);
		_table->setTable(tableName);
		if (_table->select())
		{
			emit sendTableData(_table.get(), _header);
			return QSqlError{};
		}
		return _table->lastError();
	}
} // namespace mydb
