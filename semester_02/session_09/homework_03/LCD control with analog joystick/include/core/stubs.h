/// stubs.h

#ifndef STUBS_H_FOR_THE_ARDUINO_UNO_PROJECT
#define STUBS_H_FOR_THE_ARDUINO_UNO_PROJECT

#ifdef _ARDUINO_STUBS

#include <cstdint>
#include <iostream>

/// Глобальные переменные для симуляции рантайма в тестах
inline uint32_t simulated_millis{};
inline int16_t  simulated_digital_pins[20]{ 0 };

/// Константы для симуляции рантайма в тестах
constexpr uint8_t INPUT{ 0x0 };
constexpr uint8_t INPUT_PULLUP{ 0x2 };
constexpr uint8_t OUTPUT{ 0x1 };
constexpr uint8_t DEFAULT{};

/// Функции-стабы для симуляции рантайма в тестах

/// Функции-стабы для симуляции настройки режимов пинов в тестах
inline void analogReference(uint8_t mode) {
	std::cout << __FUNCTION__ << " mode = " << static_cast<int>(mode) << '\n';
}
inline void pinMode(uint8_t pin_number, uint8_t signal_mode) {
	std::cout << __FUNCTION__ << " pin_number = " << static_cast<int>(pin_number)
		<< ", mode = " << (signal_mode == OUTPUT
			? "OUTPUT" : signal_mode == INPUT
			? "INPUT" : signal_mode == INPUT_PULLUP ? "INPUT_PULLUP" : "DEFAULT")
		<< '\n';
	/// Аппаратная эмуляция подтяжки: если пин настроен как INPUT_PULLUP, 
	/// выставляем на нем по умолчанию HIGH, как это делает настоящий контроллер
	if(signal_mode == INPUT_PULLUP) {
		simulated_digital_pins[pin_number] = 1; // HIGH
	}
}
/// Функции-стабы для симуляции записи сигналов в тестах
inline void analogWrite(uint8_t pin_number, int signal_value) {
	simulated_digital_pins[pin_number] = static_cast<int16_t>(signal_value);
	std::cout << __FUNCTION__ << " pin_number = " << static_cast<int>(pin_number)
		<< ",signal_value = " << signal_value << '\n';
}
inline void digitalWrite(uint8_t pin_number, int signal_value) {
	simulated_digital_pins[pin_number] = static_cast<int16_t>(signal_value);
	std::cout << __FUNCTION__ << " pin_number = " << static_cast<int>(pin_number)
		<< ", signal_value = " << signal_value << '\n';
}
/// Функции-стабы для симуляции чтения сигналов в тестах
inline int analogRead(uint8_t pin_number) {
	const auto signal_value = simulated_digital_pins[pin_number];
	std::cout << __FUNCTION__ << " pin_number = " << static_cast<int>(pin_number)
		<< ", signal_value = " << static_cast<int>(signal_value) << '\n';
	return signal_value;
}
inline uint8_t digitalRead(uint8_t pin_number) {
	const auto signal_value = simulated_digital_pins[pin_number];
	std::cout << __FUNCTION__ << " pin_number = " << static_cast<int>(pin_number)
		<< ", signal_value = " << static_cast<int>(signal_value) << '\n';
	return static_cast<uint8_t>(signal_value);
}
/// Функции-стабы для симуляции работы с таймерами в тестах
inline uint32_t millis() { return simulated_millis; }
inline void delay(uint32_t ms) { simulated_millis += ms; }
inline void delayMicroseconds(uint32_t) { ; }

#endif /// !_ARDUINO_STUBS

#endif /// !STUBS_H_FOR_THE_ARDUINO_UNO_PROJECT

