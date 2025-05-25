// sql_query_builder.h

#ifndef SQL_QUERY_BUILDER_H_IN_MY_PROJECT
#define SQL_QUERY_BUILDER_H_IN_MY_PROJECT

#include "sql_expr_base.h"
#include "sql_expr_requi.h"
#include "sql_expr_where.h" 
#include <algorithm>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace sql
{
	namespace query
	{
		struct sql_select_query_structure
		{
			std::vector<sql::query::expr_requi> select;
			sql::query::expr_requi from;
			std::vector<sql::query::expr_where> where;
		};
	} // namespace query
} // namespace sql

namespace patterns
{
	namespace generative
	{
		class sql_select_query
		{
		public:
			friend void swap(sql_select_query& lhs, sql_select_query& rhs) noexcept;

			/// SELECT * FROM TABLE_NAME;
			explicit sql_select_query(const char* table_name)
			{
				_struct.from = std::move(decltype(_struct.from){table_name});
			}

			/// SELECT * FROM TABLE_NAME AS NICKNAME;
			explicit sql_select_query(const char* table_name, const char* nickname)
			{
				_struct.from = std::move(decltype(_struct.from){table_name, nickname});
			}

			sql_select_query(const sql_select_query& query)
			{
				std::copy(std::cbegin(query._struct.select),
					std::end(query._struct.select), std::back_inserter(_struct.select));
				_struct.from = query._struct.from;
				std::copy(std::cbegin(query._struct.where),
					std::end(query._struct.where), std::back_inserter(_struct.where));

			}
			sql_select_query(sql_select_query&& query) noexcept : sql_select_query()
			{
				using std::swap; // Делаем возможным выроб лучшего кандидата
				swap(*this, query);
			}
			~sql_select_query() = default;

			/// ADD_COLUM
			/// Методы добавляют в запрос одно, или сразу несколько выражений для SELECT:
			/// SELECT COLUM_NAME_1 AS NICKNAME_1, COLUM_NAME_2, ..., COLUM_NAME_N AS NICKNAME_M
			template<typename... Args,
				typename = std::enable_if_t<(std::is_same_v<Args, sql::query::expr_requi> && ...)>>
				decltype(auto) add_colum(Args&&... args)
			{
				(_struct.select.emplace_back(std::forward<Args>(args)), ...);
				return *this;
			}
			template<typename... Args>
			decltype(auto) add_colum(const char* arg, Args&&... args)
			{
				_struct.select.emplace_back(arg, std::forward<Args>(args)...);
				return *this;
			}

			/// ADD_WHERE
			/// Методы добавляют в запрос одно, или сразу несколько выражений для WHERE:
			/// WHERE expr1 AND|OR|NOT expr2 ...
			template<typename... Args,
				typename = std::enable_if_t<(std::is_same_v<Args, sql::query::expr_where> && ...)>>
				decltype(auto) add_where(Args&&... args)
			{
				(_struct.where.emplace_back(std::forward<Args>(args)), ...);
				return *this;
			}
			template<typename... Args>
			decltype(auto) add_where(const char* arg, Args&&... args)
			{
				_struct.where.emplace_back(arg, std::forward<Args>(args)...);
				return *this;
			}

			auto build() -> std::string
			{
				if (!_expr.empty())
					return _expr;

				/// SELECT
				std::string expr{};
				/// Формирует список столбцов
				bind_select(expr);
				expr.push_back(sql::special_character::endl());
				/// FROM
				bind_from(expr);
				/// WHERE expression ...
				if (!_struct.where.empty()) {
					expr.push_back(sql::special_character::endl());
					bind_where(expr);
				}
				expr.push_back(sql::character::semicolon());
				_expr = std::move(expr);

				return _expr;
			}

		private:
			auto bind_select(std::string& expr) -> void
			{
				expr += sql::keyword::SELECT;
				expr.push_back(sql::character::space());

				if (_struct.select.empty()) {
					expr.push_back(sql::character::asterisk());
					return;
				}
				const auto select =
					bind(std::begin(_struct.select), std::end(_struct.select),
						sql::query::expr_requi{});
				std::copy(std::cbegin(select), std::cend(select),
					std::back_inserter(expr));
			}
			auto bind_from(std::string& expr) -> void
			{
				expr += sql::keyword::FROM;
				expr.push_back(sql::character::space());
				expr += _struct.from.get();
			}
			auto bind_where(std::string& expr) -> void
			{
				if (_struct.where.empty())
					return;

				expr += sql::keyword::WHERE;
				const auto where =
					bind(std::begin(_struct.where), std::end(_struct.where),
						sql::query::expr_where{});
				std::copy(std::cbegin(where), std::cend(where),
					std::back_inserter(expr));
			}
			template<typename Iter, typename T>
			auto bind(Iter first, Iter last, T expr = T{}) -> std::string
			{
				using elem_type = typename std::iterator_traits<decltype(first)>::value_type;
				auto count = std::distance(first, last);

				std::for_each(first, last, [&expr, &count](elem_type& elem)
					{
						if (!(--count))
							elem.bind(nullptr);
						expr += elem;
					}
				);
				return expr.get();
			}

			void swap(sql_select_query& rhs) noexcept
			{
				std::swap(_struct.select, rhs._struct.select);
				std::swap(_struct.from, rhs._struct.from);
				std::swap(_struct.where, rhs._struct.where);
			}

			sql::query::sql_select_query_structure _struct{};
			std::string _expr{};

		public:
			sql_select_query() = default;
			sql_select_query& operator=(const sql_select_query&) = delete;
			sql_select_query& operator=(sql_select_query&&) = delete;

		}; // class sql_select_query

		void swap(sql_select_query& lhs, sql_select_query& rhs) noexcept { lhs.swap(rhs); }

	} // namespace generative
} // namespace patterns

#endif // !SQL_QUERY_BUILDER_H_IN_MY_PROJECT