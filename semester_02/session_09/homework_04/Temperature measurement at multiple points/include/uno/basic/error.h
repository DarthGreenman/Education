/// error.h

#ifndef ERROR_H_FOR_THE_ARDUINO_UNO_PROJECT
#define ERROR_H_FOR_THE_ARDUINO_UNO_PROJECT

#if defined(_MSC_VER)
#include <cstdint>
#elif defined(__GNUC__) || defined(__clang__)
#include <stdint.h>
#endif

namespace uno {

	/**********************************************************************************************
	*@brief Коды ошибок для проекта Arduino Uno.
	*
	* Определяет коды ошибок, которые могут возникнуть в процессе работы проекта Arduino Uno.
	* Используется для обработки ошибок и сигнализации о сбоях.
	*/
	enum class error_code : uint8_t {
		success = 0,
		open_circuit, /// Ошибка: разрыв цепи
		short_circuit /// Ошибка: короткое замыкание
	};

} /// !namespace uno

#endif // !ERROR_H_FOR_THE_ARDUINO_UNO_PROJECT

