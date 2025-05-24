// sql_expr_where.h

#ifndef SQL_EXPR_WHERE_H_IN_MY_PROJECT
#define SQL_EXPR_WHERE_H_IN_MY_PROJECT

#include "date.h"
#include "sql_expr_base.h"
#include <cstring>
#include <functional>
#include <limits>
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
		class expr_where : public expr_base
		{
		public:
			expr_where() = default;

			explicit expr_where(const char* lhs, const comp_operator& comp, const char* rhs)
				: expr_where(lhs)
			{
				construct(comp, rhs);
			}

			template<typename T, typename = std::enable_if_t<std::is_same_v<T, int>>>
			explicit expr_where(const char* lhs, comp_operator comp, T&& rhs)
				: expr_where(lhs)
			{
				char num[std::numeric_limits<std::size_t>::digits10]{};
				helper::numeric_to_char(num, std::forward<T>(rhs));
				construct(comp, num);
			}

			explicit expr_where(const char* lhs, const comp_operator& comp, const tpsg::date& rhs,
				const logic_operator& logic) : expr_where(lhs, comp, rhs.to_string().c_str(), logic)
			{
			}

			explicit expr_where(const char* lhs, const comp_operator& comp, const char* rhs,
				const logic_operator& logic) : expr_where(lhs, comp, rhs)
			{
				_bind = logic;
			}

			expr_where(const expr_where& expr) : _bind{ expr._bind }
			{
				std::memcpy(_expr, expr._expr, _size);
			}
			expr_where(expr_where&& expr) noexcept : expr_where()
			{
				this->swap(expr);
			}
			virtual ~expr_where() = default;

			expr_where& operator=(const expr_where&) = default;
			expr_where& operator=(expr_where&& expr) noexcept
			{
				if (this != &expr)
				{
					std::memset(_expr, 0, _size);
					_bind = nullptr;
					this->swap(expr);
				}
				return *this;
			}
			std::pair<const char*, sql::logic_operator> get() const	override final
			{
				return std::make_pair(_expr, _bind);
			}

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
					std::memmove(_expr, name, std::strlen(name) + 1);
				}
				catch (...) { throw; }
			}

			void construct(const comp_operator& comp, const char* rhs)
			{
				char expr[_size]{};
				std::swap(expr, _expr); /// std::memmove(_expr, expr, std::strlen(expr) + 1);
				helper::push_back(expr, comp(), std::strlen(comp()));
				helper::push_back(expr, sql::character::single_quote());
				helper::push_back(expr, rhs, std::strlen(rhs));
				helper::push_back(expr, sql::character::single_quote());
				std::swap(_expr, expr); /// std::memmove(_expr, expr, std::strlen(expr) + 1);
			}

			void swap(expr_where& expr) noexcept
			{
				std::swap(_expr, expr._expr);
				std::swap(_bind, expr._bind);
			}

		private:
			static constexpr unsigned _size{ 1024 };
			char _expr[_size]{}; // помещаем проверенную часть выражения
			sql::logic_operator _bind{ nullptr }; // помещаем адрес функции, которая вызывается...

		}; // class expr_where
	} // namespace expr
} // namespace sql

#endif // !SQL_EXPR_WHERE_H_IN_MY_PROJECT