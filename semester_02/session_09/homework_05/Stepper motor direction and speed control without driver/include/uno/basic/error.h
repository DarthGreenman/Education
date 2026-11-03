/**
 * @file
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2026 Dmitriy Kostuychenko
 *
 * brief Коды ошибок для проекта Arduino Uno.
 * 
 * Ошибки, которые могут возникнуть в процессе работы Arduino Uno. Используется
 * для обработки ошибок, логирования и сигнализации о сбоях.
 * 
 */

#ifndef ERROR_H_FOR_THE_ARDUINO_UNO_PROJECT
#define ERROR_H_FOR_THE_ARDUINO_UNO_PROJECT

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace uno {
	enum class error_code : uint8_t {
		success = 0,
		open_circuit, /**< Ошибка: разрыв цепи. */
		short_circuit /**< Ошибка : короткое замыкание. */
	};
} /// !namespace uno

#endif // !ERROR_H_FOR_THE_ARDUINO_UNO_PROJECT

