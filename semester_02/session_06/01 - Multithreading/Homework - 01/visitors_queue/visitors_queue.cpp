// visitors_queue.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "my_localisation.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <iterator>
#include <queue>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace th
{
	class visitors_queue
	{
	public:
		using time_point = typename std::chrono::time_point<std::chrono::high_resolution_clock>;
		using queue = typename std::queue<std::pair<std::string, time_point>>;
		using visiting_report = typename std::queue<std::tuple<std::string, time_point, time_point>>;

		visitors_queue() = default;
		explicit visitors_queue(const std::chrono::seconds& call_time,
			const std::chrono::seconds& wait_time) :
			_duration{ std::make_pair(call_time, wait_time) } {
		}
		visitors_queue(const visitors_queue&) = default;
		visitors_queue(visitors_queue&&) = default;
		~visitors_queue() = default;

		visitors_queue& operator=(const visitors_queue&) = default;
		visitors_queue& operator=(visitors_queue&&) = default;

		auto came(std::vector<std::string>&& visitors)
		{ 
			for (auto first = std::begin(visitors); first != std::end(visitors); ++first)
			{
				std::this_thread::sleep_for(_duration.first);
				_queue.emplace(std::make_pair(
					*first, std::chrono::high_resolution_clock::now())); // O(1)
			}
		}
		auto gone(visiting_report& report) 
		{
			std::this_thread::sleep_for(_duration.second);
			for (;!_queue.empty(); _queue.pop())
			{
				std::this_thread::sleep_for(_duration.second);
				report.emplace(_queue.front().first,_queue.front().second, 
					std::chrono::high_resolution_clock::now()); // O(1)
			}
		}

	private:
		queue _queue{};
		std::pair<std::chrono::seconds, std::chrono::seconds> _duration{};
	};

	static auto print(visitors_queue::visiting_report&& report,
		const std::string& heading)
	{
		using namespace std;
		cout << '\n' << heading << '\n';
		for (;!report.empty(); report.pop())
		{
			const auto& [name, start, finish] = report.front();
			const chrono::duration<double> time_span = finish - start;
			cout << setiosflags(ios::left)
				<< setw(15) << name
				<< setprecision(15) << time_span.count() << '\n';
		}
	}
} // namespace th

int main()
{
	const my::console_localisation<my::code_pages::rus> locale{};
	std::vector<std::string> visitors{
		"Смирнов",
		"Иванов",
		"Кузнецов",
		"Соколов",
		"Попов",
		"Лебедев",
		"Новиков",
		"Морозов",
		"Михайлов",
		"Петров"
	};

	using namespace th;
	using namespace std::literals;
	const std::chrono::seconds call_time{ 1s };
	const std::chrono::seconds wait_time{ 2s };

	std::jthread wait{ [=]() 
		{
			const auto delay = std::size(visitors) * wait_time.count();
			std::cout << "Ждите";
			for (std::decay_t<decltype(delay)> wait{}; wait < delay; ++wait)
			{
				std::this_thread::sleep_for(1s);
				std::cout << '.';
			}
		}
	};

	visitors_queue vsq{ call_time, wait_time };
	void (visitors_queue:: * v_came)(decltype(visitors)&&) = &visitors_queue::came;
	std::thread producer{ v_came, &vsq, std::move(visitors) };
	
	visitors_queue::visiting_report report{};
	void (visitors_queue:: * v_gone)(decltype(report)&) = &visitors_queue::gone;
	std::thread consurner{ v_gone, &vsq, std::ref(report) };

	producer.join();
	consurner.join();
	print(std::move(report), "Отчет о времени, проведенной в очереди:");

	std::system("pause");
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.