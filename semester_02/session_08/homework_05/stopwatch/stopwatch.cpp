/// stopwatch.cpp

#include "stopwatch.h"
#include <chrono>
#include <memory>
#include <qdatetime.h>
#include <qobject.h>
#include <qtimer.h>
#include <qtmetamacros.h>
#include <stdexcept>
#include <string>

namespace watch
{
	stopwatch::stopwatch(QObject* parent)
		try : QObject(parent), _timer{ std::make_unique<QTimer>() }
	{
	}
	catch (...)
	{
		throw std::runtime_error{ "\nRuntime error: " + std::string{__FUNCTION__} +
			"\nfile: " + std::string{ __FILE__ } + "\nline: " + std::to_string(__LINE__) };
	}
	QTime stopwatch::convert_to_time(std::chrono::milliseconds time_point)
	{
		using namespace std::chrono;
		const auto hrs = duration_cast<hours>(time_point);
		time_point -= hrs;
		const auto min = duration_cast<minutes>(time_point);
		time_point -= min;
		const auto sec = duration_cast<seconds>(time_point);
		time_point -= sec;

		return QTime{ hrs.count(), min.count(), static_cast<int>(sec.count()), 
			static_cast<int>(time_point.count()) };
	}
	void stopwatch::start(std::chrono::milliseconds timeout)
	{
		QObject::connect(_timer.get(), &QTimer::timeout, this,
			[&, timeout]
			{
				const auto point = std::chrono::milliseconds{ ++_counter };
				emit update(point * timeout.count());
			}
		);
		_timer->start(timeout);
	}
	void stopwatch::stop() { _timer->stop(); }
	std::size_t stopwatch::add_circle() { return ++_circle; }
	void stopwatch::reset()
	{
		_timer->stop();
		_circle = 0;
		_counter = 0;
	}
} /// namespace watch