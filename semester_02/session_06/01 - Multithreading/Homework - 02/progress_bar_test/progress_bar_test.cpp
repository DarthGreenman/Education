// progress_bar_test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "include/csi_code.h"
#include "include/multiple_indicators.h"
#include "include/progress_bar.h"
#include "include/progress_bar_param.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <iterator>
#include <thread>
#include <type_traits>

int main()
{
	using namespace devices::sensors;
	const multiple_indicators<progress_bar> sensors
	{
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178)}}),
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178), csi::foreground_color::green}}),
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178), csi::foreground_color::yellow}}),
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178), csi::foreground_color::blue}}),
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178), csi::foreground_color::cyan}}),
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178), csi::foreground_color::magenta}}),
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178), csi::foreground_color::red}}),
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178), csi::foreground_color::dark_gray}}),
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178), csi::foreground_color::light_gray}}),
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178), csi::foreground_color::light_green}}),
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178), csi::foreground_color::light_yellow}}),
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178), csi::foreground_color::light_blue}}),
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178), csi::foreground_color::light_cyan}}),
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178), csi::foreground_color::light_magenta}}),
		std::move(progress_bar{ progress_bar_param{ static_cast<char>(178), csi::foreground_color::light_red}})
	};

	auto start = [&sensors](std::chrono::milliseconds tact, std::size_t pos)
		{
			for (std::size_t i{}; i <= progress_bar_param::job_done(); ++i)
			{
				sensors.update(i, pos);
				std::this_thread::sleep_for(std::chrono::milliseconds(tact));
			}
		};

	using namespace std::chrono_literals;
	constexpr std::array<std::chrono::milliseconds, 15> tacts{
		10ms, 125ms, 305ms, 267ms, 207ms, 155ms, 175ms, 115ms, 225ms, 345ms, 23ms, 107ms, 55ms, 175ms, 100ms
	};

	std::deque<std::thread> tasks{};
	for (std::size_t i{}; i < tacts.size(); ++i)
		tasks.emplace_back(std::thread{ start, tacts.at(i), i });

	std::for_each(std::begin(tasks), std::end(tasks),
		[](typename std::iterator_traits<std::decay_t<decltype(std::cbegin(tasks))>>::value_type& task)
		{
			if (task.joinable())
				task.join();
		}
	);

	std::cout << csi::cursor_position_move::row(tacts.size()) << '\n';
	std::system("pause");
	return 0;
}