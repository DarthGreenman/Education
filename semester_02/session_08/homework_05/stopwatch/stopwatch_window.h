/// stopwatch_window.h

#ifndef STOPWATCH_WINDOW_H
#define STOPWATCH_WINDOW_H
#include "stopwatch.h"

#include <chrono>
#include <memory>
#include <qlabel.h>
#include <qlayout.h>
#include <qlistwidget.h>
#include <qpushbutton.h>
#include <qtmetamacros.h>
#include <qwidget.h>

namespace watch
{
	class stopwatch_window : public QWidget
	{
		Q_OBJECT

	public:
		stopwatch_window(QWidget* parent = nullptr);
		~stopwatch_window() = default;

	public slots:
		void update_time(std::chrono::milliseconds time_point);
		void update_time_list(std::chrono::milliseconds time_point);

	private:
		QLayout* make_environment();
		QLayout* make_control_area();
		void handler();

	private:
		QPushButton* _start{}, * _circle{}, * _reset{};
		QLabel* _time_view{};
		QListWidget* _time_list_view{};
		std::unique_ptr<stopwatch> _timer{};

	}; /// class stopwatch_window
} /// namespace watch

#endif // !STOPWATCH_WINDOW_H

