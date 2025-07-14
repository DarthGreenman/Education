/// stopwatch.h

#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <memory>
#include <qdatetime.h>
#include <qobject.h>
#include <qtimer.h> 
#include <qtmetamacros.h>

namespace watch
{
	class stopwatch : public QObject
	{
		Q_OBJECT

	public:
		stopwatch(QObject* parent = nullptr);
		~stopwatch() = default;

		static QTime convert_to_time(std::chrono::milliseconds time_point);
		void start(std::chrono::milliseconds timeout);
		void stop();
		std::size_t add_circle();
		void reset();

	private:
		std::unique_ptr<QTimer> _timer{};
		std::size_t _counter{};
		std::size_t _circle{};

	signals:
		void update(std::chrono::milliseconds time_point);

	}; /// class stopwatch
} /// namespace watch
#endif // !STOPWATCH_H