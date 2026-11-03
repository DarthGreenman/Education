/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * @brief Контейнер.
 *
 */

#ifndef ARRAY_H_FOR_THE_COMPONENT_LIBRARY_PROJECT
#define ARRAY_H_FOR_THE_COMPONENT_LIBRARY_PROJECT

#include <meta.h>
#include <type_traits.h>

namespace core { 
	/** 
	 * @brief Последовательность элементов фиксированного размера.
	 *
	 * Данные последовательности расположены непрерывно. Внутреннее представление
	 * контейнера является массивом С, публичный интерфейс предоставляет методы
	 * для совместимости с алгоритмами стандартной библиотеки.
	 */
	template<typename T, auto N>
	class array {
		static_assert(is_same_v<T, remove_cv_t<T>>, "Array element type must be a non-const, "
			"non-volatile type.");
		static_assert(is_integral_v<decltype(N)> && !is_same_v<decltype(N), bool> &&
			!is_same_v<decltype(N), char>, "The size must be specified using an integer type, "
			"except for bool and char.");
		static_assert(N > 0, "Array size must be greater than zero.");

	public:
		using value_type = T;
		using size_type = decltype(N);
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using iterator = pointer;
		using const_iterator = const_pointer;

		constexpr array() = default;
		template<typename... Args, typename = enable_if_t<(is_same_v<Args, T> && ...)>>
		constexpr array(Args... args) : _data{ args... } {}

		array(const array&) = default;
		array& operator=(const array&) = default;
		array(array&&) = default;
		array& operator=(array&&) = default;

		~array() = default;

		/// Доступ к элементу
		constexpr const_reference operator[](size_type index) const noexcept { return _data[index]; }
		constexpr reference operator[](size_type index) noexcept {
			return const_cast<reference>(static_cast<const array&>(*this)[index]);
		}

		constexpr const_reference front() const noexcept { return _data[0]; }
		constexpr reference front() noexcept {
			return const_cast<reference>(static_cast<const array&>(*this).front());
		}

		constexpr const_reference back() const noexcept { return _data[N - 1]; }
		constexpr reference back() noexcept {
			return const_cast<reference>(static_cast<const array&>(*this).back());
		}

		constexpr const_pointer data() const noexcept { return _data; }
		constexpr pointer data() noexcept {
			return const_cast<pointer>(static_cast<const array&>(*this).data());
		}

		/// Итераторы
		constexpr const_iterator begin() const noexcept { return data(); }
		constexpr const_iterator cbegin() const noexcept { return begin(); }
		constexpr iterator begin() noexcept {
			return const_cast<iterator>(static_cast<const array&>(*this).begin());
		}

		constexpr const_iterator end() const noexcept { return begin() + N; }
		constexpr const_iterator cend() const noexcept { return end(); }
		constexpr iterator end() noexcept {
			return const_cast<iterator>(static_cast<const array&>(*this).end());
		}

		/// Размер
		constexpr size_type size() const noexcept { return N; }

	private:
		T _data[N]{};
	};

} /// !namespace core

#endif /// !defined(ARRAY_H_FOR_THE_COMPONENT_LIBRARY_PROJECT)
