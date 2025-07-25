/// db_connect.cpp

#include "db_connect.h"
#include "ui_db_connect.h"
#include <memory>
#include <qdialog.h>
#include <qdialogbuttonbox.h>
#include <qobject.h>
#include <qtmetamacros.h>
#include <qwidget.h>
#include <stdexcept>
#include <string>

namespace mydb
{
	DbConnectParams::DbConnectParams(QWidget* parent)
		try : QDialog(parent), _ui{ std::make_unique<Ui::DbConnectParamsCtrl>() }
	{
		_ui->setupUi(this);
		connectionSetup(); // Настройка параметров подключения к базе данных.

		QObject::connect(_ui->buttonBox, &QDialogButtonBox::accepted, this,
			&DbConnectParams::onButtonBoxAccepted);
	}
	catch (...)
	{
		throw std::runtime_error{ "\nRuntime error: " + std::string{__FUNCTION__} +
			"\nfile: " + std::string{ __FILE__ } + "\nline: " + std::to_string(__LINE__) };
	}

	DbConnectParams::~DbConnectParams()
	{
	}

	void DbConnectParams::connectionSetup(const char* host, const char* dbName, 
		const char* login, const char* password, int port)
	{
		_ui->le_host->setText(host);
		_ui->le_dbName->setText(dbName);
		_ui->le_login->setText(login);
		_ui->le_pass->setText(password);
		_ui->spB_port->setValue(port);
	}

	void DbConnectParams::onButtonBoxAccepted()
	{
		_connParam.host = _ui->le_host->text();
		_connParam.dbName = _ui->le_dbName->text();
		_connParam.login = _ui->le_login->text();
		_connParam.pass = _ui->le_pass->text();
		_connParam.port = _ui->spB_port->value();

		emit sigSendParam(_connParam);
	}
} // namespace mydb