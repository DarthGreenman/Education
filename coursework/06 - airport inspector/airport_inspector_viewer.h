/// AirportInspectorViewer.h

#ifndef AIRPORT_INSPECTOR_VIEWER_IN_PROJECT_AIRPORT_INSPECTOR
#define AIRPORT_INSPECTOR_VIEWER_IN_PROJECT_AIRPORT_INSPECTOR

#include "airport_inspector_driver.h"

#include <memory>
#include <qmainwindow.h>
#include <qsqldatabase.h>
#include <qsqlquerymodel.h>
#include <qtconfigmacros.h>
#include <qtmetamacros.h>
#include <qwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class AirportInspectorViewerClass; };
QT_END_NAMESPACE

class AirportInspectorViewer : public QMainWindow
{
	Q_OBJECT
public:
	AirportInspectorViewer(QWidget* parent, const driver::ConnectionParameters& connectParams);
	~AirportInspectorViewer();

private:
	void setupEnvironment();
	void setupControlView();
	void setupControlHandling();
	void receiveData(QSqlQueryModel* model, const driver::AirportInspectorDriver::Airports& airports);
	void receiveData(QSqlQueryModel* model, const driver::AirportInspectorDriver::Flights& flights);

private:
	std::unique_ptr<Ui::AirportInspectorViewerClass> _ui{};
	std::unique_ptr<driver::AirportInspectorDriver> _db{};

}; /// class AirportInspectorViewer

#endif /// !AIRPORT_INSPECTOR_VIEWER_IN_PROJECT_AIRPORT_INSPECTOR