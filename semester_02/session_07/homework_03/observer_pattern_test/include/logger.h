/// logger.h

#ifndef LOGGER_H_IN_MY_PROJECT
#define LOGGER_H_IN_MY_PROJECT

#include "viewer.h"

#include<algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <memory>
#include <string>

namespace pattern
{
	class logger
	{
	public:
		logger(const logger&) = delete;
		logger& operator=(const logger&) = delete;

		static logger& get_instance()
		{
			static logger instance;
			return instance;
		}

		void viewer_reg(std::shared_ptr<viewer> viewer) { _viewers.push_back(viewer); }
		void viewer_unreg(std::shared_ptr<viewer> viewer) { std::erase(_viewers, viewer); }

		void warning(const std::string& message) const { notify(message, &viewer::warning); }
		void error(const std::string& message) const { notify(message, &viewer::error); }
		void fatal(const std::string& message) const { notify(message, &viewer::fatal); }

	private:
		logger() = default;

		void notify(const std::string& message, void(viewer::* func)(const std::string&)) const
		{
			std::for_each(std::cbegin(_viewers), std::cend(_viewers),
				[&func, &message](const typename decltype(_viewers)::value_type& viewer)
				{
					std::invoke(func, viewer, message);
				}
			);
		}
		std::list<std::shared_ptr<viewer>> _viewers{};

	}; /// class logger
} /// namespace pattern

#endif // !LOGGER_H_IN_MY_PROJECT
