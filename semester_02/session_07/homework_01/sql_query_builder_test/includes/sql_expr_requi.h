// sql_expr_requi.h

#ifndef SQL_EXPR_REQUI_H_IN_MY_PROJECT
#define SQL_EXPR_REQUI_H_IN_MY_PROJECT

#include "sql_expr_base.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>

/// SELECT [DISTINCT | DISTINCTROW | ALL] expr, ...
/// expr оператора SELECT предназначено для определения результирующего набора 
/// столбцов, получаемого после вычисления табличного выражения в предложении FROM и группировки в
/// результате GROUP BY. Предложение SELECT реализует операцию проекции, то есть указание 
/// подмножества столбцов из таблиц табличного выражения, а также операцию переименования
/// столбцов и операцию добавления новых вычислимых столбцов.
/// FROM table_references

namespace sql
{
	namespace query
	{
		class expr_requi
		{
		public:
			expr_requi() = default;

			/// Конструктор для выражений:
			/// 1. SELECT COLUM_NAME
			/// 2. FROM TABLE_NAME
			/// Генерирует исключение в случае некорректного значения.
			explicit expr_requi(const char* name)
			{
				if (!helper::check(name))
				{
					throw std::invalid_argument{ "\nException \"Unknown argument in expression\" in line: " +
						std::to_string(__LINE__) + ", file:\n" + std::string{ __FILE__ } + '\n' };
				}
				try {
					std::string temp_expr{ name };
					std::swap(_expr, temp_expr);
				}
				catch (...) { throw; }
			}

			/// Конструктор для выражений:
			/// 1. SELECT COLUM_NAME AS NICKNAME
			/// 2. FROM TABLE_NAME AS NICKNAME
			/// Генерирует исключение в случае некорректного значения.
			explicit expr_requi(const char* name, const char* nickname)
				: expr_requi(name)
			{
				if (!helper::check(nickname))
				{
					throw std::invalid_argument{ "\nException \"Unknown argument in expression\" in line: " +
						std::to_string(__LINE__) + ", file:\n" + std::string{ __FILE__ } + '\n' };
				}

				try {
					std::string temp_expr{ name };
					temp_expr.push_back(sql::character::space());
					temp_expr += sql::keyword::AS;
					temp_expr.push_back(sql::character::space());
					temp_expr += nickname;
					std::swap(_expr, temp_expr);
				}
				catch (...) { throw; }
			}

			expr_requi(const expr_requi& other_expr) : _bind{ other_expr._bind }
			{
				try {
					std::copy(std::cbegin(other_expr._expr), std::cend(other_expr._expr),
						std::back_inserter(_expr));
				}
				catch (...) { throw; }
			}
			expr_requi(expr_requi&& expr) noexcept : expr_requi() { swap(expr); }
			~expr_requi() = default;

			expr_requi& operator=(const expr_requi& other_expr)
			{
				if (this == &other_expr)
					return *this;

				expr_requi temp_expr{ other_expr };
				swap(temp_expr);

				return *this;
			}
			expr_requi& operator=(expr_requi&& other_expr) noexcept
			{
				if (this == &other_expr)
					return *this;

				expr_requi temp_expr{ std::move(other_expr) };
				swap(temp_expr);

				return *this;
			}
			expr_requi& operator+=(const expr_requi& other_expr)
			{
				_expr += other_expr._expr;
				if (!other_expr._bind)
					return *this;

				_expr.push_back(_bind());
				_expr.push_back(sql::character::space());
				return *this;
			}
			void bind(sql::logic_operator<char> logic) { _bind = logic; }
			std::string get() const { return _expr; }

		private:
			void swap(expr_requi& other_expr) noexcept
			{
				std::swap(_expr, other_expr._expr);
				std::swap(_bind, other_expr._bind);
			}

		private:
			std::string _expr{}; // помещаем проверенную часть выражения
			sql::logic_operator<char> _bind{ sql::character::comma }; // помещаем адрес функции, которая вызывается...
		}; // class expr_requi
	} // namespace query
} // namespace sql

#endif // !SQL_EXPR_REQUI_H_IN_MY_PROJECT
