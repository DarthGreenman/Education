// view.h

#ifndef VIEW_H
#define VIEW_H

#include "additions.h"
#include "book.h"
#include "driver.h"
#include "publisher.h"
#include "reserve.h"
#include "sale.h"
#include "store.h"
#include "window_base.h"
#include "writer.h"

#include <iostream>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <Wt/Dbo/ptr.h>

namespace orm
{
	namespace tps
	{
		namespace my = Wt::Dbo;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		template<typename Table_type>
			requires Is_tabulartypes<Table_type>
		auto view(const my::ptr<Table_type>& data)
		{
			Table_type::view(data);
			std::cout << '\n';
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		template<typename... Tabular_types>
			requires (Is_tabulartypes<Tabular_types> && ...)
		auto view(const std::tuple<my::ptr<Tabular_types> ...>& data)
		{
		}
		template<>
		auto view(const std::tuple<my::ptr<book>, my::ptr<writer>, my::ptr<publisher>>& data)
		{
			auto& [t1, t2, t3] = data;
			const auto& [heading, year] = t1->get<0, 1>();
			const auto& [name, genre, birth, death] = t2->get<0, 1, 2, 3>();
			const auto& [title, address] = t3->get<0, 1>();

			view::print({ 4,/*heading*/50,/*year*/7,/*name*/35,/*genre*/12,/*birth*/12,/*title*/40,/*address*/40 },
				t1->id(), heading, year, name, genre, birth, title, address);
			std::cout << '\n';
		}
		template<>
		auto view(const std::tuple<my::ptr<reserve>, my::ptr<book>, my::ptr<store>>& data)
		{
			auto& [t1, t2, t3] = data;
			const auto& [count] = t1->get<0>();
			const auto& [heading, year] = t2->get<0, 1>();
			const auto& [title, web] = t3->get<0, 1>();

			view::print({ 4,/*heading*/50,/*year*/7,/*title*/50,/*web*/20,/*count*/7 },
				t1->id(), heading, year, title, web, count);
			std::cout << '\n';
		}
		template<>
		auto view(const std::tuple<my::ptr<sale>, my::ptr<reserve>>& data)
		{
			auto& [t1, t2] = data;
			const auto& [count, price] = t1->get<0, 1>();
			const auto& [amount] = t2->get<0>();

			view::print({ 4,/*count*/7,/*price*/15,/*amount*/7 },
				t1->id(), count, price, amount);
			std::cout << '\n';
		}
	} // namespace tps
} // namespace orm

#endif // !VIEW_H

