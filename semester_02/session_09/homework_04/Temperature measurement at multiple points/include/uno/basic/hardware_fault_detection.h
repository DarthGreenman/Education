/// hardware_fault_detection.h

#ifndef HARDWARE_FAULT_DETECTION_H_FOR_THE_ARDUINO_UNO_PROJECT
#define HARDWARE_FAULT_DETECTION_H_FOR_THE_ARDUINO_UNO_PROJECT

#include <common.h>
#include <error.h>

namespace uno {

	/**********************************************************************************************
	 * @brief Класс обнаружения аппаратных сбоев.
	 *
	 * Предназначен для обнаружения аппаратных сбоев в системе Arduino Uno.
	 * Использует различные методы проверки состояния системы и сигнализирует о сбое.
	 */
	struct hardware_fault_detection {
		/// TODO:
		/// Реализовать аппаратное обнаружение неисправностей для фактической электрической схемы датчика.
		/// Текущая реализация демонстрирует обнаружение короткого замыкания и обрыва цепи по крайним
		/// значениям сигнала.
		template<typename T>
		[[nodiscard]] FORCEINLINE static error_code check(T value, T short_circuit_threshold,
			T open_circuit_threshold) {
			if(value == short_circuit_threshold)
				return error_code::short_circuit;
			if(value == open_circuit_threshold)
				return error_code::open_circuit;

			return error_code::success;
		}
	};

} /// !namespace uno

#endif /// !defined(HARDWARE_FAULT_DETECTION_H_FOR_THE_ARDUINO_UNO_PROJECT)

