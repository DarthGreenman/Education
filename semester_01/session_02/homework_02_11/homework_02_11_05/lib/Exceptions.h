/* Exceptions.h */
/*
* Базовый класс Errors_data (производный от std::logic_error) - объединяет
* все исключения которые генерируются при возникновении ошибки создания,
* переноса, вращения, масштабирования геометрии. Так же базовый класс
* однозначно определят типы Bad_value и Bad_data в зоне видимости
* geo::Errors_data, что исключает коллизия имен.
* Исключение Errors_data определяет ошибки значений, например: < 0; > 180; > 360 и т.п.
* Исключение Bad_data определяет ошибки набора значений, например:
* сумма углов != 180 (360); длина стороны >= суммы остальных сторон.
*/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

namespace geo {
	class Errors_data
		: public std::logic_error {
	public:
		Errors_data(const std::string& message) :
			std::logic_error(message) {}

		~Errors_data() {}

		const char* what() const noexcept override {
			return std::exception::what();
		}
	};

	class Bad_value
		: public Errors_data {
	public:
		Bad_value(const std::string& message) :
			Errors_data(message) {}

		~Bad_value() {}

		const char* what() const noexcept override {
			return Errors_data::what();
		}
	};

	class Bad_data
		: public Errors_data {
	public:
		Bad_data(const std::string& message) :
			Errors_data(message) {}

		~Bad_data() {}

		const char* what() const noexcept override {
			return Errors_data::what();
		}
	};
}
#endif /* EXCEPTIONS_H */