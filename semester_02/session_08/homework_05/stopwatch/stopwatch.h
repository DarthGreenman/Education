/// stopwatch.h

#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <memory>
#include <qdatetime.h>
#include <qobject.h>
#include <qtimer.h> 
#include <qtmetamacros.h>
#include <utility>

namespace watch
{
	class stopwatch : public QObject
	{
		Q_OBJECT

	public:
		stopwatch(std::chrono::milliseconds timeout, QObject* parent = nullptr);
		~stopwatch() = default;
		/// <summary>
		/// Раскладывает миллисекунды на часы, минуты, секунды, миллисекунды (как часть секнды)
		/// Возвращает объект QTime
		/// </summary>
		/// <param name="time_point"></param>
		/// <returns></returns>
		static QTime format_to_view(std::chrono::milliseconds time_point);
		void start() { _timer->start(); }
		void stop() { _timer->stop(); }
		/// <summary>
		/// Вычисляет номер и длительность круга
		/// Возвращает параметры круга в структуре
		/// </summary>
		/// <returns></returns>
		std::pair<std::size_t, std::chrono::milliseconds> add_circle();
		void reset();

	private:
		std::unique_ptr<QTimer> _timer{};
		
		std::chrono::milliseconds _timeout{};
		std::chrono::milliseconds _time_point{};
		std::chrono::milliseconds _prev_time_point{};		
		std::size_t _circle{};

	signals:
		void update(std::chrono::milliseconds time_point);

	}; /// class stopwatch
} /// namespace watch
#endif // !STOPWATCH_H