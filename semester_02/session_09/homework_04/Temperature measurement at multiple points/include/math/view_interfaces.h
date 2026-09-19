/// view_interfaces.h

#ifndef VIEW_OF_INTERFACES_H_THE_COMPONENT_LIBRARY_PROJECT
#define VIEW_OF_INTERFACES_H_THE_COMPONENT_LIBRARY_PROJECT

#include <meta.h>
#include <type_traits.h>

namespace math {

	namespace helper {

		template<typename...>
		struct nothing { using type = void; };

	} /// !namespace helper

	/**********************************************************************************************
	* @brief Вспомогательный шаблон для реализации аналога std::void_t в C++17.
	*
	* Независимо от списка переданных типов всегда определяет вложенный тип type = void.
	* Используется исключительно в SFINAE-проверках.
	* @tparam T Список типов, для которых определяется void_t.
	*/
	template<typename... T>
	using void_t = typename helper::nothing<T...>::type;

	namespace helper {

		template<typename T, typename = void>
		struct has_static_mantissa : core::false_type {};
		template<typename T>
		struct has_static_mantissa<T, void_t<decltype(T::mantissa)>> : core::true_type {};

	} /// !namespace helper

	/**********************************************************************************************
	* @brief Проверяет наличие статического члена T::mantissa.
	*
	* Используется для проверки соответствия математическому интерфейсу библиотеки.
	* @tparam T Тип, для которого проверяется наличие статического члена mantissa.
	* @return true, если тип T содержит статический член mantissa, иначе false.
	*/
	template<typename T>
	inline constexpr bool has_static_mantissa_v = helper::has_static_mantissa<T>::value;

	namespace helper {

		template<typename T, typename = void>
		struct has_static_exponent : core::false_type {};
		template<typename T>
		struct has_static_exponent<T, void_t<decltype(T::exponent)>> : core::true_type {};

	} /// !namespace helper

	/**********************************************************************************************
	* @brief Проверяет наличие статического члена T::exponent.
	*
	* Используется для проверки соответствия математическому интерфейсу библиотеки.
	* @tparam T Тип, для которого проверяется наличие статического члена exponent.
	* @return true, если тип T содержит статический член exponent, иначе false.
	*/
	template<typename T>
	inline constexpr bool has_static_exponent_v = helper::has_static_exponent<T>::value;

	namespace helper {

		template<typename T, typename = void>
		struct has_static_antecedent : core::false_type {};
		template<typename T>
		struct has_static_antecedent<T, void_t<decltype(T::antecedent)>> : core::true_type {};

	} /// !namespace helper

	/******************************************************************************************
	* @brief Проверяет наличие статического члена T::antecedent.
	*
	* Используется для проверки соответствия математическому интерфейсу библиотеки.
	* @tparam T Тип, для которого проверяется наличие статического члена antecedent.
	* @return true, если тип T содержит статический член antecedent, иначе false.
	*/
	template<typename T>
	inline constexpr bool has_static_antecedent_v = helper::has_static_antecedent<T>::value;

	namespace helper {

		template<typename T, typename = void>
		struct has_static_consequent : core::false_type {};
		template<typename T>
		struct has_static_consequent<T, void_t<decltype(T::consequent)>> : core::true_type {};

	} /// !namespace helper

	/******************************************************************************************
	* @brief Проверяет наличие статического члена T::consequent.
	*
	* Используется для проверки соответствия математическому интерфейсу библиотеки.
	* @tparam T Тип, для которого проверяется наличие статического члена consequent.
	* @return true, если тип T содержит статический член consequent, иначе false.
	*/
	template<typename T>
	inline constexpr bool has_static_consequent_v = helper::has_static_consequent<T>::value;


	namespace helper {
		/// Специализация для типов, содержащих update(U) с возвращаемым типом R.
		template<typename T, typename U, typename R, typename = void>
		struct has_update : core::false_type {};

		template<typename T, typename U, typename R> struct has_update<
			T, U, R, void_t<decltype(core::declval<T>().update(core::declval<U>()))>
		>
		: core::bool_constant<core::is_same_v<
			decltype(core::declval<T>().update(core::declval<U>())), R>>{};

		/// Специализация для типов, содержащих value() с возвращаемым типом typename T::value_type.
		template<typename T, typename = void>
		struct has_value : core::false_type {};

		/// Специализация для типов, содержащих value().
		template<typename T> struct has_value<T, void_t<decltype(core::declval<const T&>().value())>>
		: core::bool_constant<core::is_same_v<
			decltype(core::declval<const T&>().value()), typename T::value_type>>{};

	} /// !namespace helper

	/**********************************************************************************************
	 * @brief Проверяет, является ли тип T трансформирующим вычислительным узлом для входного типа U.
	 * @tparam T Тип вычислительного узла.
	 * @tparam U Тип входного сигнала.
	 * @return true, если тип T является трансформирующим вычислительным узлом для входного типа U,
	 *         иначе false.
	 */
	template<typename T, typename U>
	inline constexpr bool is_transform_v = helper::has_update<T, U, void>::value&&
		helper::has_value<T>::value;

	/**********************************************************************************************
	 * @brief Проверяет, является ли тип T детектором для входного типа U.
	 * @tparam T Тип вычислительного узла.
	 * @tparam U Тип входного сигнала.
	 * @return true, если тип T является детектором для входного типа U, иначе false.
	 */
	template<typename T, typename U>
	inline constexpr bool is_detector_v = helper::has_update<T, U, bool>::value&&
		helper::has_value<T>::value;

} /// !namespace math

#endif // !defined(VIEW_OF_INTERFACES_H_THE_COMPONENT_LIBRARY_PROJECT)

