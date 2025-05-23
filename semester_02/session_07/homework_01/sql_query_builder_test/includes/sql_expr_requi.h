// sql_expr_requi.h

#ifndef SQL_EXPR_REQUI_H_IN_MY_PROJECT
#define SQL_EXPR_REQUI_H_IN_MY_PROJECT

#include "sql_expr_base.h"
#include <cstring>
#include <functional>
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
		class expr_requi : public expr_base
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
					std::memmove(_expr, name, std::strlen(name) + 1);
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
				try
				{
					char expr[_size]{};
					std::swap(expr, _expr); /// std::memmove(_expr, expr, std::strlen(expr) + 1);
					helper::insert_back(expr, " AS ", std::strlen(" AS "));
					helper::insert_back(expr, nickname, std::strlen(nickname));
					std::swap(_expr, expr); /// std::memmove(_expr, expr, std::strlen(expr) + 1);
				}
				catch (...) { throw; }
			}

			expr_requi(const expr_requi&) = delete;
			expr_requi(expr_requi&& expr) noexcept : expr_requi()
			{
				this->swap(expr);
			}
			virtual ~expr_requi() = default;

			expr_requi& operator=(const expr_requi&) = delete;
			expr_requi& operator=(expr_requi&& expr) noexcept
			{
				if (this != &expr)
				{
					std::memset(_expr, 0, _size);
					_bind = nullptr;
					this->swap(expr);
				}
				return *this;
			}
			std::pair<const char*, sql::logic_operator> get() const override final
			{
				return std::make_pair(_expr, _bind);
			}

		private:
			void swap(expr_requi& expr) noexcept
			{
				std::swap(_expr, expr._expr);
				std::swap(_bind, expr._bind);
			}

		private:
			/// Длина наименования псевдонима столбца в SQL имеет определённые ограничения 
			/// в зависимости от используемой системы баз данных. В MySQL максимально
			/// допустимый размер для имён баз данных, таблиц, столбцов и индексов — 64 символа,
			/// для псевдонимов — 256 символов.
			static constexpr unsigned _size{ 325 }; /// 64 + " AS " + 256 + '\0'
			char _expr[_size]{}; // помещаем проверенную часть выражения
			sql::logic_operator _bind{ sql::logic::comma }; // помещаем адрес функции, которая вызывается...

		}; // class expr_requi
	} // namespace query
} // namespace sql

#endif // !SQL_EXPR_REQUI_H_IN_MY_PROJECT
