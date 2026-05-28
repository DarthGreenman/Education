// arduino_stubs.h

#ifndef ARDUINO_STUBS_H
#define ARDUINO_STUBS_H

#include "signal.h"

#include <cstdint>
#include <iostream>

constexpr uint8_t INPUT{ 0x0 };
constexpr uint8_t INPUT_PULLUP{ 0x2 };
constexpr uint8_t OUTPUT{ 0x1 };
constexpr uint8_t DEFAULT{};

static void analogReference(uint8_t mode) {
	std::cout << __FUNCTION__ << " mode = " << static_cast<int>(mode) << '\n';
}
static void analogWrite(uint8_t pin_number, int signal_value) {
	std::cout << __FUNCTION__ << " pin_number = " << static_cast<int>(pin_number)
		<< ", signal_value = " << signal_value << '\n';
}
static void digitalWrite(uint8_t pin_number, int signal_value) {
	std::cout << __FUNCTION__ << " pin_number = " << static_cast<int>(pin_number)
		<< ", signal_value = " << signal_value << '\n';
}
static int analogRead(uint8_t pin_number) {
	std::cout << __FUNCTION__ << " pin_number = " << static_cast<int>(pin_number)
		<< '\n';
	return 1023;
}
static uint8_t digitalRead(uint8_t pin_number) {
	std::cout << __FUNCTION__ << " pin_number = " << static_cast<int>(pin_number)
		<< '\n';
	return 0;
}
static void pinMode(uint8_t pin_number, uint8_t signal_direction) {
	std::cout << __FUNCTION__ << " pin_number = " << static_cast<int>(pin_number)
		<< ", mode = " << (signal_direction == OUTPUT ? "OUTPUT" : signal_direction == INPUT ? "INPUT" : signal_direction == INPUT_PULLUP ? "INPUT_PULLUP" : "DEFAULT")
		<< '\n';
}
static uint32_t millis() {
	return uint32_t{};
}
static void delay(uint32_t) { ; }

#endif // !ARDUINO_STUBS_H
