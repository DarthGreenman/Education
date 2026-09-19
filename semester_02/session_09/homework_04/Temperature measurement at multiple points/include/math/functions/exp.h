/// exp.h

#ifndef EXPONENTIATION_H_THE_COMPONENT_LIBRARY_PROJECT
#define EXPONENTIATION_H_THE_COMPONENT_LIBRARY_PROJECT

#include <abs.h>
#include <log.h>
#include <numeric_limits.h>
#include <type_traits.h>

namespace math {

	namespace helper {
		/******************************************************************************************
		 * @brief Вычисляет значение B^E на этапе компиляции.
		 *
		 * Структура предназначена исключительно для целочисленных оснований и неотрицательных
		 * показателей степени. Перед вычислением выполняется проверка корректности входных
		 * параметров и гарантируется отсутствие переполнения результирующего значения.
		 *
		 * Если вычисление невозможно (например, из-за переполнения), компиляция завершается с
		 * поясняющим сообщением.
		 */
		template<auto B, auto E>
		struct exp {
			using base_type = decltype(B);
			static_assert(core::is_arithmetic_integer_v<base_type>, "Base must be an integer type.");
			static_assert(core::is_arithmetic_integer_v<decltype(E)>, "Exponent must be an integer type.");
			/// Нулевое основание запрещено.
			/// В библиотеке отсутствует необходимость поддерживать выражение 0^E, поэтому такой 
			/// случай считается ошибкой проектирования.
			static_assert(B != 0, "Base must not be zero.");
			/// Поддерживаются только неотрицательные показатели степени.
			/// Отрицательная степень требует представления дробных значений,
			/// тогда как данная метафункция работает исключительно с целыми числами.
			static_assert(E >= 0, "Exponent must be a positive number.");
			/**
			 * Проверка переполнения.
			 *
			 * Вместо непосредственного вычисления степени определяется максимально допустимый
			 * показатель степени для данного основания:
			 *
			 *      E <= log_B(limit)
			 *
			 * где limit — наибольшее по модулю значение, которое способен хранить
			 * тип основания.
			 *
			 * Такой подход позволяет обнаружить переполнение ещё до начала вычисления степени и
			 * избежать выполнения потенциально некорректных арифметических операций.
			 */
			static_assert(!([]() {
				if constexpr(B == 1 || B == -1) {
					return false;
				} else {
					constexpr auto limit = []() noexcept {
						/**
						 * Определение максимально допустимого значения результата.
						 *
						 * Для отрицательного основания возможен отрицательный результат, если
						 * показатель степени нечётный.
						 *
						 * В этом случае допустимой границей становится абсолютное значение
						 * минимального числа типа, поскольку диапазон отрицательных значений у
						 * знаковых типов обычно больше диапазона положительных на единицу.
						 *
						 * Во всех остальных случаях используется максимальное положительное
						 * значение типа.
						 */
						if constexpr(core::is_signed_v<base_type> && B < 0 && (E % 2 != 0))
							return abs_v<(core::numeric_limits<base_type>::min)()>;
						else
							return static_cast<
							decltype(abs_v<(core::numeric_limits<base_type>::min)()>)
							>((core::numeric_limits<base_type>::max)());
					}();
					return E > (log_v<abs_v<B>, limit>);
				}
			}()), "Overflow detected: The resulting value is too large for the base type.");
			/**************************************************************************************
			 * Непосредственное вычисление степени.
			 *
			 * После успешного прохождения всех проверок переполнение исключено, поэтому достаточно
			 * обычного последовательного умножения.
			 *
			 * Специальные случаи B = ±1 и E = 0 обрабатываются отдельно, что позволяет избежать
			 * лишних итераций.
			 */
			static constexpr auto value = []() noexcept {
				if(E == 0 || B == 1) return static_cast<base_type>(1);
				if(B == -1) return (E % 2 == 0) ? static_cast<base_type>(1)
					: static_cast<base_type>(-1);

				base_type num{ 1 };
				for(decltype(E) i = 0; i < E; ++i) { num *= B; }
				return num;
			}();
		};

	} /// !namespace helper

	template<auto B, auto E>
	inline constexpr auto exp_v = helper::exp<B, E>::value;


	namespace helper {

		template<auto N>
		struct number_is_power_of_two {
			static_assert(core::is_arithmetic_integer_v<decltype(N)>, "Only integer types are supported.");
			static_assert(N > 0, "N must be greater than zero.");

			static constexpr bool value{ (N != 0) && !(N & (N - 1)) };
		};

	} /// !namespace helper

	/**********************************************************************************************
	 * @brief Проверяет, является ли число степенью двойки.
	 *
	 * Число является степенью двойки, если в его двоичном представлении установлен только один
	 * бит.
	 * Проверка выполняется с использованием свойства:
	 *     N & (N - 1) == 0
	 *
	 * Например:
	 *     N = 8
	 *
	 *     00001000
	 *     00000111
	 *     --------
	 *     00000000
	 *
	 * Поэтому:
	 *     number_is_power_of_two<8>::value == true
	 *
	 * Поддерживаются только целочисленные типы с шириной представления менее 64 бит.
	 * Ограничение связано с использованием свойства числовых диапазонов библиотеки и отсутствием
	 * расширенного типа для безопасной обработки 64-битных значений.
	 *
	 * @tparam N Целочисленная константа времени компиляции.
	 */
	template<auto N>
	inline constexpr bool number_is_power_of_two_v = helper::number_is_power_of_two<N>::value;

} /// !namespace math

#endif /// !defined(EXPONENTIATION_H_THE_COMPONENT_LIBRARY_PROJECT)

