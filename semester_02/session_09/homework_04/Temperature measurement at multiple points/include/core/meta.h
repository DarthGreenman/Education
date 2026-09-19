/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Метафункции.
 *
 */

#ifndef META_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define META_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include <type_traits.h>

namespace core {
	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template <bool B, typename T = void> struct enable_if {};
		template <typename T> struct enable_if<true, T> { using type = T; };
	} /// !namespace helper
	/// @endcond

	/**
	* @brief Выполняет условную компиляцию.
	*
	* Метафункция использует SFINAE, в частности для условного удаления
	* функций из набора кандидатов на основе признаков типов, что позволяет
	* создавать отдельные перегрузки функций или специализации на основе
	* различных признаков типов.
	*
	* @tparam B логическое значение, определяющее включать или исключать код
	* @tparam T тип, который будет использоваться, если B истинно
	*/
	template <bool B, typename T = void>
	using enable_if_t = typename helper::enable_if<B, T>::type;
	

	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template<bool B, typename T, typename U> struct conditional { using type = T; };
		template<typename T, typename U> struct conditional<false, T, U> { using type = U; };
	} /// !namespace helper
	/// @endcond

	/**
	* @brief Предоставляет тип из двух типов.
	*
	* Метафункция определяет тип как T, если B истинно, в противном случае
	* как U.
	*
	* @tparam B логическое значение, определяющее тип
	* @tparam T тип, который будет использоваться, если B истинно
	* @tparam U тип, который будет использоваться, если B ложно
	*/
	template<bool B, typename T, typename U>
	using conditional_t = typename helper::conditional<B, T, U>::type;


	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template<typename T> struct remove_cv { using type = T; };
		template<typename T> struct remove_cv<T const> { using type = T; };
		template<typename T> struct remove_cv<T volatile> { using type = T; };
		template<typename T> struct remove_cv<T const volatile> { using type = T; };
	} /// !namespace helper
	/// @endcond

	/**
	 * @brief    Предоставляет тип, который аналогичен типу T, за исключением 
	 *           того, что из него удалены cv-квалификаторы.
	 * @tparam T тип, возможно с cv-квалификаторами, или ссылочный
	 */
	template<typename T>
	using remove_cv_t = typename helper::remove_cv<T>::type;


	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		template<typename T> struct remove_reference { using type = T; };
		template<typename T> struct remove_reference<T&> { using type = T; };
		template<typename T> struct remove_reference<T&&> { using type = T; };
	} /// !namespace helper
	/// @endcond

	/**
	 * @brief    Предоставляет тип, на который ссылается T.
	 * @tparam T тип, возможно ссылочный, или с cv-квалификаторами
	 */
	template<typename T>
	using remove_reference_t = typename helper::remove_reference<T>::type;


	/**
	 * @brief    Предоставляет тип, на который ссылается T, с удаленными
	 *           cv-квалификаторами.
	 * @tparam T тип, возможно ссылочный, или с cv-квалификаторами
	 */
	template<typename T>
	using pure_type_t = remove_cv_t<remove_reference_t<T>>;


	/**
	 * @brief 
	 * @tparam T 
	 */
	template<typename T> struct add_rvalue_reference { using type = T&&; };
	template<> struct add_rvalue_reference<void> { using type = void; };


	/* declval не создаёт объект типа T. Он лишь позволяет компилятору 
	 * рассуждать так, как будто объект этого типа существует. Благодаря этому
	 * можно анализировать выражения и определять их типы без необходимости 
	 * конструировать реальные объекты.
	 *
	 * У этой функции нет реализации — есть только объявление. Именно поэтому её
	 * физически нельзя вызвать в выполняемом коде. Если вы попробуете это сделать,
	 * линкер выдаст ошибку undefined reference.
	 *
	 * Напрямую вернуть объект абстрактного класса (или массива) из функции по
	 * значению нельзя. Но вернуть ссылку на него — можно.
	 *
	 * Правило сжатия ссылок (Reference Collapsing). Благодаря возврату T&& и 
	 * правилам свёртки ссылок declval формирует выражение той категории 
	 * значения, которая соответствует шаблонному параметру T.
	 *
	 * Если передать T, функция вернет T&&.
	 * Если передать T&, то T& && сожмется в обычную lvalue-ссылку T&.
	 *
	 * Главные сценарии использования:
	 * Определение типа результата операций над разными шаблонами. 
	 * Классический пример — вычисление общего типа для тернарного оператора или 
	 * математического действия.
	 *
	 *      template <typename T, typename U>
	 *      auto add(T a, U b) -> decltype(std::declval<T>() + std::declval<U>())
	 *      { return a + b; }
	 *
	 * Здесь std::declval позволяет компилятору подставить «мнимые» объекты 
	 * типов T и U, сложить их и определить итоговый тип, даже если у T или 
	 * U нет дефолтных конструкторов.
	 *
	 * Проверка ограничений в SFINAE и Concepts (до C++20) std::declval 
	 * активно применяется для* проверки наличия у класса определенного 
	 * метода или оператора.
	 *
	 *     template <typename T, typename = void>
	 *     struct has_serialize : std::false_type {};
	 *
	 *     template <typename T>
	 *     struct has_serialize<T, std::void_t<decltype(std::declval<T>().serialize())>>
	 *            : std::true_type {};
	 *
	 * Важные нюансы и ограничения:
	 * Используйте std::declval строго внутри decltype, sizeof, noexcept или 
	 * концептов, только в невычисляемом контексте.
	 * Исключение для void: Если передать std::declval<void>(), стандарт 
	 * гарантирует, что типом возврата будет просто void, а не void&& (так как 
	 * ссылок на void не существует).
	 *
	 */

	/**
	 * @brief Преобразует любой тип T (который может быть неполным типом)
	 *        в выражение этого типа
	 * @tparam T
	 */
	template<typename T>
	typename add_rvalue_reference<T>::type declval() noexcept;


	/**
	 * @brief Структура для представления последовательности целых,
	 *        положительных чисел.
	 *
	 * Структура может быть полезна для создания шаблонов, которые требуют 
	 * генерации последовательности индексов на этапе компиляции. Например, 
	 * для создания шаблонов классов и функций, которые должны работать с 
	 * массивами или другими структурами данных, требующими доступа по индексам.
	 *
	 * @tparam Is пакет индексов, который представляет собой последовательность
	 *            целых, положительных чисел от 0 до N-1, где N - это количество
	 *            элементов в структуре данных, с которой работает шаблон.
	 */
	template <auto... Is>
	struct index_sequence {
		static_assert((... && (is_integral_v<decltype(Is)>)),
			"The indices in Index_sequence must be integers and positive numbers.");
		using type = index_sequence;
	};

	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		/* 
		 * Шаблон для генерации последовательности.
		 * Шаблон работает путем рекурсивного вызова самого себя, уменьшая 
		 * значение N на каждом этапе и добавляя текущий индекс (N-1) к уже
		 * накопленным индексам (Is...). Когда N достигает 0, рекурсия 
		 * останавливается. Результатом является тип index_sequence, содержащий
		 * последовательность целых, положительных чисел  от 0 до N-1.
		 *
		 * @tparam N  количество элементов, которые необходимо добавить
		 * @tparam Is уже имеющаяся последовательность
		 */
		template <auto N, auto... Is>
		struct make_index_sequence_impl : make_index_sequence_impl<N - 1, N - 1, Is...> {};

		/* 
		 * Специализация шаблона для остановки рекурсии.
		 * Когда N достигает 0, тип index_sequence<Is...> будет использован 
		 * как результат генерации последовательности целых, положительных 
		 * чисел  от 0 до N-1.
		 *
		 * @tparam Is последовательности целых, положительных чисел от 0 до N-1,
		 *            которая была накоплена в процессе рекурсии.
		 */
		template <auto... Is>
		struct make_index_sequence_impl<0, Is...> : index_sequence<Is...> {};
	} /// !namespace helper
	/// @endcond

	/**
	 * @brief Генератор последовательности целых, положительных чисел от 0 до N-1.
	 *
	 * Шаблон использует рекурсивную структуру make_index_sequence_impl для 
	 * создания типа index_sequence, который содержит последовательность целых, 
	 * положительных чисел от 0 до N-1. Это может быть полезно для создания 
	 * шаблонов классов и функций, которые должны работать с массивами или
	 * другими структурами данных, требующими доступа по индексам.
	 *
	 * @tparam N количество элементов, которые нужно сгенерировать, обычно 
	 *           соответствует количеству элементов в структуре данных, с 
	 *           которой работает шаблон. Например, если N равно 5,то 
	 *           результатом будет тип index_sequence<0, 1, 2, 3, 4>.
	 */
	template <auto N>
	using make_index_sequence = typename helper::make_index_sequence_impl<N>::type;


	/**
	 * @brief       Структура для представления пакета параметров.
	 * @tparam Args пакет параметров, который может содержать произвольное 
	 *              количество типов
	 */
	template <typename... Args>
	struct package_of_types {};


	/**
	 * @brief     Структура для представления пакета значений (параметров 
	 *            не типов).
	 * @tparam Is пакет параметров, который может содержать произвольное 
	 *            количество значений.
	 */
	template <auto... Is>
	struct package_of_values {};

	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		/*
		 * Структура для распаковки параметров.
		 * @tparam T пакет параметров
		 */
		struct expand_parameter_package {
			template <typename... T> constexpr expand_parameter_package(T...) {}
		};
	} // !namespace helper
	/// @endcond


	/// @cond HIDE_ENUM_BRIEF
	namespace helper {
		/*
		 * Проверяет уникальность типов в списке. Пустой список, или список из 
		 * одного типа считаются уникальными.
		 */
		template<typename... Ts>
		struct is_unique_types : true_type {};

		/*
		 * Head сравнивается с каждым типом из Tail. После этого рекурсивно 
		 * проверяется уникальность самого Tail. Список типов является уникальным
		 * только в том случае, если Head не встречается в Tail и все типы Tail 
		 * уникальны.
		 *
		 *             is_unique_types<Head, Tail...>
		 *                        │
		 *                        ├── Head отсутствует в Tail...
		 *                        │
		 *                        └── is_unique_types<Tail...>
		 *                                     ↓
		 *                                    ...
		 *                                     ↓
		 *                             is_unique_types<>
		 *                                     ↓
		 *                                   true
		 */
		template<typename Head, typename... Tail>
		struct is_unique_types<Head, Tail...> {
			/* 
			 * value истинно, если:
			 *   1. Head не встречается в Tail — !(is_same_v<Head, Tail> || ...)
			 *   2. Tail сам по себе уникален — is_unique_types<Tail...>::value
			 * Если Head найден в Tail — false (второе условие не вычисляется
			 * благодаря короткому замыканию &&).
			 */
			static constexpr bool value =
				!(is_same_v<Head, Tail> || ...) && is_unique_types<Tail...>::value;
		};

	} /// !namespace helper
	/// @endcond

	/**
	 * @brief Возвращает значение проверки уникальности списка типов.
	 * 
	 * Если каждый тип встречается в списке не более одного раза, возвращает true,
	 * иначе false.
	 */
	template<typename... Ts>
	inline constexpr bool is_unique_types_v = helper::is_unique_types<Ts...>::value;

} /// !namespace core

#endif /// !defined(META_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)
