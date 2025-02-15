// driver.h

#ifndef DRIVER_H
#define DRIVER_H

#include "additions.h"
#include "book.h"
#include "publisher.h"
#include "reserve.h"
#include "sale.h"
#include "store.h"
#include "writer.h"


#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <unordered_map>
#include <Wt/Dbo/backend/Postgres.h>
#include <Wt/Dbo/collection.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/Exception.h>
#include <Wt/Dbo/ptr.h>
#include <Wt/Dbo/Query.h>
#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/Session_impl.h>
#include <Wt/Dbo/Transaction.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace orm
{
	template<typename A>
	concept Is_tabulartypes = is_book_v<A> || is_publisher_v<A> || is_sale_v<A> || is_store_v<A> ||
		is_reserve_v<A> || is_writer_v<A>;

	namespace drv
	{
		class driver
		{
		public:
			using handle = typename std::size_t;

			driver() = default;
			explicit driver(const std::string& connection_string) :
				_psession{ std::make_unique<Wt::Dbo::Session>() }
			{
				try
				{
					auto postgres = std::make_unique<Wt::Dbo::backend::Postgres>(connection_string);
					(_psession.get())->setConnection(std::move(postgres));
					mapping<_tabulartypes>(std::make_index_sequence<_tabulartypes::size>{});
				}
				catch (std::exception&) { throw /* плюс исключения Wt::Dbo::Exception */; }
			}
			driver(const driver&) = delete;
			driver(driver&&) = delete;
			virtual ~driver() = default;

			driver& operator=(const driver&) = delete;
			driver& operator=(driver&&) = delete;

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename Table_type, typename... Args> requires Is_tabulartypes<Table_type>
			auto add(Args&&... args)
			{
				Wt::Dbo::Transaction work{ *_psession.get() };
				Wt::Dbo::ptr<Table_type> data = (_psession.get())->add(std::make_unique<Table_type>(std::forward<Args>(args)...));
				work.commit();
				return data;
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename Table_type> requires Is_tabulartypes<Table_type>
			auto remove(std::size_t id)
			{
				Wt::Dbo::Transaction work{ *_psession.get() };
				Wt::Dbo::ptr<Table_type> data = (_psession.get())->find<Table_type>().where("id = ?").bind(std::to_string(id));
				data.remove();
				work.commit();
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto session() { return _psession.get(); }
			template<typename Table_type, typename... Tabular_types,
				typename = std::enable_if_t<sizeof... (Tabular_types) != 0>> requires Is_tabulartypes<Table_type> &&
				(Is_tabulartypes<Tabular_types> && ...)
				decltype(auto) get()
			{
				// Подготовка части заголовка SQL
				auto sql = std::string{ "SELECT " + Table_type::nickname() };
				((sql += ", " + Tabular_types::nickname()), ...);
				sql += " FROM \"" + std::string{ Table_type::type_name() } + "\" " + Table_type::nickname();

				using Tuple = typename std::tuple<Wt::Dbo::ptr<Table_type>, Wt::Dbo::ptr<Tabular_types> ...>;
				typename Wt::Dbo::Query<Tuple> query = (_psession.get())->query<Tuple>(sql);
				// Формирование хвоста SQL в свертке
				(query.join<Tabular_types>(Tabular_types::nickname(), Table_type::nickname() + "." +
					std::string{ Tabular_types::type_name() } + "_id = " + Tabular_types::nickname() + ".id"), ...);
				// ...
				query.orderBy(Table_type::nickname() + "." + Table_type::order_by());

				return make(std::move(query.resultList())); // Переместить данные из Wt::Dbo::collection<T> в std::vector<T>
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename Table_type> requires Is_tabulartypes<Table_type>
			decltype(auto) get()
			{
				typename Wt::Dbo::collection<Wt::Dbo::ptr<Table_type>> source =
					(_psession.get())->find<Table_type>().orderBy(Table_type::order_by());
				return make(std::move(source)); // Переместить данные из Wt::Dbo::collection<T> в std::vector<T>
			}
			// Создать схему базы данных для сопоставленных таблиц. Создание схемы завершится исключением,
			// если одна или несколько таблиц уже существовали, и или есть ошибки в даннх для связывания таблиц.
			// Создание таблиц выполняется в транзакции, которая откатывается при возникновении ошибки.
			auto make_schema()
			{
				try { (_psession.get())->createTables(); }
				catch (Wt::Dbo::Exception) { throw; }
			}

		private:
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename Table_type> requires Is_tabulartypes<Table_type>
			auto mapping()
			{
				(_psession.get())->mapClass<Table_type>(Table_type::type_name());
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename Typelist, std::size_t... I>
			auto mapping(std::index_sequence<I...>)
			{
				((mapping<tps::type_at_t<I, Typelist>>()), ...);
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename Container>
			decltype(auto) make(Container&& cont)
			{
				using namespace std;
				using Value_type = typename iterator_traits<decay_t<decltype(cbegin(cont))>>::value_type;
				vector<Value_type> dest{};
				dest.reserve(cont.size());
				for_each(cbegin(forward<Container>(cont)), cend(forward<Container>(cont)),
					[&dest](const Value_type& value) { dest.push_back(value); });
				return dest;
			}

		private:
			std::unique_ptr<Wt::Dbo::Session> _psession{};

			using _tabulartypes =
				typename tps::type_list<
				tps::book,
				tps::publisher,
				tps::reserve,
				tps::sale,
				tps::store,
				tps::writer>;

		public:
			using Tabulartypes = _tabulartypes;
		};

	} // namespace drv
} // namespace orm



#endif // !DRIVER_H