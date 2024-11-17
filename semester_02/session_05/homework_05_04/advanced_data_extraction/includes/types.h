// types.h

#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>
#include <utility>

namespace my
{
	class person_name
	{
	public:
		person_name() = default;
		person_name(const std::string& forename, const std::string& surname);
		person_name(const person_name&) = default;
		person_name(person_name&&) = delete;
		~person_name() = default;

		person_name& operator=(const person_name&) = default;
		person_name& operator=(person_name&&) = delete;

		std::pair<std::string, std::string> get() const;

	private:
		std::string forename_{}, surname_{};
	};

	// буквы, цифры, точки, подчеркивания, дефисы, апострофы и плюсы
	// перевести в нижний регистр

	class email_address
	{
	public:
		email_address() = default;
		email_address(const std::string& address);
		email_address(const email_address&) = default;
		email_address(email_address&&) = default;
		~email_address() = default;

		email_address& operator=(const email_address&) = default;
		email_address& operator=(email_address&&) = default;

		std::string get() const;

	private:
		// mailbox - является пользовательским аккаунтом, приложением или аккаунтом системной роли,
		// может содержать информацию для дальнейшей маршрутизации или идентификаторы используемые
		// для сортировки, автоматизации или отслеживания
		std::string mailbox_{};
		// hostname — доменн, субдоменн, сервер, сервис, ip - адрес, хост
		std::string hostname_{};
	};

	class phone_number
	{
	public:
		phone_number() = default;
		phone_number(const std::string& number);
		phone_number(const phone_number&) = default;
		phone_number(phone_number&&) = default;
		~phone_number() = default;

		phone_number& operator=(const phone_number&) = default;
		phone_number& operator=(phone_number&&) = default;

		std::string get() const;

	private:
		void normalization(const std::string& number);
	
	public:
		// Североамериканский план нумерации
		enum code_sizes { country = 2, zone = 3, node = 3, number = 4 };
	
	private:
		// code_[0] - код страны
		// code_[1] - код зоны, допустимые значения: [2-9] для первой цифры, [0-9] - для второй и третьей
		// code_[2] - код узла, допустимые значения: [2-9] для первой цифры, [0-9] - для второй и третьей
		// code_[3] - уникальный 4-значный номер в пределах телефонного узла, [0-9] для каждой из цифр
		std::vector<std::string> codes_{};
		std::string normalized_number_{};
	};
}

#endif // !TYPES_H
