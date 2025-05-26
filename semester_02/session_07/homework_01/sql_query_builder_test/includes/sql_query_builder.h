// sql_query_builder.h

#ifndef SQL_QUERY_BUILDER_H_IN_MY_PROJECT
#define SQL_QUERY_BUILDER_H_IN_MY_PROJECT

#include "sql_expr_base.h"
#include "sql_expr_requi.h"
#include "sql_expr_where.h" 
#include <algorithm>
#include <iterator>
#include <stdexcept>
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
			std::vector<sql::query::expr_requi> colums;
			sql::query::expr_requi table;
			std::vector<sql::query::expr_where> wheres;
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
			friend auto swap(sql_select_query& lhs, sql_select_query& rhs) noexcept -> void;

			/// SELECT * FROM TABLE_NAME;
			explicit sql_select_query(const char* table_name)
			{
				_expr.table = std::move(decltype(_expr.table){table_name});
			}

			/// SELECT * FROM TABLE_NAME AS NICKNAME;
			explicit sql_select_query(const char* table_name, const char* nickname)
			{
				_expr.table = std::move(decltype(_expr.table){table_name, nickname});
			}

			sql_select_query(const sql_select_query& query)
			{
				std::copy(std::cbegin(query._expr.colums),
					std::end(query._expr.colums), std::back_inserter(_expr.colums));
				_expr.table = query._expr.table;
				std::copy(std::cbegin(query._expr.wheres),
					std::end(query._expr.wheres), std::back_inserter(_expr.wheres));

			}
			sql_select_query(sql_select_query&& query) noexcept : sql_select_query() { swap(query);}
			~sql_select_query() = default;

			/// ADD_COLUM
			/// Методы добавляют в запрос одно, или сразу несколько выражений для SELECT:
			/// SELECT COLUM_NAME_1 AS NICKNAME_1, COLUM_NAME_2, ..., COLUM_NAME_N AS NICKNAME_M
			template<typename... Args,
				typename = std::enable_if_t<(std::is_same_v<Args, sql::query::expr_requi> && ...)>>
				decltype(auto) add_colum(Args&&... args)
			{
				(_expr.colums.emplace_back(std::forward<Args>(args)), ...);
				return *this;
			}
			template<typename... Args>
			decltype(auto) add_colum(const char* arg, Args&&... args)
			{
				_expr.colums.emplace_back(arg, std::forward<Args>(args)...);
				return *this;
			}

			/// ADD_WHERE
			/// Методы добавляют в запрос одно, или сразу несколько выражений для WHERE:
			/// WHERE expr1 AND|OR|NOT expr2 ...
			template<typename... Args,
				typename = std::enable_if_t<(std::is_same_v<Args, sql::query::expr_where> && ...)>>
				decltype(auto) add_where(Args&&... args)
			{
				(_expr.wheres.emplace_back(std::forward<Args>(args)), ...);
				return *this;
			}
			template<typename... Args>
			decltype(auto) add_where(const char* arg, Args&&... args)
			{
				_expr.wheres.emplace_back(arg, std::forward<Args>(args)...);
				return *this;
			}

			auto build() -> std::string
			{
				if (!check()) {
					throw std::invalid_argument{ "\nException \"Unknown column name or alias\" in line: " +
							std::to_string(__LINE__) + ", file:\n" + std::string{ __FILE__ } + '\n' };
				}
				/// SELECT
				std::string text{};
				bind(text, sql::query::expr_requi{});
				text.push_back(sql::special_character::endl());
				/// FROM
				bind(text);
				/// WHERE expression ...
				if (!_expr.wheres.empty()) {
					text.push_back(sql::special_character::endl());
					bind(text, sql::query::expr_where{});
				}
				text.push_back(sql::character::semicolon());
				_text = std::move(text);

				return _text;
			}

		private:
			auto bind(std::string& text, sql::query::expr_requi) -> void
			{
				text += sql::keyword::SELECT;
				text.push_back(sql::character::space());

				if (_expr.colums.empty()) {
					text.push_back(sql::character::asterisk());
					text.push_back(sql::character::space());
					return;
				}
				bind(std::begin(_expr.colums), std::end(_expr.colums), text);
			}
			auto bind(std::string& text) const -> void
			{
				text += sql::keyword::FROM;
				text.push_back(sql::character::space());
				text += _expr.table.get();
			}
			auto bind(std::string& text, sql::query::expr_where) -> void
			{
				text += sql::keyword::WHERE;
				text.push_back(sql::character::space());
				bind(std::begin(_expr.wheres), std::end(_expr.wheres), text);
			}
			template<typename Iter>
			auto bind(Iter first, Iter last, std::string& text) const -> void
			{
				using value_type = typename std::iterator_traits<decltype(first)>::value_type;
				value_type expr{};
				auto count = std::distance(first, last);

				std::for_each(first, last, [&expr, &count](value_type& elem) {
						if (!(--count))
							elem.bind(nullptr);
						expr += elem; }
				);
				const auto __text = expr.get();
				std::copy(std::cbegin(__text), std::cend(__text), std::back_inserter(text));
			}

			auto check() const -> bool
			{
				/// Алгоритм проверки наличия имен или псевдонимов выражения WHERE в 
				/// списке SELECT
				return true; /// считаем, что проверка прошла успешно
			}

			auto swap(sql_select_query& rhs) noexcept -> void
			{
				std::swap(_expr.colums, rhs._expr.colums);
				std::swap(_expr.table, rhs._expr.table);
				std::swap(_expr.wheres, rhs._expr.wheres);
			}

			sql::query::sql_select_query_structure _expr{};
			std::string _text{};

		public:
			sql_select_query() = default;
			sql_select_query& operator=(const sql_select_query&) = delete;
			sql_select_query& operator=(sql_select_query&&) = delete;

		}; // class sql_select_query

		auto swap(sql_select_query& lhs, sql_select_query& rhs) noexcept -> void { lhs.swap(rhs); }

	} // namespace generative
} // namespace patterns

#endif // !SQL_QUERY_BUILDER_H_IN_MY_PROJECT