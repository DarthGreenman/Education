// sql_query_builder_test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include "includes/date.h"
#include "includes/sql_expr_base.h"
#include "includes/sql_expr_requi.h"
#include "includes/sql_expr_where.h"
#include "includes/sql_query_builder.h"
#include "includes/thread_pool.h"
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

int main()
{
	using namespace sql::query;
	using namespace tpsg;
	try
	{
		std::vector<patterns::generative::sql_select_query> querys{};
		querys.reserve(1000);

		decltype(querys)::value_type query_a{ "it_specialists" };
		/// Добавляем по одному столбцу
		query_a.add_colum("firstname", "nick").add_colum("lastname");
		/// Добавляем одновременно несколько столбцов
		using colum = sql::query::expr_requi;
		query_a.add_colum(
			colum{ "date_of_birth", "birth" },
			colum{ "employee_position", "level" },
			colum{ "mailbox" }
		);
		/// Добавляем по одному условию
		query_a.add_where("birth", sql::selection::less,
			tpsg::date{ day{20}, month{month::may()}, year{2000} }, sql::logic::AND)
			.add_where("firstname", sql::selection::equal, "Nemo", sql::logic::AND)
			.add_where("pos", sql::selection::greater_or_equal, 1234.09);

		decltype(querys)::value_type query_b{ "sales_department_staff", "sales" };
		/// Добавляем одновременно несколько столбцов
		using colum = sql::query::expr_requi;
		query_b.add_colum(
			colum{ "firstname" },
			colum{ "lastname" },
			colum{ "date_of_birth", "birth" },
			colum{ "employee_position", "pos" },
			colum{ "mailbox" },
			colum{ "postal_address", "address" }
		);
		/// Добавляем одновременно несколько условий
		using where = sql::query::expr_where;
		tpsg::date death_line{ day{22}, month{month::may()}, year{2025} };
		query_b.add_where(
			where{ "birth", sql::selection::less, std::move(death_line), sql::logic::OR },
			where{ "birth", sql::selection::unequal, std::move(death_line), sql::logic::OR },
			where{ "pos", sql::selection::greater_or_equal, 12 }
		);

		/// Добавляем запросы на все данные таблиц SELECT * FROM table_name из списка имен таблиц для примера,
		/// из зарезирвированных слов SQL.
		std::for_each(std::cbegin(sql::keywords), std::cend(sql::keywords),
			[&querys](const typename std::iterator_traits<decltype(std::cbegin(sql::keywords))>::value_type& elem)
			{
				querys.push_back(patterns::generative::sql_select_query{ elem });
			});
		/// Добавляем запросы на все данные таблиц
		querys.push_back(decltype(querys)::value_type{ "managers1" });
		querys.push_back(decltype(querys)::value_type{ "managers2" });
		querys.push_back(decltype(querys)::value_type{ "managers3" });
		querys.push_back(decltype(querys)::value_type{ "managers4" });
		querys.push_back(decltype(querys)::value_type{ "managers5" });
		querys.push_back(decltype(querys)::value_type{ "managers6" });

		/// Добавляем "расширенные" запросы
		querys.push_back((std::move(query_a)));
		querys.push_back((std::move(query_b)));

		/// Обрабатываем запросы в пуле потоков
		using iterator = typename decltype(querys)::iterator;
		using value_type = typename decltype(querys)::value_type;

		auto build =	[](value_type& query)
			{
				std::cout << query.build() << "\n\n";
			};

		using build_query = std::function<decltype(build)(iterator, iterator, decltype(build))>;
		multitask::thread_pool<typename build_query::result_type> exec{};

		auto first = std::begin(querys);
		auto last = std::end(querys);
		const auto median = querys.size() / 2;

		auto run1 =
			exec.submit(build_query{ std::for_each<iterator, decltype(build)> },
				first, std::next(first, median), build);
		auto run2 =
			exec.submit(build_query{ std::for_each<iterator, decltype(build)> },
				std::next(first, median + 1), last, build);
		run1.get();
		run2.get();
	}
	catch (const std::exception& err) { std::cout << err.what(); }
	std::cout << '\n';
	
	std::system("pause");
	return 0;
}