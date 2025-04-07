// progress_bar.h

#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include "csi_code.h"
#include "my_types.h"
#include "progress_bar_param.h"

#include <__msvc_ostream.hpp>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <mutex>
#include <thread>
#include <utility>

namespace devices
{
	namespace sensors
	{
		class progress_bar
		{
		public:
			friend auto swap(progress_bar&, progress_bar&) noexcept;

			explicit progress_bar(const progress_bar_param& param)
			{
				std::lock_guard<std::mutex> lock{ _mutex };
				_param = param;
			}
			progress_bar(progress_bar&& movable) noexcept : progress_bar()
			{
				std::swap(_param, movable._param);
			}
			~progress_bar() = default;

			auto update(std::size_t workload, std::ostream& os = std::cout) const
			{
				std::lock_guard<std::mutex> lock{ _mutex };
				if (workload > progress_bar_param::job_done())
					return;
				
				os << _param.pen_color << '\r';
#ifdef _DEBUG
				os << std::this_thread::get_id(); 
				os << csi::cursor_position_move::col(15);
#endif // _DEBUG
				for (auto completed = workload * _param.scale(),
					i = 0ull; i < _param.width; ++i)
				{
					os << (i < completed ? _param.completed : progress_bar_param::remaining());
				}
				os << my::ascii::space << workload << my::ascii::percent;
			}
		private:
			progress_bar() = default;

		private:
			mutable std::mutex _mutex{};
			progress_bar_param _param{};

		public:
			progress_bar(const progress_bar&) = delete;
			progress_bar& operator=(const progress_bar&) = delete;
			progress_bar& operator=(progress_bar&&) = delete;

		}; // class progress_bar

		auto swap(progress_bar& lhs, progress_bar& rhs) noexcept
		{
			if (&lhs == &rhs)
				return;

			std::unique_lock<std::mutex> ull{ lhs._mutex, std::defer_lock };
			std::unique_lock<std::mutex> ulr{ rhs._mutex, std::defer_lock };
			std::lock(ull, ulr);

			std::swap(lhs._param, rhs._param);
		}
	} // namespace sensors
} // namespace devices

#endif // !PROGRESS_BAR_H