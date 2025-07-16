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
#include <utility>

namespace watch
{
	using namespace std::chrono_literals;

	stopwatch::stopwatch(std::chrono::milliseconds timeout, QObject* parent)
		try : QObject(parent), _timer{ std::make_unique<QTimer>() }, _timeout{ timeout }
	{
		_timer.get()->setInterval(timeout);
		QObject::connect(_timer.get(), &QTimer::timeout, this, [&, timeout]
			{
				emit update(++_time_point * _timeout.count());
			}
		);
	}
	catch (...)
	{
		throw std::runtime_error{ "\nRuntime error: " + std::string{__FUNCTION__} +
			"\nfile: " + std::string{ __FILE__ } + "\nline: " + std::to_string(__LINE__) };
	}
	/// <summary>
	/// Раскладывает миллисекунды на часы, минуты, секунды, миллисекунды (как часть секнды)
	/// Возвращает объект QTime
	/// </summary>
	/// <param name="time_point"></param>
	/// <returns></returns>
	QTime stopwatch::format_to_view(std::chrono::milliseconds time_point)
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
	/// <summary>
	/// Вычисляет номер и длительность круга
	/// Возвращает параметры круга в структуре
	/// </summary>
	/// <returns></returns>
	std::pair<std::size_t, std::chrono::milliseconds> stopwatch::add_circle()
	{
		const auto curr_time_point = _time_point * _timeout.count();
		const auto duration = curr_time_point - _prev_time_point;
		_prev_time_point = curr_time_point;
		return std::make_pair(++_circle, duration);
	}
	void stopwatch::reset()
	{
		_circle = 0;
		_time_point = 0ms;
		_prev_time_point = 0ms;
	}
} /// namespace watch