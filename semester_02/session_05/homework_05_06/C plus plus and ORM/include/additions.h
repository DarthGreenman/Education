// additions.h

#ifndef ADDITIONS_H
#define ADDITIONS_H

#include "date.h"

#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include <Wt/Dbo/SqlConnection.h>
#include <Wt/Dbo/SqlStatement.h>
#include <Wt/Dbo/StdSqlTraits.h>

namespace orm
{
	namespace tps
	{
		struct person_name
		{
			std::string firstname{};
			std::string middlename{};
			std::string lastname{};
		};
	} // namespace tps

	namespace view
	{
		template<typename Arg>
		auto print(int width, Arg&& arg)
		{
			std::cout << std::setiosflags(std::ios::left);
			std::cout << std::setw(width) << arg;
		}

		template<typename... Args, typename = std::enable_if_t<sizeof... (Args) != 1>>
		auto print(const std::vector<int>& widths, Args&& ... args)
		{
			std::size_t index{};
			((print(widths[index++], std::forward<Args>(args))), ...);
		}

	} // namespace view

	namespace tps
	{
		template<typename... Types>
		struct type_list
		{
			static constexpr auto size = sizeof...(Types);
		};

		template<std::size_t I, typename L>
		struct type_at {};

		template<std::size_t I, typename T, typename... Tail>
		struct type_at<I, type_list<T, Tail...>>
		{
			using type = typename type_at<I - 1ull, type_list<Tail...>>::type;
		};

		template<typename T, typename... Tail>
		struct type_at<0ull, type_list<T, Tail...>>
		{
			using type = T;
		};

		template<std::size_t I, typename T>
		using type_at_t = typename type_at<I, T>::type;
	} // namespace tps

} // namespace orm

std::ostream& operator<<(std::ostream& out, const orm::tps::person_name& type_name)
{
	const auto fn = type_name.firstname + " " + type_name.middlename + " " + type_name.lastname;
	out << fn;
	return out;
}
std::ostream& operator<<(std::ostream& out, const tpsg::year& year)
{
	out << std::to_string(year.get());
	return out;
}
std::ostream& operator<<(std::ostream& out, const tpsg::date& date)
{
	out << date.to_string();
	return out;
}

namespace Wt
{
	namespace Dbo
	{
		template <class Action>
		void field(Action& action, orm::tps::person_name& person, const std::string& type_name,
			int size = -1)
		{
			field(action, person.firstname, "first" + type_name);
			field(action, person.middlename, "middle" + type_name);
			field(action, person.lastname, "last" + type_name);
		}
	} // namespace Dbo
} // namespace Wt

namespace Wt
{
	namespace Dbo
	{
		template<>
		struct sql_value_traits<tpsg::date>
		{
			static std::string type(SqlConnection* conn, int size)
			{
				return sql_value_traits<std::string>::type(conn, size);
			}

			static void bind(tpsg::date date, SqlStatement* statement, int column, int size)
			{
				statement->bind(column, date.to_string());
			}

			static bool read(tpsg::date& date, SqlStatement* statement, int column, int size)
			{
				std::string s;
				bool result = statement->getResult(column, &s, size);
				if (!result)
					return false;
				date = tpsg::date_from_string(s);
				return true;
			}
		};
	}// namespace Dbo

	namespace Dbo
	{
		template<>
		struct sql_value_traits<tpsg::year>
		{
			static std::string type(SqlConnection* conn, int size)
			{
				return sql_value_traits<std::string>::type(conn, size);
			}

			static void bind(tpsg::year year, SqlStatement* statement, int column, int size)
			{
				statement->bind(column, std::to_string(year.get()));
			}

			static bool read(tpsg::year& year, SqlStatement* statement, int column, int size)
			{
				std::string s;
				bool result = statement->getResult(column, &s, size);
				if (!result)
					return false;
				year = tpsg::from_string<tpsg::year>::get(s);
				return true;
			}
		};
	} // namespace Dbo
} // namespace W

#endif // !ADDITIONS_H
