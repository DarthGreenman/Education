#include "widgets.h"

#include <qapplication.h>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <qcontainerfwd.h>
#include <qdockwidget.h>
#include <qgridlayout.h>
#include <qgroupbox.h>
#include <qmainwindow.h>
#include <qnamespace.h>
#include <qprogressbar.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qscreen.h>
#include <qsize.h>
#include <qstatusbar.h>
#include <qwidget.h>

widgets::widgets(QWidget* parent)
	: QMainWindow(parent), _push_button{ new QPushButton{"&Ok"} }, _progress_bar{ new QProgressBar{} }
{
	setWindowTitle("Homework for the lesson \"Widgets\"");
	setFixedSize(400, 150);

	const auto screen = QApplication::primaryScreen();
	const auto rect = screen->availableGeometry();
	move((rect.width() - frameSize().width()) / 2, (rect.height() - frameSize().height()) / 2);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto dock = new QDockWidget{ this };
	dock->setFeatures(QDockWidget::DockWidgetFeature::NoDockWidgetFeatures);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, dock);
	dock->setWidget(make_main_widget(dock));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	_progress_bar->setRange(0, 100);
	_progress_bar->setValue(0);
	statusBar()->addWidget(_progress_bar);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	connect(_push_button, &QPushButton::clicked, this,
		[&]()
		{
			const auto value = _progress_bar->value();
			_progress_bar->setValue(value < _progress_bar->maximum() ? value + 10 : 0);
		}
	);
}

widgets::~widgets()
{
}

QGroupBox* widgets::make_group_box(QWidget* parent) const
{
	const auto group = new QGroupBox{ "Calculation error level", parent };
	auto box = new QHBoxLayout{ group };
	auto lower = new QRadioButton{ "&Lower" }, high = new QRadioButton{ "&High" };

	lower->setChecked(true);
	box->addWidget(lower); box->addWidget(high);
	box->addStretch(1);

	return group;
}

QComboBox* widgets::make_combo_box(QWidget* parent) const
{
	auto combo = new QComboBox{ parent };
	combo->setMaxCount(10);
	combo->addItems(QStringList{ "Trace", "Debug", "Info", "Warm", "Error", "Fatal" });

	return combo;
}

QWidget* widgets::make_main_widget(QWidget* parent)
{
	const auto widget = new QWidget{ parent };
	auto grid = new QGridLayout{ widget };
	grid->addWidget(make_group_box(widget), 0, 0);
	grid->addWidget(make_combo_box(widget), 1, 0);
	grid->addWidget(_push_button, 1, 1);

	return widget;
}
