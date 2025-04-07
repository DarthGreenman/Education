// progress_bar_param.h

#ifndef PROGRESS_BAR_PARAM_H
#define PROGRESS_BAR_PARAM_H

#include "csi_code.h"
#include "my_types.h"

namespace devices
{
	namespace sensors
	{
		struct progress_bar_param
		{
			static constexpr auto remaining() { return static_cast<char>(my::ascii::space); }
			static constexpr auto job_done() { return 100ull; };
			constexpr auto scale() const { return width / job_done(); };
			char completed{};
			csi::foreground_color pen_color{ csi::foreground_color::dark_gray };
			std::size_t width{ 100 };
		}; // struct progress_bar_param
	} // namespace sensors
} // namespace devices

#endif // !PROGRESS_BAR_PARAM_H

