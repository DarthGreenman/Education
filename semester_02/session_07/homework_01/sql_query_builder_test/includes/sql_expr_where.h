// sql_expr_where.h

#ifndef SQL_EXPR_WHERE_H_IN_MY_PROJECT
#define SQL_EXPR_WHERE_H_IN_MY_PROJECT

#include "date.h"
#include "sql_expr_base.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

/// WHERE expr
/// 1. WHERE зарезервированное слово языка SQL. Раздел WHERE используется совместно с DML операторами.
/// 2. expr (expr_pred) используется для определения, какие строки должны быть выбраны из табличного выражения
///    в предложении FROM.
/// 
/// SELECT EmployeeKey, LastName
/// FROM DimEmployee
/// Нахождение строки с помощью простого равенства
/// WHERE LastName = 'Smith';
///
/// Нахождение строк, содержащих значение как часть строки
/// WHERE LastName LIKE('%Smi%'); 
///
/// Нахождение строк с использованием оператора сравнения
/// WHERE EmployeeKey <= 500; 
///
/// Нахождение строк, удовлетворяющих любому из трех условий
/// WHERE EmployeeKey = 1 OR EmployeeKey = 8 OR EmployeeKey = 12;
///
/// Нахождение строк, которые должны удовлетворять нескольким условиям
/// WHERE EmployeeKey <= 500 AND LastName LIKE '%Smi%' AND FirstName LIKE '%A%';
///
/// Нахождение строк, находящихся в списке значений
/// WHERE LastName IN('Smith', 'Godfrey', 'Johnson');
///
/// Нахождение строк, содержащих значение, расположенное между двумя значениями
/// WHERE EmployeeKey Between 100 AND 200;

namespace sql
{
	namespace query
	{
		class expr_where
		{
		public:
			expr_where() = default;

			explicit expr_where(const char* lhs, const compression_operator& select, const char* rhs)
				: expr_where(lhs)
			{
				try 	{
					construct(select, rhs);
				}
				catch (...) { throw; }
			}

			template<typename T, typename = std::enable_if_t<std::is_integral_v<T> ||
				std::is_floating_point_v<T>>>
				explicit expr_where(const char* lhs, compression_operator comp, T&& rhs)
				: expr_where(lhs)
			{
				try {
					construct(comp, std::to_string(rhs));
				}
				catch (...) { throw; }
			}

			explicit expr_where(const char* lhs, const compression_operator& comp, const tpsg::date& rhs,
				const logical_operator<const char*>& logic) : expr_where(lhs, comp, rhs.to_string().c_str(), logic)
			{
			}

			explicit expr_where(const char* lhs, const compression_operator& comp, const char* rhs,
				const logical_operator<const char*>& logic) : expr_where(lhs, comp, rhs)
			{
				_bind = logic;
			}

			expr_where(const expr_where& other_expr) : _bind{ other_expr._bind }
			{
				try {
					std::copy(std::cbegin(other_expr._expr), std::cend(other_expr._expr),
						std::back_inserter(_expr));
				}
				catch (...) { throw; }
			}
			expr_where(expr_where&& other_expr) noexcept : expr_where() { swap(other_expr); }

			~expr_where() = default;

			expr_where& operator=(const expr_where& other_expr)
			{
				if (this == &other_expr)
					return *this;

				expr_where temp_expr{ other_expr };
				swap(temp_expr);

				return *this;
			}
			expr_where& operator=(expr_where&& other_expr) noexcept
			{
				if (this == &other_expr)
					return *this;

				expr_where temp_expr{ std::move(other_expr) };
				swap(temp_expr);

				return *this;
			}
			expr_where& operator+=(const expr_where& other_expr)
			{
				_expr += other_expr._expr;
				if (!other_expr._bind)
					return *this;

				_expr.push_back(sql::character::space());
				_expr += other_expr._bind();
				_expr.push_back(sql::character::space());

				return *this;
			}
			auto bind(sql::logical_operator<const char*> logic) -> void { _bind = logic; }
			auto get() const -> std::string { return _expr; }

		private:
			/// Конструктор для выражений:
			/// 1. SELECT COLUM_NAME
			/// 2. FROM TABLE_NAME
			/// Генерирует исключение в случае некорректного значения.  При генерации исключения 
			/// деструктор не вызывается.
			explicit expr_where(const char* name)
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

			auto construct(const compression_operator& select, const std::string& rhs) -> void
			{
				try
				{
					std::string temp_expr{ _expr };
					temp_expr += select();
					temp_expr.push_back(sql::character::single_quote());
					temp_expr += rhs;
					temp_expr.push_back(sql::character::single_quote());
					std::swap(_expr, temp_expr);
				}
				catch (...) { throw; }
			}

			auto swap(expr_where& other_expr) noexcept -> void
			{
				std::swap(_expr, other_expr._expr);
				std::swap(_bind, other_expr._bind);
			}

		private:
			std::string _expr{}; // помещаем проверенную часть выражения
			sql::logical_operator<const char*> _bind{}; // помещаем адрес функции, которая вызывается...
		}; // class expr_where
	} // namespace expr
} // namespace sql

#endif // !SQL_EXPR_WHERE_H_IN_MY_PROJECT