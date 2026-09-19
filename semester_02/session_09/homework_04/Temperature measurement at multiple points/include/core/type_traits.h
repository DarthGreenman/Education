/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Свойтсва фундаментальных типов.
 * 
 */

#ifndef TYPE_TRAITS_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define TYPE_TRAITS_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

namespace core {

	template<bool B>
	struct bool_constant { static constexpr bool value = B;	};

	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;

	template<bool B>
	inline constexpr bool bool_constant_v = bool_constant<B>::value;

	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template<typename T>
		struct is_void { static constexpr bool value = false; };
		template<> struct is_void<void> { static constexpr bool value = true; };
		template<> struct is_void<const void> { static constexpr bool value = true; };
		template<> struct is_void<volatile void> { static constexpr bool value = true; };
		template<> struct is_void<const volatile void> { static constexpr bool value = true; };
	} /// !namespace helper
	/// @endcond

	template<typename T>
	constexpr bool is_void_v = helper::is_void<T>::value;
	
	
	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template<typename T> struct is_integral { static constexpr bool value{}; };

		template<> struct is_integral<bool> { static constexpr bool value{ true }; };
		template<> struct is_integral<char> { static constexpr bool value{ true }; };

		template<> struct is_integral<signed char> { static constexpr bool value{ true }; };
		template<> struct is_integral<short int> { static constexpr bool value{ true }; };
		template<> struct is_integral<int> { static constexpr bool value{ true }; };
		template<> struct is_integral<long int> { static constexpr bool value{ true }; };
		template<> struct is_integral<long long int> { static constexpr bool value{ true }; };

		template<> struct is_integral<unsigned char> { static constexpr bool value{ true }; };
		template<> struct is_integral<unsigned short int> { static constexpr bool value{ true }; };
		template<> struct is_integral<unsigned int> { static constexpr bool value{ true }; };
		template<> struct is_integral<unsigned long int> { static constexpr bool value{ true }; };
		template<> struct is_integral<unsigned long long int> { static constexpr bool value{ true }; };
	} /// !namespace helper
	/// @endcond

	/**
	* @brief Содержит результат проверки, является ли тип T целым числом.
	*
	* Это может быть полезно для создания шаблонов классов и функций, которые 
	* должны работать только с целыми числами, обеспечивая тем самым более 
	* гибкую и оптимизированную реализацию. 
	* Например, если T является int, то is_integral_v будет истинным, а если T 
	* является float, то is_integral_v будет ложным.
	*
	* @tparam T тип, который нужно проверить на соответствие целому числу
	*/
	template<typename T>
	inline constexpr bool is_integral_v = helper::is_integral<T>::value;


	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template<typename T> struct is_signed { static constexpr bool value{}; };

		template<> struct is_signed<signed char> { static constexpr bool value{ true }; };
		template<> struct is_signed<short int> { static constexpr bool value{ true }; };
		template<> struct is_signed<int> { static constexpr bool value{ true }; };
		template<> struct is_signed<long int> { static constexpr bool value{ true }; };
		template<> struct is_signed<long long int> { static constexpr bool value{ true }; };
	} /// !namespace helper
	/// @endcond

	/**
	 * @brief Содержит результат проверки, является ли тип T знаковым целым числом.
	 *
	 * Это может быть полезно для создания шаблонов классов и функций, которые 
	 * должны работать только с знаковыми целыми числами, обеспечивая тем самым 
	 * более гибкую и оптимизированную реализацию.
	 * Например, если T является int, то is_signed_v будет истинным, а если T 
	 * является unsigned int, то is_signed_v будет ложным.
	 *
	 * @tparam T тип, который нужно проверить на соответствие знаковому
	 *           целому числу
	 */
	template<typename T>
	inline constexpr bool is_signed_v = helper::is_signed<T>::value;


	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template<typename T> struct is_unsigned { static constexpr bool value{}; };

		template<> struct is_unsigned<unsigned char> { static constexpr bool value{ true }; };
		template<> struct is_unsigned<unsigned short int> { static constexpr bool value{ true }; };
		template<> struct is_unsigned<unsigned int> { static constexpr bool value{ true }; };
		template<> struct is_unsigned<unsigned long int> { static constexpr bool value{ true }; };
		template<> struct is_unsigned<unsigned long long int> { static constexpr bool value{ true }; };
	} /// !namespace helper
	/// @endcond

	/**
	 * @brief Содержит результат проверки, является ли тип T беззнаковым 
	 *        целым числом.
	 *
	 * Это может быть полезно для создания шаблонов классов и функций, которые
	 * должны работать только с беззнаковыми целыми числами, обеспечивая тем 
	 * самым более гибкую и оптимизированную реализацию.
	 * Например, если T является unsigned int, то is_unsigned_v будет истинным,
	 * а если T является int, то is_unsigned_v будет ложным.
	 *
	 * @tparam T тип, который нужно проверить на соответствие беззнаковому
	 *           целому числу
	 */
	template<typename T>
	inline constexpr bool is_unsigned_v = helper::is_unsigned<T>::value;


	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template <typename T, typename U> struct is_same;
		template <typename T> struct is_same<T, T> { static constexpr bool value{ true }; };
		template <typename T, typename U> struct is_same { static constexpr bool value{}; };
	} /// !namespace helper
	/// @endcond

	/**
	 * @brief Содержит результат проверки, являются ли два типа одинаковыми.
	 *
	 * Это может быть полезно для создания шаблонов, которые требуют проверки
	 * типов на этапе компиляции, например, для создания условных конструкций
	 * или для обеспечения безопасности типов в шаблонных функциях и классах.
	 *
	 * @tparam T первый тип для сравнения
	 * @tparam U второй тип для сравнения
	 */
	template <typename T, typename U>
	inline constexpr bool is_same_v = helper::is_same<T, U>::value;


	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template<typename T>
		struct is_arithmetic_integer : bool_constant<
			is_integral_v<T> &&
			!core::is_same_v<T, bool> &&
			!core::is_same_v<T, char>
		> {};
	} /// !namespace helper
	/// @endcond

	/**
	 * @brief Содержит результат проверки, являются ли тип арифметическим
	 *        целым типом.
	 * 
	 * Это может быть полезно для создания шаблонов, которые требуют проверки
	 * типов на этапе компиляции, например, для создания условных конструкций
	 * или для обеспечения безопасности типов в шаблонных функциях и классах.
	 *
	 * @tparam T тип, который нужно проверить на соответствие арифметическому
	 *        целому типу
	 */
	template<typename T>
	inline constexpr bool is_arithmetic_integer_v = helper::is_arithmetic_integer<T>::value;


	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template<typename T> struct is_floating_point { static constexpr bool value{}; };

		template<> struct is_floating_point<float> { static constexpr bool value{ true }; };
		template<> struct is_floating_point<double> { static constexpr bool value{ true }; };
	} /// !namespace helper
	/// @endcond

	/**
	 * @brief 
	 * @tparam T 
	 */
	template<typename T>
	inline constexpr bool is_floating_point_v = helper::is_floating_point<T>::value;

	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template<typename T> struct make_unsigned;

		template<> struct make_unsigned<signed char> { using type = unsigned char; };
		template<> struct make_unsigned<short int> { using type = unsigned short int; };
		template<> struct make_unsigned<int> { using type = unsigned int; };
		template<> struct make_unsigned<long int> { using type = unsigned long int; };
		template<> struct make_unsigned<long long int> { using type = unsigned long long int; };

		template<> struct make_unsigned<unsigned char> { using type = unsigned char; };
		template<> struct make_unsigned<unsigned short int> { using type = unsigned short int; };
		template<> struct make_unsigned<unsigned int> { using type = unsigned int; };
		template<> struct make_unsigned<unsigned long int> { using type = unsigned long int; };
		template<> struct make_unsigned<unsigned long long int> { using type = unsigned long long int; };
	} /// !namespace helper
	/// @endcond

	/**
	 * @brief Предоставляет беззнаковый тип, соответствующего типу T.
	 *
	 * Для каждого фундаментального целочисленного типа предоставляет 
	 * соответствующий беззнаковый тип с той же шириной представления.
	 * Для беззнакового типа результатом является сам тип.
	 *
	 * @tparam T фундаментальный целочисленный тип
	 */
	template<typename T>
	using make_unsigned_t = typename helper::make_unsigned<T>::type;
	
	
	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template<typename T> struct make_signed;

		template<> struct make_signed<signed char> { using type = signed char; };
		template<> struct make_signed<short int> { using type = short int; };
		template<> struct make_signed<int> { using type = int; };
		template<> struct make_signed<long int> { using type = long int; };
		template<> struct make_signed<long long int> { using type = long long int; };

		template<> struct make_signed<unsigned char> { using type = signed char; };
		template<> struct make_signed<unsigned short int> { using type = short int; };
		template<> struct make_signed<unsigned int> { using type = int; };
		template<> struct make_signed<unsigned long int> { using type = long int; };
		template<> struct make_signed<unsigned long long int> { using type = long long int; };
	} /// !namespace helper
	/// @endcond

	/**
	 * @brief Предоставляет знаковый тип, соответствующего типу T.
	 *
	 * Для каждого фундаментального целочисленного типа предоставляет
	 * соответствующий знаковый тип с той же шириной представления.
	 * Для знакового типа результатом является сам тип.
	 *
	 * @tparam T фундаментальный целочисленный тип
	 */
	template<typename T>
	using make_signed_t = typename helper::make_signed<T>::type;

} ///! namespace core

#endif /// !defined(TYPE_TRAITS_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)	

