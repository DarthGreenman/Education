/// main.cpp

#include "airport_inspector_driver.h"
#include "airport_inspector_viewer.h"
#include <qapplication.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	constexpr driver::ConnectionParameters connectParams{
		"demo","981757-ca08998.tmweb.ru","netology_usr_cpp","CppNeto3",5432 }; /// 5432
	AirportInspectorViewer window{ nullptr, connectParams };

	window.show();
	return app.exec();
}
