/// stopwatch_window.cpp

#include "stopwatch.h"
#include "stopwatch_window.h"
#include <chrono>
#include <memory>
#include <qboxlayout.h>
#include <qdatetime.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlistwidget.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qwidget.h>
#include <stdexcept>
#include <string>
#include <utility>

namespace watch
{
	using namespace std::chrono_literals;
	stopwatch_window::stopwatch_window(QWidget* parent)
		try : QWidget(parent),
		_start{ new QPushButton{"Старт"} }, _circle{ new QPushButton{"Круг"} }, _reset{ new QPushButton{"Сброс"} },
		_time_view{ new QLabel{QTime{ 0,0 }.toString(Qt::ISODateWithMs)} }, _time_list_view{ new QListWidget{} },
		_timer{ std::make_unique<stopwatch>(100ms) }
	{
		QWidget::setWindowTitle("Секундомер");
		QWidget::setFixedWidth(300);
		QWidget::setLayout(make_environment());

		handler();
		QObject::connect(_timer.get(), &stopwatch::update, this, &stopwatch_window::update_time);
	}
	catch (...)
	{
		throw std::runtime_error{ "\nRuntime error: " + std::string{__FUNCTION__} +
			"\nfile: " + std::string{ __FILE__ } + "\nline: " + std::to_string(__LINE__) };
	}
	void stopwatch_window::update_time(std::chrono::milliseconds time_point)
	{
		_time_view->setText(stopwatch::format_to_view(time_point).toString(Qt::ISODateWithMs));
	}
	QLayout* stopwatch_window::make_environment()
	{
		auto box = new QVBoxLayout{};
		box->addWidget(_time_view); 
		box->addWidget(_time_list_view);
		box->addLayout(make_control_area());

		return box;
	}
	QLayout* stopwatch_window::make_control_area()
	{
		_start->setCheckable(true);
		_circle->setDisabled(true);

		auto box = new QHBoxLayout{};
		box->addWidget(_start); 
		box->addWidget(_circle); 
		box->addWidget(_reset);

		return box;
	}
	/// <summary>
	/// Обработчик сигналов элементов управления
	/// </summary>
	void stopwatch_window::handler()
	{
		QObject::connect(_start, &QPushButton::toggled, this,[&]
			{
				const auto is_checked = _start->isChecked();
				_start->setText(is_checked ? "Стоп" : "Старт");
				_circle->setEnabled(is_checked ? true : false);
				is_checked ? _timer->start() : _timer->stop();
			}
		);

		QObject::connect(_circle, &QPushButton::clicked, this, [&]
			{
				const auto& [circle, duration] = _timer->add_circle();
				QString value{ "Круг: " + QString::number(circle) + " время: " + 
					stopwatch::format_to_view(duration).toString(Qt::ISODateWithMs) };
				_time_list_view->addItem(std::move(value));
			}
		);

		QObject::connect(_reset, &QPushButton::clicked, this, [&]
			{
				_timer->reset();
				_time_view->setText(QTime{ 0,0 }.toString(Qt::ISODateWithMs));
				_time_list_view->clear();
				_start->setChecked(false);
			}
		);
	}
} /// namespace watch