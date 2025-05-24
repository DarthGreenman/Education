// sql_query_builder.h

#ifndef SQL_QUERY_BUILDER_H_IN_MY_PROJECT
#define SQL_QUERY_BUILDER_H_IN_MY_PROJECT
#include "sql_expr_base.h"
#include "sql_expr_requi.h"
#include "sql_expr_where.h" 
#include <functional>
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
				decltype(_query.from) table{ table_name };
				_query.from = std::move(table);
			}

			/// SELECT * FROM TABLE_NAME AS NICKNAME;
			explicit sql_select_query(const char* table_name, const char* nickname)
			{
				decltype(_query.from) table{ table_name, nickname };
				_query.from = std::move(table);
			}
			
			sql_select_query(const sql_select_query&) = delete;
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
				(_query.select.emplace_back(std::forward<Args>(args)), ...);
				return *this;
			}
			template<typename... Args>
			decltype(auto) add_colum(const char* arg, Args&&... args)
			{
				_query.select.emplace_back(arg, std::forward<Args>(args)...);
				return *this;
			}
			
			/// ADD_WHERE
			/// Методы добавляют в запрос одно, или сразу несколько выражений для WHERE:
			/// WHERE expr1 AND|OR|NOT expr2 ...
			template<typename... Args,
				typename = std::enable_if_t<(std::is_same_v<Args, sql::query::expr_where> && ...)>>
				decltype(auto) add_where(Args&&... args)
			{
				(_query.where.emplace_back(std::forward<Args>(args)), ...);
				return *this; 
			}			
			template<typename... Args>
			decltype(auto) add_where(const char* arg, Args&&... args)
			{
				_query.where.emplace_back(arg, std::forward<Args>(args)...);
				return *this;
			}
	
			decltype(auto) build()
			{
				if (!_expr.empty())
					return _expr;

				/// SELECT expression, ...
				std::string expr{ _query.select.empty() ? "SELECT * " : "SELECT " };
				expr += bind(std::cbegin(_query.select), std::cend(_query.select), ' ');
				/// FROM table
				expr += "\nFROM " + std::string{ _query.from.get().first } +
					(_query.where.empty() ? std::string{ send_to_server } : "\nWHERE ");
				/// WHERE expression ...
				expr += bind(std::cbegin(_query.where), std::cend(_query.where),
					send_to_server);
				
				_expr = std::move(expr);
				return _expr;
			}

		private:
			template<typename Iter>
			auto bind(Iter first, Iter last, const char end_section) const -> std::string
			{
				using elem_type = typename std::iterator_traits<decltype(first)>::value_type;
				std::string expr{};
				auto count = std::distance(first, last);

				std::for_each(first, last, [&expr, &count, &end_section, this](const elem_type& elem)
					{
						auto&& [ex, fl] = elem.get();
						expr += ex;
						expr += (--count ? bind(fl) : std::string{ end_section });
					}
				);
				return expr;
			}
			
			auto bind(sql::logic_operator logic) const -> std::string
				try
			{
				std::string expr{ logic() };
				if (expr[0] == sql::character::comma())
					expr += sql::character::space();
				else
					expr = sql::character::space() + expr + sql::character::space();
				return expr;
			}
			catch (...) { throw; }
			
			void swap(sql_select_query& rhs) noexcept
			{
				std::swap(_query.select, rhs._query.select);
				std::swap(_query.from, rhs._query.from);
				std::swap(_query.where, rhs._query.where);
			}

			sql::query::sql_select_query_structure _query{};
			std::string _expr{};
			static constexpr char send_to_server{ ';' };

		public:
			sql_select_query() = default;
			sql_select_query& operator=(const sql_select_query&) = delete;
			sql_select_query& operator=(sql_select_query&&) = delete;

		}; // class sql_select_query

		void swap(sql_select_query& lhs, sql_select_query& rhs) noexcept { lhs.swap(rhs); }

	} // namespace generative
} // namespace patterns

#endif // !SQL_QUERY_BUILDER_H_IN_MY_PROJECT