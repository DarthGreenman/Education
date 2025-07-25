/// db_connect.h

#ifndef DB_CONNECT_PARAMS_H
#define DB_CONNECT_PARAMS_H

#include <memory>
#include <qdialog.h>
#include <qstring.h>
#include <qtconfigmacros.h>
#include <qtmetamacros.h>
#include <qwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class DbConnectParamsCtrl; }
QT_END_NAMESPACE

namespace mydb
{
	struct ConnectParamFields // Структура данных соединения с базой данных.
	{
		QString host{};
		QString dbName{};
		QString login{};
		QString pass{};
		int port{};
	}; 

	class DbConnectParams : public QDialog
	{
		Q_OBJECT
	public:
		explicit DbConnectParams(QWidget* parent = nullptr);
		~DbConnectParams();

		/// <summary>
		/// Метод инициализирует элементы управления данными (по умолчанию) 
		/// для соединения с базой данных.
		/// </summary>
		/// <param name="host"></param>
		/// <param name="dbName"></param>
		/// <param name="login"></param>
		/// <param name="password"></param>
		/// <param name="port"></param>
		void connectionSetup(const char* host = "981757-ca08998.tmweb.ru", 
			const char* dbName = "netology_cpp", const char* login = "netology_usr_cpp",
			const char* password = "CppNeto3", int port = 5432);

	signals:
		void sigSendParam(const ConnectParamFields& connParam);

	private slots:
		/// <summary>
		/// Обработчик кнопки экземпляра QDialogButtonBox.
		/// </summary>
		void onButtonBoxAccepted();

	private:
		std::unique_ptr<Ui::DbConnectParamsCtrl> _ui{};
		ConnectParamFields _connParam{};

	}; // class DbConnectParams
} // namespace mydb

#endif // !DB_CONNECT_PARAMS_H
