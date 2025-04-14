// multiple_indicators.h

#ifndef MULTIPLE_INDICATORS_H
#define MULTIPLE_INDICATORS_H

#include "csi_code.h"
#include "progress_bar.h"

#include <cassert>
#include <deque>
#include <iosfwd>
#include <iostream>
#include <mutex>
#include <type_traits>

namespace devices
{
	namespace sensors
	{
		template<typename Indicator,
			typename = std::enable_if_t<std::is_same_v<Indicator, progress_bar>>>
		class multiple_indicators
		{
		public:
			template<typename... Indicator>
			explicit multiple_indicators(Indicator&&... indicator) noexcept
			{
				std::lock_guard<std::mutex> lock{ _mutex };
				(_sensors.push_back(std::move(indicator)), ...);
			}
			~multiple_indicators() = default;

			auto update(std::size_t workload, std::size_t pos, std::ostream& os = std::cout) const
			{
				assert((pos < _sensors.size() && pos >= 0ull) && "Out of range of the index.");
				std::lock_guard<std::mutex> lock{ _mutex };				
				os << csi::cursor_position_move::row(static_cast<unsigned short>(pos) + 1);
				_sensors.at(pos).update(workload, os << csi::cursor_event_control::disable); 
			}

		private:
			mutable std::mutex _mutex{};
			std::deque<Indicator> _sensors{};

		public:
			multiple_indicators() = delete;
			multiple_indicators(const Indicator&) = delete;
			multiple_indicators& operator=(const Indicator&) = delete;
			multiple_indicators& operator=(Indicator&&) = delete;

		}; // class multiple_indicators
	} // namespace sensors
} // namespace devices

#endif // !MULTIPLE_INDICATORS_H

