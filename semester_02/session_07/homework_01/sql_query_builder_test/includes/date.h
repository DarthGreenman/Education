// date.h

#ifndef DATE_H
#define DATE_H

#include "utilities.h"
#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace tpsg
{
	// Основное отличия между юлианским и григорианским календарями заключаются в 
	// системе определения високосных годов.В юлианском календаре високосный год 
	// происходит каждые четыре года без исключений, тогда как в григорианском 
	// определение високосных лет включает дополнительные правила:
	// если номер года кратен 400, то этот год — високосный. Другие года, номера
	// которых кратны 100, не считаются високосными, если только они не делятся 
	// без остатка на 400.

	class day
	{
	public:
		day() = default;
		explicit day(unsigned d) : _d{ d } {}
		day(const day&) = default;
		day(day&&) = default;
		~day() = default;

		day& operator=(const day&) = default;
		day& operator=(day&&) = default;
		auto get() const { return _d; }

	private:
		unsigned _d{};
	};

	inline auto operator==(const day& lhs, const day& rhs)
	{
		return lhs.get() == rhs.get();
	}
	inline auto operator<(const day& lhs, const day& rhs)
	{
		return lhs.get() < rhs.get();
	}
	inline auto operator>(const day& lhs, const day& rhs)
	{
		return !(lhs.get() < rhs.get());
	}

	class month
	{
	public:
		static auto jan() { return month{ 1u }; }
		static auto feb() { return month{ 2u }; }
		static auto mar() { return month{ 3u }; }
		static auto apr() { return month{ 4u }; }
		static auto may() { return month{ 5u }; }
		static auto jun() { return month{ 6u }; }
		static auto jul() { return month{ 7u }; }
		static auto aug() { return month{ 8u }; }
		static auto sep() { return month{ 9u }; }
		static auto oct() { return month{ 10u }; }
		static auto nov() { return month{ 11u }; }
		static auto dec() { return month{ 12u }; }

		month() = default;
		month(const month&) = default;
		month(month&&) = default;
		~month() = default;

		month& operator=(const month&) = default;
		month& operator=(month&&) = default;
		auto get() const { return _m; }
		static auto get_month(unsigned m)
		{
			return m == 1u ? month{ month::jan() } : m == 2u ? month{ month::feb() } :
				m == 3u ? month{ month::mar() } : m == 4u ? month{ month::apr() } :
				m == 5u ? month{ month::may() } : m == 6u ? month{ month::jun() } :
				m == 7u ? month{ month::jul() } : m == 8u ? month{ month::aug() } :
				m == 9u ? month{ month::sep() } : m == 10u ? month{ month::oct() } :
				m == 11u ? month{ month::nov() } : month{ month::dec() };
		}

	private:
		explicit month(unsigned m) : _m{ m } {}
		unsigned _m{};
	};

	inline auto operator==(const month& lhs, const month& rhs)
	{
		return lhs.get() == rhs.get();
	}
	inline auto operator<(const month& lhs, const month& rhs)
	{
		return lhs.get() < rhs.get();
	}

	class year
	{
	public:
		year() = default;
		explicit year(unsigned y) : _y{ y } {}
		year(const year&) = default;
		year(year&&) = default;
		~year() = default;

		year& operator=(const year&) = default;
		year& operator=(year&&) = default;
		auto get() const { return _y; }

	private:
		unsigned _y{};
	};

	inline auto operator==(const year& lhs, const year& rhs)
	{
		return lhs.get() == rhs.get();
	}
	inline auto operator<(const year& lhs, const year& rhs)
	{
		return lhs.get() < rhs.get();
	}

	inline auto last_day_month(const month& m, const year& y)
	{
		static std::vector<std::pair<std::string, unsigned>> _m{
			{"jan", 31u}, {"feb", 28u}, {"mar", 31u}, {"apr", 30u}, {"may", 31u}, {"jun", 30u},
			{"jul", 31u}, {"aug", 31u}, {"sep", 30u}, {"oct", 31u}, {"nov", 30u}, {"dec", 31u}
		};
		decltype(auto) days = _m.at(static_cast<std::size_t>(m.get() - 1)).second;
		auto is_leap = [](unsigned y)
			{
				return (!(y % 4) && y % 100) ? true : !(y % 100) && !(y % 400);
			};
		return (m == month{ month::feb() } && is_leap(y.get())) ? days + 1 : days;
	}

	class date
	{
	public:
		date() = default;
		date(day d, month m, year y) :
			_d{ d }, _m{ m }, _y{ y }
		{
			if (d == day{ 0 } || d > day{ last_day_month(m, y) })
				throw std::out_of_range{ "Incorrect number of days in this month..." };

			if (y < year{ 1582 } || y == year{ 1582 } && m < month{ month::oct() } ||
				y == year{ 1582 } && m == month{ month::oct() } && d < day{ 15 })
			{
				throw std::invalid_argument{ "The current date must be later than October 15, 1582" };
			}
		}
		date(const date&) = default;
		date(date&&) = default;
		~date() = default;

		date& operator=(const date&) = default;
		date& operator=(date&&) = default;

		auto get() const
		{
			return std::make_tuple(_d, _m, _y);
		}

		auto to_string() const
		{
			auto day = _d.get() < 10u ? "0" + std::to_string(_d.get()) :
				std::to_string(_d.get());
			auto month = _m.get() < 10u ? "0" + std::to_string(_m.get()) :
				std::to_string(_m.get());

			return std::to_string(_y.get()) + "-" + std::move(month) + "-" +
				std::move(day);
		}

	private:
		day _d{};
		month _m{};
		year _y{};
	};

	inline auto operator==(const date& lhs, const date& rhs)
	{
		const auto& [dl, ml, yl] = lhs.get();
		const auto& [dr, mr, yr] = rhs.get();
		return dl == dr && ml == mr && yl == yr;
	}
	inline auto operator<(const date& lhs, const date& rhs)
	{
		const auto& [dl, ml, yl] = lhs.get();
		const auto& [dr, mr, yr] = rhs.get();
		return yl < yr || yl == yr && ml < mr || yl == yr && ml == mr && dl < dr;
	}

	template<typename T,
		typename = std::enable_if_t<std::is_same_v<T, day> || std::is_same_v<T, month> ||
		std::is_same_v<T, year>>>
		struct from_string
	{
		static auto get(const std::string& y)
		{
			return T{ static_cast<unsigned>(utls::to_number(y)) };
		}
	};

	template<>
	struct from_string<month>
	{
		static auto get(const std::string& m)
		{
			auto mn = static_cast<unsigned>(utls::to_number(m));
			return month::get_month(mn);
		}
	};

	inline auto date_from_string(const std::string& data_as_string)
	{
		std::istringstream stream{ data_as_string };
		auto parts = utls::split(stream, '-');
		// Проверить на количество частей
		return date{ from_string<tpsg::day>::get(parts[2]), from_string<tpsg::month>::get(parts[1]),
			from_string<tpsg::year>::get(parts[0])
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::istream& operator>> (std::istream& in, tpsg::day& day)
	{
		unsigned val{}; in >> val;
		auto d = tpsg::day{ val };
		std::swap(day, d);
		return in;
	}

	std::istream& operator>> (std::istream& in, tpsg::month& month)
	{
		unsigned val{};	in >> val;
		auto m = tpsg::month{ tpsg::month(tpsg::month::get_month(val)) };
		std::swap(month, m);
		return in;
	}

	std::istream& operator>> (std::istream& in, tpsg::year& year)
	{
		unsigned val{}; in >> val;
		auto y = tpsg::year{ val };
		std::swap(year, y);
		return in;
	}


} // namespace tpsg

#endif // ! DATE_H